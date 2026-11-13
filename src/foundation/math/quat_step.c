/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "math_quat.h"

quatd_t quatd_step(quatd_t q, vec3d_t speed, double time)
{
    double rate = vec3d_get_length(speed);
    double angle = rate * time;
    quatd_t turn;

    if (angle == 0.0)
        return (q);
    turn = quatd_make_turn(speed / rate, angle);
    return (quatd_get_unit(quatd_get_product(turn, q)));
}
