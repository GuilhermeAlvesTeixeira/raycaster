//
// Created by usuario on 19/09/25.
//

#ifndef EDITOR_H
#define EDITOR_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

class Editor {
public:
    void init(sf::RenderWindow &window);
    void run(sf::RenderWindow &window);
    void handleEvent(const sf::Event &event);
private:
    bool isFirstMouse{};
    sf::Vector2i lastMousePos;
    sf::View view;
};



#endif //EDITOR_H
