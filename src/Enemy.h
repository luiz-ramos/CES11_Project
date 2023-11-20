//
// Created by ramos on 11/19/2023.
//

#ifndef GAME_PROJECT_ENEMY_H
#define GAME_PROJECT_ENEMY_H

#include "Character.h"

class Enemy : public Character{
private:
    int enemyLevel;
    int timer;

    // Private functions
    void initVars(int difficulty);
public:

    // Constructors and Destructors
    Enemy(int difficulty = 0, int characterId = 0, int gunId = 0,
          float x = 0,  float y = 0);
    ~Enemy() = default;

    // Accessors
    sf::Sprite getCharSprite() const&;

    // Functions
    void shootCycle(std::vector<sf::Sprite> * bullets);
};


#endif //GAME_PROJECT_ENEMY_H
