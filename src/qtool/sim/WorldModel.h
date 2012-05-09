
#pragma once

#include "FieldConstants.h"
#include "noggin/CombinationObjects.h"

namespace qtool {
namespace sim {

typedef noggin::Location Ball;
typedef noggin::RobotLocation Robot;

class WorldModel {

public:
    WorldModel() : ball(CENTER_FIELD_X, CENTER_FIELD_Y),
                   robot(CENTER_FIELD_X - 40.0f, CENTER_FIELD_Y, 0.0f) {
    }

    Ball ball;
    Robot robot;

};

}
}
