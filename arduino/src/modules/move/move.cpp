#include <./modules/move/move.h>
#include <./modules/motor/motor.h>
#include <./modules/motor/double_motor.h>

Move::Move(Motor* axis_x, DoubleMotor* axis_y) {
    this->axis_x_ = axis_x;
    this->axis_y_ = axis_y;
}

void Move::run(long steps_x, long steps_y) {
    if (steps_x > 0) axis_x_->change_direction(HIGH);
    else axis_x_->change_direction(LOW);

    if (steps_y > 0) axis_y_->change_direction(HIGH);
    else axis_y_->change_direction(LOW);

    long dx = abs(steps_x), dy = abs(steps_y);
    long error = dx - dy;

    while (dx > 0 || dy > 0) {
        long prev_dx = dx, prev_dy = dy;

        if (error > -dy) {
            error -= dy;
            dx--;

            axis_x_->run(1);
        }

        if (error < dx) {
            error += dx;
            dy--;

            axis_y_->run(1);
        }

        if (dx == prev_dx && dy == prev_dy) {
            break;
        }
    }
}