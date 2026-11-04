/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static bool is_done(double elapsed, double deadline)
{
    return (elapsed >= deadline);
}

static uint64_t finishes(double elapsed, double start, double period,
    uint64_t handled)
{
    uint64_t total = (uint64_t)((elapsed - start) / period);

    return (total - handled);
}

static void show_once(void)
{
    static const double ELAPSED[] = { 0.5, 0.999, 1.0, 1.5 };

    printf("done");
    for (size_t i = 0; i < sizeof(ELAPSED) / sizeof(ELAPSED[0]); i++)
        printf(" %d", is_done(ELAPSED[i], 1.0));
    printf("\n");
}

static void show_repeat(void)
{
    static const double ELAPSED[] = { 0.1, 0.6, 0.7, 1.3 };
    uint64_t handled = 0;

    printf("finishes");
    for (size_t i = 0; i < sizeof(ELAPSED) / sizeof(ELAPSED[0]); i++) {
        uint64_t count = finishes(ELAPSED[i], 0.0, 0.25, handled);

        handled += count;
        printf(" %llu", (unsigned long long)count);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "once")) {
        show_once();
        return (0);
    }
    if (argc > 1 && !strcmp(argv[1], "repeat")) {
        show_repeat();
        return (0);
    }
    printf("usage: timer once|repeat\n");
    return (2);
}
