#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "imgui-sfml/imgui-SFML.h"
#include "imgui/imgui.h"
#include "editor.h"
#include "map.h"
#include "player.h"
#include "renderer.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_W,SCREEN_H), "Raycaster",
        sf::Style::Close | sf::Style::Titlebar);

    window.setVerticalSyncEnabled((true));          //ajuda a evitar sobrecargas (tive um bug sinistro alternando de estado com 'Esc' sem isso)

    if (!ImGui::SFML::Init(window)) {
        std::cerr << "Erro ao inicializar o ImGui\n";
        return 1;
    }

    float cellSize = 48.0f;                         //Tamanho da célula

    Map map(cellSize, "../map.png");     //Mapa
    sf::Clock gameClock;                            //gameClock
    Player player;                                  //jogador
    player.position = sf::Vector2f(96,96);
    Renderer renderer;
    renderer.init();                                //inicializa render com texturas

    /********* Editor ***********/
    Editor editor{};
    editor.init(window);

    float timeAcc = 0.0f;                           //Acumulador para controlar a atualização do título
    float fps = 0.0f;

    enum class State {Editor, Game} state = State::Game;
    /***************************************************************************/

    while (window.isOpen()) {
        sf::Time deltaTime = gameClock.restart();
        ImGui::SFML::Update(window, deltaTime );

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape){
                state = state == State::Game ? State::Editor : State::Game;
            }

            if (state == State::Editor) {
                editor.handleEvent(event);
            }

            ImGui::SFML::ProcessEvent(window,event);
        }

        ImGui::ShowDemoWindow();
        window.clear();

        if (state == State::Game) {
            /* cena "3d" */
            window.setView(window.getDefaultView());
            player.update(deltaTime.asSeconds());                           //atualiza estados do jogador
            renderer.draw3dView(window,player,map);
        } else {
            map.draw(window);
            editor.run(window, map);
        }

        ImGui::SFML::Render(window);
        window.display();

        timeAcc += deltaTime.asSeconds();
        if (timeAcc >= 1.0f) {
            if (deltaTime.asSeconds() > 0.0f) {
                fps = 1.0f / deltaTime.asSeconds();
            }
        }
        window.setTitle("Raycaster | " + std::to_string(1.0f / deltaTime.asSeconds()));    //Pegar o frameRate
        timeAcc = 0.0f;
    }
    ImGui::SFML::Shutdown();
}