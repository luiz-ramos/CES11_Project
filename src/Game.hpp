//
// Created by João Pedro Barbosa on 14/11/2023.
//

#ifndef GAME_PROJECT_GAME_HPP
#define GAME_PROJECT_GAME_HPP

#include "Player.h"

class Game {
private:
    sf::RenderWindow *window;
    sf::VideoMode videoMode;
    sf::Event ev;
    sf::Texture mainBackground;

    // Game Objects
    sf::Sprite background;
    Player player;

    // Private functions
    void initVars();
    void initWindow();
    void initBackground();

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
