//
// Created by usuario on 14/09/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "player.h"
#include "map.h"

//constexpr float SCREEN_W = 1280;
//constexpr float SCREEN_H = 720;

constexpr float SCREEN_W = 1920;
constexpr float SCREEN_H = 1080;


class Renderer {
public:
    void init();
    void draw3dView(sf::RenderTarget &target, const Player &player, const Map &map);
private:
    sf::Texture wallTexture;
    sf::Image floorImage;
};

#endif //RENDERER_H
