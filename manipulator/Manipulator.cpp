#include "Manipulator.hpp"
#include <cmath>

Manipulator::Manipulator(float armLength, sf::Vector2f base)
    : L(armLength), basePos(base) {
}

sf::Vector2f Manipulator::getAbsoluteEndEffectorPos() {
    float q1 = joint1.currentAngle;
    float q2 = joint2.currentAngle;

    float x1 = L * std::cos(q1);
    float y1 = L * std::sin(q1);

    float x2 = x1 + L * std::cos(q1 + q2);
    float y2 = y1 + L * std::sin(q1 + q2);

    return basePos + sf::Vector2f(x2, -y2);
}

bool Manipulator::inverseKinematics(sf::Vector2f targetPos, float& q1, float& q2) {
    float dx = targetPos.x - basePos.x;
    float dy = -(targetPos.y - basePos.y);

    float r = std::sqrt(dx * dx + dy * dy);
    if (r > 2.0f * L || r < 0.0f) {
        return false;
    }

    float cos_q2 = (dx * dx + dy * dy - 2.0f * L * L) / (2.0f * L * L);
    if (cos_q2 > 1.0f) cos_q2 = 1.0f;
    if (cos_q2 < -1.0f) cos_q2 = -1.0f;

    q2 = std::acos(cos_q2);

    float alpha = std::atan2(dy, dx);
    float beta = std::atan2(L * std::sin(q2), L + L * std::cos(q2));
    q1 = alpha - beta;

    return true;
}

void Manipulator::moveToTarget(sf::Vector2f targetPos, float V, float A, bool& outReachable) {
    float targetQ1, targetQ2;
    if (inverseKinematics(targetPos, targetQ1, targetQ2)) {
        joint1.initMotion(targetQ1, V, A);
        joint2.initMotion(targetQ2, V, A);
        outReachable = true;
    }
    else {
        outReachable = false;
    }
}

void Manipulator::update(float dt) {
    joint1.update(dt);
    joint2.update(dt);
}

bool Manipulator::isMoving() {
    return joint1.isMoving || joint2.isMoving;
}
