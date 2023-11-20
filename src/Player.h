//
// Created by Luiz Ramos on 11/18/2023.
//

#ifndef GAME_PROJECT_PLAYER_H
#define GAME_PROJECT_PLAYER_H

#include "Character.h"

class Player : public Character{
protected:
    float movementSpeed;

    // Mouse position
    sf::Vector2i mousePos;
    sf::Vector2f mousePosView;

    // Private functions
    void loadTexture();

public:

    // Constructors and Destructors
    Player(int playerID, int gunId, float x = 0.f, float y = 0.f);
    ~Player();

    // Functions
    void update(const sf::RenderWindow * target);
    void updateInput();
    void updateMousePos(const sf::RenderWindow * target);
    void updateGun();
    void updateCollisions(const sf::RenderTarget * target);
};


#endif //GAME_PROJECT_PLAYER_H
