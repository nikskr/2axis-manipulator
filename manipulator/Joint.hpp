#pragma once

class Joint {
public:
    float currentAngle = 0.0f;
    float targetAngle = 0.0f;
    float startAngle = 0.0f;

    float maxV = 0.0f;
    float accel = 0.0f;

    float t_a = 0.0f;
    float t_v = 0.0f;
    float t_total = 0.0f;
    float timeElapsed = 0.0f;
    bool isMoving = false;
    float actualMaxV = 0.0f;

    void initMotion(float target, float V, float A);
    void update(float dt);
};
