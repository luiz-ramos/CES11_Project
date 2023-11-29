//
// Created by ramos on 11/19/2023.
//

#ifndef GAME_PROJECT_ENEMY_H
#define GAME_PROJECT_ENEMY_H

#include "Character.h"

class Enemy : public Character{
private:
    int enemyLevel;
    int characterId;
    int gunId;
    int timer;

    // Private functions
    void initVars(int enemyLevel_, int characterId_, int gunId_);
    void shootCycle(std::vector<sf::Sprite> * bullets);
    void updateEnemyDir();
public:

    // Constructors and Destructors
    Enemy(int enemyLevel_ = 0, int characterId_ = 0, int gunId_ = 0,
          float x = 0,  float y = 0);
    ~Enemy() = default;

    // Accessors
    int getEnemyLevel() const&;
    int getCharacterId() const&;
    int getGunId() const&;

    // Functions
    void updateEnemy(const sf::Vector2f playerTarget, std::vector<sf::Sprite> * enemyBullets);
};


#endif //GAME_PROJECT_ENEMY_H
