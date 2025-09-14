#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "map.h"
#include "player.h"
#include "renderer.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_W,SCREEN_H), "Raycaster",
        sf::Style::Close | sf::Style::Titlebar);

    std::vector<std::vector<int>> grid = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 0 ,0 ,0 ,0, 0, 0 ,1},
        {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    float cellSize = 48.0f;                         //Tamanho da célula

    Map map(cellSize, grid);                     //Mapa
    sf::Clock gameClock;                            //gameClock
    Player player;                                  //jogador
    player.position = sf::Vector2f(96,96);
    Renderer renderer;

    /***************************************************************************/

    while (window.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        player.update(deltaTime);                           //atualiza estados do jogador
        window.clear();

        /* Minimapa */
        //map.draw(window);                                 //Desenha Mapa
        //player.draw(window);                              //Desenha jogador
        //renderer.drawRays(window, player, map);           //Desenha raios

        /* cena "3d" */
        renderer.draw3dView(window,player,map);
        window.display();
    }
}