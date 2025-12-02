#include <./modules/move/xy_move.h>
#include <./modules/motor/motor.h>
#include <./modules/motor/double_motor.h>

XYMove::XYMove(DoubleMotor* axis_x, Motor* axis_y) {
    this->axis_x = axis_x;
    this->axis_y = axis_y;
}

void XYMove::run(long int steps_x, long int steps_y, int step_delay) {
    if (steps_x > 0) axis_x->change_direction(HIGH);
    else axis_x->change_direction(LOW);

    if (steps_y > 0) axis_y->change_direction(HIGH);
    else axis_y->change_direction(LOW);

    long dx = abs(steps_x), dy = abs(steps_y);
    long error = dx - dy;

    while (dx > 0 || dy > 0) {
        long prev_dx = dx, prev_dy = dy;

        if (error > -dy) {
            error -= dy;
            dx--;

            axis_x->run(1, step_delay);
        }

        if (error < dx) {
            error += dx;
            dy--;

            axis_y->run(1, step_delay);
        }

        if (dx == prev_dx && dy == prev_dy) {
            break;
        }
    }
}