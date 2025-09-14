//
// Created by usuario on 14/09/25.
//

#include "player.h"

#include <cmath>
#include <iostream>
#include <math.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

constexpr float PI = 3.14159265359f;      //Constante "pi"
constexpr float TURN_SPEED = 100.0f;      //Velocidade de "giro"
constexpr float MOVE_SPEED = 100.0f;      //Velocidade de movimento

void Player::draw(sf::RenderTarget &target) {
    sf::CircleShape circle(8.0f);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(position);
    circle.setFillColor(sf::Color::Yellow);

    sf::RectangleShape line(sf::Vector2f(24.0f, 2.0f));
    line.setPosition(position);
    line.setFillColor(sf::Color::Yellow);
    line.setRotation(angle);

    //DRAW
    target.draw(line);
    target.draw(circle);
}

void Player::update(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        angle -= TURN_SPEED * deltaTime;
        //std::cout << "A" << '\n';
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        angle += TURN_SPEED * deltaTime;
        //std::cout << "D" << '\n';
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        float radians = angle * PI / 180.0f;
        position.x += cos(radians) * MOVE_SPEED * deltaTime;
        position.y += sin(radians) * MOVE_SPEED * deltaTime;
        //std::cout << "W" << '\n';
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        //std::cout << "S" << '\n';
    }

    //std::cout << angle << '\n';

}