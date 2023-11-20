//
// Created by ramos on 11/19/2023.
//

#include "Enemy.h"

// Private functions
void Enemy::initVars(int difficulty) {
    enemyLevel = difficulty;
    timer = 0;

    shadow.setPosition(this->character.getGlobalBounds().left + this->character.getGlobalBounds().width/10,
                       this->character.getGlobalBounds().top +  this->character.getGlobalBounds().height);
}

// Constructor
Enemy::Enemy(int difficulty, int characterId, int gunId,
             float x, float y) : Character(characterId, gunId, x, y){

    this->initVars(difficulty);
}

// Accessors
sf::Sprite Enemy::getCharSprite() const &{
    return this->character;
}

// Functions
void Enemy::shootCycle(std::vector<sf::Sprite> * bullets) {
    if (timer == 30 && health > 0){
        this->fireGun(bullets);
        timer = 0;
    }
    else
        timer++;
}



