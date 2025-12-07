#ifndef XY_MOVE
#define XY_MOVE

#include <./modules/motor/motor.h>
#include <./modules/motor/double_motor.h>

class Move {
    private:
        DoubleMotor* axis_x_;
        Motor* axis_y_;
    public:
        Move(DoubleMotor* axis_x, Motor* axis_y);

        void run(long steps_x, long steps_y);
};

#endif