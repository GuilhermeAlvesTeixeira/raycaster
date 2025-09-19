#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "map.h"
#include "player.h"
#include "renderer.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_W,SCREEN_H), "Raycaster",
        sf::Style::Close | sf::Style::Titlebar);

    window.setVerticalSyncEnabled((true));          //ajuda a evitar sobrecargas (tive um bug sinistro alternando de estado com 'Esc' sem isso)

    float cellSize = 48.0f;                         //Tamanho da célula

    Map map(cellSize, "../map.png");     //Mapa
    sf::Clock gameClock;                            //gameClock
    Player player;                                  //jogador
    player.position = sf::Vector2f(96,96);
    Renderer renderer;
    renderer.init();                                //inicializa render com texturas

    float timeAcc = 0.0f;                           //Acumulador para controlar a atualização do título
    float fps = 0.0f;

    enum class State {Editor, Game} state = State::Game;
    /***************************************************************************/

    while (window.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape){
                state = state == State::Game ? State::Editor : State::Game;
            }
        }

        window.clear();

        if (state == State::Game) {
            /* cena "3d" */
            player.update(deltaTime);                           //atualiza estados do jogador
            renderer.draw3dView(window,player,map);
        } else {
            //implementar isso daqui #24 5:11
        }

        window.display();

        timeAcc += deltaTime;
        if (timeAcc >= 1.0f) {
            if (deltaTime > 0.0f) {
                fps = 1.0f / deltaTime;
            }
        }
        window.setTitle("Raycaster | " + std::to_string(1.0f / deltaTime));    //Pegar o frameRate
        timeAcc = 0.0f;
    }
}