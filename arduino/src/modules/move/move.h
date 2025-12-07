#ifndef XY_MOVE
#define XY_MOVE

#include <./modules/motor/motor.h>
#include <./modules/motor/double_motor.h>

class Move {
    private:
        Motor* axis_x_;
        DoubleMotor* axis_y_;
    public:
        Move(Motor* axis_x, DoubleMotor* axis_y);

        void run(long steps_x, long steps_y);
};

#endif