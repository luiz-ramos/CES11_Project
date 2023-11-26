//
// Created by ramos on 11/18/2023.
//

#include "Player.h"
#define PI 3.14159265


// Constructors and Destructors
Player::Player(int playerID, int gunId, float x, float y) : Character(playerID, gunId, x, y){
    movementSpeed = 3.f;
    this->animationTimer.restart();
}

Player::~Player() {
    delete characterTextures;
}

// Functions
void Player::goTowards(sf::Vector2f targetPos) {
    this->resetAnimationTimer();
    if (targetPos.x > this->character.getGlobalBounds().getPosition().x)
        this->animationState = ANIMATION_STATES::RUN_FRONT_RIGHT;
    else if (targetPos.x < this->character.getGlobalBounds().getPosition().x)
        this->animationState = ANIMATION_STATES::RUN_FRONT_LEFT;

    if (targetPos.y > this->character.getGlobalBounds().getPosition().y)
        this->animationState = ANIMATION_STATES::RUN_BACK;
    else if (targetPos.y < this->character.getGlobalBounds().getPosition().y)
        this->animationState = ANIMATION_STATES::RUN_FRONT;
}

void Player::moveTowards(sf::Vector2f targetPos) {
    float x = targetPos.x - this->character.getPosition().x;
    float y = targetPos.y - this->character.getPosition().y;

    float dir = sqrt(pow(x,2) + pow(y,2));
    x /= dir;
    y /= dir;

    this->character.move(this->movementSpeed * x, this->movementSpeed * y);
}

void Player::changePos(sf::Vector2f targetPos) {
    this->character.setPosition(targetPos);
}

void Player::updateInput() {
    float factor = sqrt(2)/2;
    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
              sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
                 sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

    if (this->animationState == ANIMATION_STATES::RUN_FRONT_LEFT ||
        this->animationState == ANIMATION_STATES::RUN_BACK_LEFT ||
        this->animationState == ANIMATION_STATES::IDLE_FRONT_LEFT||
        this->animationState == ANIMATION_STATES::IDLE_BACK_LEFT)

    {this->unFlipSprite();}

    if (up){
        if (right){
            this->animationState = ANIMATION_STATES::RUN_BACK_RIGHT;
            this->character.move(factor * this->movementSpeed, -factor* this->movementSpeed);
        }
        else if (left){
            this->animationState = ANIMATION_STATES::RUN_BACK_LEFT;
            this->character.move(-factor * this->movementSpeed, -factor* this->movementSpeed);
        }
        else{
            this->animationState = ANIMATION_STATES::RUN_BACK;
            this->character.move(0.f, -this->movementSpeed);
        }
    }
    else if (down) {
        if (right) {
            this->animationState = ANIMATION_STATES::RUN_FRONT_RIGHT;
            this->character.move(factor * this->movementSpeed, factor * this->movementSpeed);
        } else if (left) {
            this->animationState = ANIMATION_STATES::RUN_FRONT_LEFT;
            this->character.move(-factor * this->movementSpeed, factor * this->movementSpeed);
        } else {
            this->animationState = ANIMATION_STATES::RUN_FRONT;
            this->character.move(0.f, this->movementSpeed);
        }
    }
    else if (right){
        this->animationState = ANIMATION_STATES::RUN_FRONT_RIGHT;
        this->character.move(this->movementSpeed, 0.f);
    }
    else if (left){
        this->animationState = ANIMATION_STATES::RUN_FRONT_LEFT;
        this->character.move(-this->movementSpeed, 0.f);
    }
    else{
        switch (animationState){
            case ANIMATION_STATES::RUN_FRONT_RIGHT:
                this->animationState = ANIMATION_STATES::IDLE_FRONT_RIGHT;
                break;
            case ANIMATION_STATES::RUN_FRONT_LEFT:
                this->animationState = ANIMATION_STATES::IDLE_FRONT_LEFT;
                break;
            case ANIMATION_STATES::RUN_FRONT:
                this->animationState = ANIMATION_STATES::IDLE_FRONT;
                break;
            case ANIMATION_STATES::RUN_BACK_RIGHT:
                this->animationState = ANIMATION_STATES::IDLE_BACK_RIGHT;
                break;
            case ANIMATION_STATES::RUN_BACK_LEFT:
                this->animationState = ANIMATION_STATES::IDLE_BACK_LEFT;
                break;
            case ANIMATION_STATES::RUN_BACK:
                this->animationState = ANIMATION_STATES::IDLE_BACK;
                break;
        }
    }

    shadow.setPosition(this->character.getGlobalBounds().left + this->character.getGlobalBounds().width/4,
                       this->character.getGlobalBounds().top + 6 * this->character.getGlobalBounds().height/7);
}

void Player::updateCollisions(const sf::RenderTarget * target) {
    sf::FloatRect playerBounds = this->character.getGlobalBounds();

    if (playerBounds.top <= 45.f)
        this->character.setPosition(playerBounds.left, 45.f);

    else if (playerBounds.top + playerBounds.height >= target->getSize().y - 45.f)
        this->character.setPosition(playerBounds.left, target->getSize().y - playerBounds.height - 45.f);

    if (playerBounds.left <= 45.f)
        this->character.setPosition(45.f, playerBounds.top);

    else if (playerBounds.left + playerBounds.width >= target->getSize().x - 45.f)
        this->character.setPosition(target->getSize().x - playerBounds.width - 45.f, playerBounds.top);


}

void Player::update(const sf::RenderWindow * target, sf::Vector2f gunTarget) {

    // Keyboard input
    this->updateInput();
    this->updateAnimations();

    // Gun direction
    this->updateGun(gunTarget);

    // Collisions
    updateCollisions(target);
}
