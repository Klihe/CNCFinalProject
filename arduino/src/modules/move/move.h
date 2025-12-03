#ifndef XY_MOVE
#define XY_MOVE

#include <./modules/motor/motor.h>
#include <./modules/motor/double_motor.h>

class Move {
    public:
        DoubleMotor* axis_x;
        Motor* axis_y;

        Move(DoubleMotor* axis_x, Motor* axis_y);

        void run(long steps_x, long steps_y);
};

#endif