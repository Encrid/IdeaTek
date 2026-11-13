/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "math_vec3.h"

typedef struct plain {
    double x;
    double y;
    double z;
} plain_t;

typedef struct plain_body {
    plain_t speed;
    plain_t spin;
    plain_t inertia;
    double mass;
} plain_body_t;

typedef struct stored_body {
    vec3d_t speed;
    vec3d_t spin;
    vec3d_t inertia;
    double mass;
} stored_body_t;

enum : uint32_t { STORED_ROOM = 4096 };

static stored_body_t STORED[STORED_ROOM];

typedef struct contact {
    double arm_a[3];
    double arm_b[3];
    double normal[3];
    double push;
    uint32_t a;
    uint32_t b;
} contact_t;

typedef struct scene {
    plain_body_t *plains;
    contact_t *contacts;
    uint32_t bodies;
    uint32_t count;
} scene_t;

static double next_number(uint64_t *state)
{
    *state = *state * 6364136223846793005ULL + 1442695040888963407ULL;
    return ((double)(*state >> 11) / 9007199254740992.0 - 0.5);
}

static plain_t plain_add(plain_t a, plain_t b)
{
    return ((plain_t){ a.x + b.x, a.y + b.y, a.z + b.z });
}

static plain_t plain_sub(plain_t a, plain_t b)
{
    return ((plain_t){ a.x - b.x, a.y - b.y, a.z - b.z });
}

static plain_t plain_scale(plain_t a, double s)
{
    return ((plain_t){ a.x * s, a.y * s, a.z * s });
}

static plain_t plain_times(plain_t a, plain_t b)
{
    return ((plain_t){ a.x * b.x, a.y * b.y, a.z * b.z });
}

static double plain_dot(plain_t a, plain_t b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

static plain_t plain_cross(plain_t a, plain_t b)
{
    return ((plain_t){ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x });
}

static void solve_vector(plain_body_t *a, plain_body_t *b, contact_t *c)
{
    vec3d_t ra = vec3d_load(c->arm_a);
    vec3d_t rb = vec3d_load(c->arm_b);
    vec3d_t n = vec3d_load(c->normal);
    vec3d_t speed_a = vec3d_load(&a->speed.x);
    vec3d_t speed_b = vec3d_load(&b->speed.x);
    vec3d_t spin_a = vec3d_load(&a->spin.x);
    vec3d_t spin_b = vec3d_load(&b->spin.x);
    vec3d_t at_a = speed_a + vec3d_get_cross(spin_a, ra);
    vec3d_t at_b = speed_b + vec3d_get_cross(spin_b, rb);
    vec3d_t turn_a = vec3d_load(&a->inertia.x) * vec3d_get_cross(ra, n);
    vec3d_t turn_b = vec3d_load(&b->inertia.x) * vec3d_get_cross(rb, n);
    vec3d_t both = vec3d_get_cross(turn_a, ra) + vec3d_get_cross(turn_b, rb);
    double weight = a->mass + b->mass + vec3d_get_dot(both, n);
    double push = -vec3d_get_dot(at_b - at_a, n) / weight;
    double total = c->push + push;

    total = total < 0.0 ? 0.0 : total;
    push = total - c->push;
    c->push = total;
    vec3d_store(speed_a - n * (push * a->mass), &a->speed.x);
    vec3d_store(spin_a - turn_a * push, &a->spin.x);
    vec3d_store(speed_b + n * (push * b->mass), &b->speed.x);
    vec3d_store(spin_b + turn_b * push, &b->spin.x);
}

static void solve_stored(stored_body_t *a, stored_body_t *b, contact_t *c)
{
    vec3d_t ra = vec3d_load(c->arm_a);
    vec3d_t rb = vec3d_load(c->arm_b);
    vec3d_t n = vec3d_load(c->normal);
    vec3d_t at_a = a->speed + vec3d_get_cross(a->spin, ra);
    vec3d_t at_b = b->speed + vec3d_get_cross(b->spin, rb);
    vec3d_t turn_a = a->inertia * vec3d_get_cross(ra, n);
    vec3d_t turn_b = b->inertia * vec3d_get_cross(rb, n);
    vec3d_t both = vec3d_get_cross(turn_a, ra) + vec3d_get_cross(turn_b, rb);
    double weight = a->mass + b->mass + vec3d_get_dot(both, n);
    double push = -vec3d_get_dot(at_b - at_a, n) / weight;
    double total = c->push + push;

    total = total < 0.0 ? 0.0 : total;
    push = total - c->push;
    c->push = total;
    a->speed = a->speed - n * (push * a->mass);
    a->spin = a->spin - turn_a * push;
    b->speed = b->speed + n * (push * b->mass);
    b->spin = b->spin + turn_b * push;
}

static void solve_plain(plain_body_t *a, plain_body_t *b, contact_t *c)
{
    plain_t ra = { c->arm_a[0], c->arm_a[1], c->arm_a[2] };
    plain_t rb = { c->arm_b[0], c->arm_b[1], c->arm_b[2] };
    plain_t n = { c->normal[0], c->normal[1], c->normal[2] };
    plain_t at_a = plain_add(a->speed, plain_cross(a->spin, ra));
    plain_t at_b = plain_add(b->speed, plain_cross(b->spin, rb));
    plain_t turn_a = plain_times(a->inertia, plain_cross(ra, n));
    plain_t turn_b = plain_times(b->inertia, plain_cross(rb, n));
    plain_t both = plain_add(plain_cross(turn_a, ra), plain_cross(turn_b, rb));
    double weight = a->mass + b->mass + plain_dot(both, n);
    double push = -plain_dot(plain_sub(at_b, at_a), n) / weight;
    double total = c->push + push;

    total = total < 0.0 ? 0.0 : total;
    push = total - c->push;
    c->push = total;
    a->speed = plain_sub(a->speed, plain_scale(n, push * a->mass));
    a->spin = plain_sub(a->spin, plain_scale(turn_a, push));
    b->speed = plain_add(b->speed, plain_scale(n, push * b->mass));
    b->spin = plain_add(b->spin, plain_scale(turn_b, push));
}

static void fill(scene_t *s)
{
    uint64_t state = 20261113;
    double numbers[9];

    for (uint32_t i = 0; i < s->bodies; i++) {
        for (int k = 0; k < 9; k++)
            numbers[k] = next_number(&state);
        s->plains[i].speed = (plain_t){ numbers[0], numbers[1], numbers[2] };
        s->plains[i].spin = (plain_t){ numbers[3], numbers[4], numbers[5] };
        s->plains[i].inertia = (plain_t){ numbers[6] + 1.0, numbers[7] + 1.0,
            numbers[8] + 1.0 };
        s->plains[i].mass = 1.0 + numbers[0] * numbers[0];
    }
    for (uint32_t i = 0; i < s->count; i++) {
        for (int k = 0; k < 3; k++) {
            s->contacts[i].arm_a[k] = next_number(&state);
            s->contacts[i].arm_b[k] = next_number(&state);
            s->contacts[i].normal[k] = k == 1 ? 1.0 : 0.0;
        }
        s->contacts[i].push = 0.0;
        s->contacts[i].a = (uint32_t)(state >> 20) % s->bodies;
        s->contacts[i].b = (s->contacts[i].a + 1 + i % 7) % s->bodies;
    }
}

static uint64_t sum_scene(const scene_t *s)
{
    uint64_t sum = 1469598103934665603ULL;
    uint64_t bits;
    double numbers[6];

    for (uint32_t i = 0; i < s->bodies; i++) {
        memcpy(numbers, &s->plains[i].speed, sizeof(numbers));
        for (int k = 0; k < 6; k++) {
            memcpy(&bits, &numbers[k], sizeof(bits));
            sum = (sum ^ bits) * 1099511628211ULL;
        }
    }
    return (sum);
}

static void copy_stored(scene_t *s, bool back)
{
    for (uint32_t i = 0; i < s->bodies; i++) {
        if (back) {
            vec3d_store(STORED[i].speed, &s->plains[i].speed.x);
            vec3d_store(STORED[i].spin, &s->plains[i].spin.x);
            continue;
        }
        STORED[i].speed = vec3d_load(&s->plains[i].speed.x);
        STORED[i].spin = vec3d_load(&s->plains[i].spin.x);
        STORED[i].inertia = vec3d_load(&s->plains[i].inertia.x);
        STORED[i].mass = s->plains[i].mass;
    }
}

static double run_stored(scene_t *s, uint32_t rounds)
{
    struct timespec from;
    struct timespec to;
    contact_t *c;

    copy_stored(s, false);
    timespec_get(&from, TIME_UTC);
    for (uint32_t r = 0; r < rounds; r++)
        for (uint32_t i = 0; i < s->count; i++) {
            c = &s->contacts[i];
            solve_stored(&STORED[c->a], &STORED[c->b], c);
        }
    timespec_get(&to, TIME_UTC);
    copy_stored(s, true);
    return ((double)(to.tv_sec - from.tv_sec) * 1e9
        + (double)(to.tv_nsec - from.tv_nsec));
}

static double run(scene_t *s, bool plain, uint32_t rounds)
{
    struct timespec from;
    struct timespec to;
    contact_t *c;

    timespec_get(&from, TIME_UTC);
    for (uint32_t r = 0; r < rounds; r++)
        for (uint32_t i = 0; i < s->count; i++) {
            c = &s->contacts[i];
            if (plain)
                solve_plain(&s->plains[c->a], &s->plains[c->b], c);
            else
                solve_vector(&s->plains[c->a], &s->plains[c->b], c);
        }
    timespec_get(&to, TIME_UTC);
    return ((double)(to.tv_sec - from.tv_sec) * 1e9
        + (double)(to.tv_nsec - from.tv_nsec));
}

int main(int argc, char **argv)
{
    bool plain = argc > 1 && !strcmp(argv[1], "plain");
    scene_t s = { .bodies = argc > 2 ? (uint32_t)atoi(argv[2]) : 1024,
        .count = argc > 3 ? (uint32_t)atoi(argv[3]) : 4096 };
    uint32_t rounds = argc > 4 ? (uint32_t)atoi(argv[4]) : 8;
    double nanos;

    s.plains = calloc(s.bodies, sizeof(*s.plains));
    s.contacts = calloc(s.count, sizeof(*s.contacts));
    if (!s.plains || !s.contacts || s.bodies < 9 || s.bodies > STORED_ROOM)
        return (1);
    fill(&s);
    if (argc > 1 && !strcmp(argv[1], "stored"))
        nanos = run_stored(&s, rounds);
    else
        nanos = run(&s, plain, rounds);
    printf("sum %016llx\n", (unsigned long long)sum_scene(&s));
    printf("time %.3f ns for each contact\n",
        nanos / ((double)s.count * (double)rounds));
    free(s.plains);
    free(s.contacts);
    return (0);
}
