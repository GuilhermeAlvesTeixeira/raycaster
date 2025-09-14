//
// Created by usuario on 14/09/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics/RenderTarget.hpp>

#include "player.h"
#include "map.h"

class Renderer {
public:
    void drawRays(sf::RenderTarget &target, const Player &player, const Map &map);
private:
};


#endif //RENDERER_H
