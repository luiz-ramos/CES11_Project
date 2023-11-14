//
// Created by João Pedro Barbosa on 14/11/2023.
//

#ifndef GAME_PROJECT_GAME_HPP
#define GAME_PROJECT_GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>


class Game {
private:
    sf::RenderWindow *window;
    sf::VideoMode videoMode;
    sf::Event ev;

    // Private functions
    void initVars();
    void initWindow();

public:

    // Constructor and destructor
    Game();
    ~Game();

    // Accessors
    bool running() const&;

    // Functions

    void pollEvents();
    void update();
    void render();
};


#endif //GAME_PROJECT_GAME_HPP
