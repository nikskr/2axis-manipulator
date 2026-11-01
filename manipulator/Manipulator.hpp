#pragma once
#include <SFML/System/Vector2.hpp>
#include "Joint.hpp"

class Manipulator {
public:
    float L;
    Joint joint1;
    Joint joint2;
    sf::Vector2f basePos;

    Manipulator(float armLength, sf::Vector2f base);

    sf::Vector2f getAbsoluteEndEffectorPos();
    bool inverseKinematics(sf::Vector2f targetPos, float& q1, float& q2);
    void moveToTarget(sf::Vector2f targetPos, float V, float A, bool& outReachable);
    void update(float dt);
    bool isMoving();
};
