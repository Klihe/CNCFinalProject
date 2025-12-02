#ifndef XY_MOVE
#define XY_MOVE

#include <./modules/motor/motor.h>
#include <./modules/motor/double_motor.h>

class XYMove {
    public:
        DoubleMotor* axis_x;
        Motor* axis_y;

        XYMove(DoubleMotor* axis_x, Motor* axis_y);

        void run(long int steps_x, long int steps_y, int step_delay);
};

#endif