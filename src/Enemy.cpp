//
// Created by ramos on 11/19/2023.
//

#include "Enemy.h"

// Private functions
void Enemy::initVars(int enemyLevel_, int characterId_, int gunId_) {
    this->enemyLevel = enemyLevel_;
    this->characterId = characterId_;
    this->gunId = gunId_;
    this->timer = 0;

    this->shadow.setPosition(this->character.getGlobalBounds().left + this->character.getGlobalBounds().width/4,
                       this->character.getGlobalBounds().top +  this->character.getGlobalBounds().height);
}

void Enemy::shootCycle(std::vector<std::shared_ptr<sf::Sprite>> * bullets) {
    if (timer == 50 && HP > 0){
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
Enemy::Enemy(int enemyLevel_, int characterId_, int gunId_,
             float x, float y) : Character(characterId_, gunId_, x, y){

    this->initVars(enemyLevel_, characterId_, gunId_);
    this->upgrade(4, 1);
}

// Accessors
int Enemy::getEnemyLevel() const &{
    return enemyLevel;
}

int Enemy::getCharacterId() const &{
    return characterId;
}

int Enemy::getGunId() const &{
    return gunId;
}

// Functions
void Enemy::updateEnemy(const sf::Vector2f playerTarget, std::vector<std::shared_ptr<sf::Sprite>> * enemyBullets) {
    // Updates the direction the enemy is facing
    this->updateEnemyDir();

    // Animation
    this->updateAnimations();

    // Health bar
    this->updateHPBar();

    // Gun related updates
    this->updateGun(playerTarget);
    this->shootCycle(enemyBullets);
}



