//
// Created by usuario on 14/09/25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics/RenderTarget.hpp>

class Player {
public:
    void draw(sf::RenderTarget &target);
    void update(float deltaTime);

    //Atributos
    sf::Vector2f position;
    float angle;
};



#endif //PLAYER_H
