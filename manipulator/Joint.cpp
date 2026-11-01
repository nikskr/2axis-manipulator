#include "Joint.hpp"
#include <cmath>

inline float sign(float val) {
    return (val > 0.0f) - (val < 0.0f);
}

void Joint::initMotion(float target, float V, float A) {
    targetAngle = target;
    startAngle = currentAngle;
    maxV = V;
    accel = A;
    timeElapsed = 0.0f;

    float delta = targetAngle - startAngle;
    if (std::abs(delta) < 1e-4) {
        isMoving = false;
        return;
    }

    isMoving = true;
    float absDelta = std::abs(delta);
    float t_ramp = maxV / accel;
    float dist_ramp = 0.5f * accel * t_ramp * t_ramp;

    if (2.0f * dist_ramp > absDelta) {
        actualMaxV = std::sqrt(accel * absDelta);
        t_a = actualMaxV / accel;
        t_v = 0.0f;
        t_total = 2.0f * t_a;
    }
    else {
        actualMaxV = maxV;
        t_a = t_ramp;
        t_v = (absDelta - 2.0f * dist_ramp) / maxV;
        t_total = 2.0f * t_a + t_v;
    }
}

void Joint::update(float dt) {
    if (!isMoving) return;

    timeElapsed += dt;
    if (timeElapsed >= t_total) {
        currentAngle = targetAngle;
        isMoving = false;
        return;
    }

    float delta = targetAngle - startAngle;
    float dir = sign(delta);
    float t = timeElapsed;
    float angleOffset = 0.0f;

    if (t < t_a) {
        angleOffset = 0.5f * accel * t * t;
    }
    else if (t < t_a + t_v) {
        float dist_a = 0.5f * accel * t_a * t_a;
        angleOffset = dist_a + actualMaxV * (t - t_a);
    }
    else {
        float dist_a = 0.5f * accel * t_a * t_a;
        float dist_v = actualMaxV * t_v;
        float t_d = t - t_a - t_v;
        angleOffset = dist_a + dist_v + (actualMaxV * t_d - 0.5f * accel * t_d * t_d);
    }

    currentAngle = startAngle + dir * angleOffset;
}
