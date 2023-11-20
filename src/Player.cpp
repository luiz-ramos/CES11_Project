//
// Created by ramos on 11/18/2023.
//

#include "Player.h"
#define PI 3.14159265


// Private functions

void Player::loadTexture() {
    this->character.setTexture(characterTextures->at(this->current));
}

// Constructors and Destructors

Player::Player(int playerID, int gunId, float x, float y) : Character(playerID, gunId, x, y){
    movementSpeed = 3.f;
}

Player::~Player() {
    delete characterTextures;
}

// Functions

void Player::updateInput() {
    bool check = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){

        this->character.move(-this->movementSpeed, 0.f);
        this->current = 6;
        this->loadTexture();
        check = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){

        this->character.move(this->movementSpeed, 0.f);
        this->current = 7;
        this->loadTexture();
        check = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){

        if (check){
            float change = 1 - 0.5 * sqrt(2);
            int i = pow(-1, current % 2);
            this->character.move(i * change * movementSpeed, 0.f);
        }
        this->character.move(0.f, -this->movementSpeed);
        this->current = 4;
        this->loadTexture();
        check = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){

        if (check){
            float change = 1 - 0.5 * sqrt(2);
            int i = pow(-1, current % 2);
            this->character.move(i * change * movementSpeed, 0.f);
        }
        this->character.move(0.f, this->movementSpeed);
        this->current = 5;
        this->loadTexture();
        check = true;
    }

    if (!check && current > 3)
        current -= 4;

    shadow.setPosition(this->character.getGlobalBounds().left + this->character.getGlobalBounds().width/4,
                       this->character.getGlobalBounds().top + 4 * this->character.getGlobalBounds().height/5);
}

void Player::updateMousePos(const sf::RenderWindow *target) {
    this->mousePos = sf::Mouse::getPosition(*target);
    this->mousePosView = target->mapPixelToCoords(this->mousePos);
}

void Player::updateGun() {
    this->gun.setPosition(this->character.getGlobalBounds().left,this->character.getGlobalBounds().top);

    sf::Vector2f dir = mousePosView - this->character.getPosition();
    float x = dir.x;
    float y = dir.y;

    dir.x /= pow(pow(x,2) + pow(y,2) , 0.5);
    dir.y /= pow(pow(x,2) + pow(y,2) , 0.5);

    x = dir.x;
    y = dir.y;

    float angle = acos(x) * 180/PI;

    if (y <= 0)
        this->gun.setRotation(-angle);
    else
        this->gun.setRotation(angle);

    this->gun.move(this->character.getGlobalBounds().width/4, this->character.getGlobalBounds().height/2);
}

void Player::updateCollisions(const sf::RenderTarget * target) {
    sf::FloatRect playerBounds = this->character.getGlobalBounds();

    if (playerBounds.left <= 45.f)
        this->character.setPosition(45.f, playerBounds.top);

    else if (playerBounds.left + playerBounds.width >= target->getSize().x - 45.f)
        this->character.setPosition(target->getSize().x - playerBounds.width - 45.f, playerBounds.top);

    if (playerBounds.top <= 45.f)
        this->character.setPosition(playerBounds.left, 45.f);

    else if (playerBounds.top + playerBounds.height >= target->getSize().y - 45.f)
        this->character.setPosition(playerBounds.left, target->getSize().y - playerBounds.height - 45.f);
}

void Player::update(const sf::RenderWindow * target) {

    // Keyboard input
    updateInput();

    // Mouse Positions
    updateMousePos(target);

    // Gun direction
    updateGun();

    // Collisions
    updateCollisions(target);
}
