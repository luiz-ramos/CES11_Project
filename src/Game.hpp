//
// Created by João Pedro Barbosa on 14/11/2023.
//

#ifndef GAME_PROJECT_GAME_HPP
#define GAME_PROJECT_GAME_HPP

#include "Player.h"

class Game : public Player {
private:
    sf::RenderWindow *window;
    sf::VideoMode videoMode;
    sf::Event ev;
    sf::Texture mainBackground;
    int bulletSpeed = 5.f;

    // Game Objects
    sf::Sprite background;
    std::vector<sf::Sprite> * bullets;

    // Private functions
    void initGameVars();
    void initWindow();
    void initTextures();

public:

    // Constructor and destructor
    Game();
    ~Game();

    // Accessors
    bool running() const&;

    // Functions

    void pollEvents();
    void updateGame();
    void updateBullets();
    bool bulletCheckCollisions(sf::Sprite * bullet);
    void renderGame();
};


#endif //GAME_PROJECT_GAME_HPP
