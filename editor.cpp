//
// Created by usuario on 19/09/25.
//

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "imgui/imgui.h"
#include "map.h"
#include "editor.h"

void Editor::init(sf::RenderWindow &window) {view = window.getDefaultView(); cell.setFillColor(sf::Color::Transparent); cell.setOutlineColor(sf::Color::Green); cell.setOutlineThickness(1.0f);}

void Editor::run(sf::RenderWindow &window, Map &map) {

    //Menu principal
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save")) {
                map.save("../map_bin.map");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (isFirstMouse) {
            lastMousePos = mousePos;
            isFirstMouse = false;
        } else {
            sf::Vector2i mouseDelta = mousePos - lastMousePos;
            view.setCenter(view.getCenter() - (sf::Vector2f)mouseDelta);
            sf::Mouse::setPosition(lastMousePos, window);
        }
        window.setMouseCursorVisible(false);
    } else {
        isFirstMouse = true;
        window.setMouseCursorVisible(true);
    }

    if (!ImGui::GetIO().WantCaptureMouse) {   //caso o Imgui não queira capturar o mouse
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        sf::Vector2i mapPos = (sf::Vector2i) (worldPos / map.getCellSize());

        cell.setSize(sf::Vector2f(map.getCellSize(), map.getCellSize()));
        cell.setPosition((sf::Vector2f) mapPos * map.getCellSize());

        window.draw(cell);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            map.setMapCell(mapPos.x, mapPos.y, sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? 0 : 1);
        }
        window.setView(view);
    }
}

void Editor::handleEvent(const sf::Event &event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        float zoom = 1.0f - 0.1f * event.mouseWheelScroll.delta;
        view.zoom(zoom);
    }
}