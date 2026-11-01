#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Manipulator.hpp"

struct TargetObject {
    sf::Vector2f pos;
    bool reachable = true;
    bool active = true;
};

int main() {
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "2-Axis Manipulator Model (SFML 3.0)");

    sf::Vector2f baseCenter(windowWidth / 2.0f, windowHeight / 2.0f);
    float armLength = 140.0f;
    Manipulator robot(armLength, baseCenter);

    float V = 2.0f;
    float A = 1.5f;

    std::vector<TargetObject> targets;
    size_t currentTargetIdx = 0;
    bool processingTarget = false;

    sf::Clock updateClock;
    float timeAccumulator = 0.0f;
    const float dt = 0.050f;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseButtonEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonEvent->button == sf::Mouse::Button::Left) {
                    TargetObject obj;
                    obj.pos = sf::Vector2f(static_cast<float>(mouseButtonEvent->position.x),
                        static_cast<float>(mouseButtonEvent->position.y));

                    float t1, t2;
                    obj.reachable = robot.inverseKinematics(obj.pos, t1, t2);
                    obj.active = true;

                    targets.push_back(obj);
                }
            }
        }

        timeAccumulator += updateClock.restart().asSeconds();
        while (timeAccumulator >= dt) {
            timeAccumulator -= dt;

            if (!processingTarget && currentTargetIdx < targets.size()) {
                if (targets[currentTargetIdx].active) {
                    bool canReach = false;
                    robot.moveToTarget(targets[currentTargetIdx].pos, V, A, canReach);
                    targets[currentTargetIdx].reachable = canReach;

                    if (canReach) {
                        processingTarget = true;
                    }
                    else {
                        currentTargetIdx++;
                    }
                }
            }

            if (processingTarget) {
                robot.update(dt);
                if (!robot.isMoving()) {
                    targets[currentTargetIdx].active = false;
                    processingTarget = false;
                    currentTargetIdx++;
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        sf::CircleShape accessZone(2.0f * armLength);
        accessZone.setFillColor(sf::Color(50, 50, 50, 100));
        accessZone.setOrigin({ 2.0f * armLength, 2.0f * armLength });
        accessZone.setPosition(baseCenter);
        window.draw(accessZone);

        sf::CircleShape baseCircle(10.0f);
        baseCircle.setFillColor(sf::Color::Blue);
        baseCircle.setOrigin({ 10.0f, 10.0f });
        baseCircle.setPosition(baseCenter);
        window.draw(baseCircle);

        for (const auto& obj : targets) {
            if (!obj.active && obj.reachable) continue;

            if (!obj.reachable) {
                sf::Vertex line1[] = {
                    sf::Vertex(obj.pos + sf::Vector2f(-8, -8), sf::Color::Red),
                    sf::Vertex(obj.pos + sf::Vector2f(8, 8), sf::Color::Red)
                };
                sf::Vertex line2[] = {
                    sf::Vertex(obj.pos + sf::Vector2f(8, -8), sf::Color::Red),
                    sf::Vertex(obj.pos + sf::Vector2f(-8, 8), sf::Color::Red)
                };
                window.draw(line1, 2, sf::PrimitiveType::Lines);
                window.draw(line2, 2, sf::PrimitiveType::Lines);
            }
            else {
                sf::CircleShape targetShape(6.0f);
                targetShape.setFillColor(sf::Color::Green);
                targetShape.setOrigin({ 6.0f, 6.0f });
                targetShape.setPosition(obj.pos);
                window.draw(targetShape);
            }
        }

        float q1 = robot.joint1.currentAngle;
        float q2 = robot.joint2.currentAngle;

        sf::Vector2f joint1Pos = baseCenter + sf::Vector2f(armLength * std::cos(q1), -armLength * std::sin(q1));
        sf::Vector2f endEffectorPos = robot.getAbsoluteEndEffectorPos();

        sf::Vertex link1[] = { sf::Vertex(baseCenter, sf::Color::White), sf::Vertex(joint1Pos, sf::Color::White) };
        window.draw(link1, 2, sf::PrimitiveType::Lines);

        sf::Vertex link2[] = { sf::Vertex(joint1Pos, sf::Color::White), sf::Vertex(endEffectorPos, sf::Color::White) };
        window.draw(link2, 2, sf::PrimitiveType::Lines);

        sf::CircleShape jointCircle(6.0f);
        jointCircle.setFillColor(sf::Color::Yellow);
        jointCircle.setOrigin({ 6.0f, 6.0f });
        jointCircle.setPosition(joint1Pos);
        window.draw(jointCircle);

        sf::CircleShape effectorCircle(8.0f);
        effectorCircle.setFillColor(sf::Color::Magenta);
        effectorCircle.setOrigin({ 8.0f, 8.0f });
        effectorCircle.setPosition(endEffectorPos);
        window.draw(effectorCircle);

        window.display();
    }

    return 0;
}
