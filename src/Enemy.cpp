//
// Created by ramos on 11/19/2023.
//

#include "Enemy.h"

// Private functions
void Enemy::initVars(int difficulty) {
    this->enemyLevel = difficulty;
    this->timer = 0;

    this->shadow.setPosition(this->character.getGlobalBounds().left + this->character.getGlobalBounds().width/4,
                       this->character.getGlobalBounds().top +  this->character.getGlobalBounds().height);
}

void Enemy::shootCycle(std::vector<sf::Sprite> * bullets) {
    if (timer == 50 && health > 0){
        this->fireGun(bullets);
        timer = 0;
    }
    else
        timer++;
}

void Enemy::updateEnemyDir() {
    if (this->animationState == ANIMATION_STATES::IDLE_FRONT_LEFT ||
        this->animationState == ANIMATION_STATES::IDLE_BACK_LEFT)
    {
        this->unFlipSprite();
    }

    float angle = this->gun.getRotation();
    bool quad4 = angle >= 270;
    bool quad3 = angle >= 180;
    bool quad2 = angle >= 90;

    if (quad4)
        this->animationState = ANIMATION_STATES::IDLE_BACK_RIGHT;
    else if (quad3)
        this->animationState = ANIMATION_STATES::IDLE_BACK_LEFT;
    else if (quad2)
        this->animationState = ANIMATION_STATES::IDLE_FRONT_LEFT;
    else
        this->animationState = ANIMATION_STATES::IDLE_FRONT_RIGHT;
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
void Enemy::updateEnemy(const sf::Vector2f playerTarget, std::vector<sf::Sprite> * enemyBullets) {
    this->updateEnemyDir();
    this->updateAnimations();
    this->updateGun(playerTarget);
    this->shootCycle(enemyBullets);
}



