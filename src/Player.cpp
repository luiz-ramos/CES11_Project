//
// Created by ramos on 11/18/2023.
//

#include "Player.h"
#define PI 3.14159265


// Private functions

void Player::initVars() {
    this->health = 100;

    this->movementSpeed = 5.f;
}

void Player::initTexture() {
    textures = new std::vector<sf::Texture>;
    std::string path = "../../src/sprites/marine";

    // Save all character sprites into a vector

    for (const auto &entry : std::filesystem::directory_iterator(path)){
        sf::Texture fileTexture;
        fileTexture.loadFromFile(entry.path().string());
        this->textures->push_back(fileTexture);
    }

    // Set initial sprite

    current = 1;

    this->player.setTexture(textures->at(current));
    this->player.setScale(2,2);

    // Gun texture
    this->gunTexture.loadFromFile("../../src/sprites/gun/m16_gl_fire_001.png");
    this->gun.setTexture(this->gunTexture);
    this->gun.setScale(2,2);
    this->gun.setRotation(90.f);
}

void Player::loadTexture() {
    this->player.setTexture(textures->at(this->current));
}

// Constructors and Destructors

Player::Player(float x, float y) {

    this->player.setPosition(x,y);
    this->gun.setPosition(x,y);
    this->gun.move(this->player.getGlobalBounds().width/2, this->player.getGlobalBounds().height/2);

    this->initVars();
    this->initTexture();
}

Player::~Player() {
    delete textures;
}

// Functions

void Player::updateInput() {
    bool check = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){

        this->player.move(-this->movementSpeed, 0.f);
        this->current = 6;
        this->player.setScale(2,2);
        this->loadTexture();
        check = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){

        this->player.move(this->movementSpeed, 0.f);
        this->current = 7;
        this->player.setScale(2,2);
        this->loadTexture();
        check = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){

        if (check){
            this->player.move(this->movementSpeed, 0.f);
        }
        this->player.move(0.f, -this->movementSpeed);
        this->current = 4;
        this->player.setScale(2,2);
        this->loadTexture();
        check = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){

        this->player.move(0.f, this->movementSpeed);
        this->current = 5;
        this->player.setScale(2,2);
        this->loadTexture();
        check = true;
    }

    if (!check && current > 3)
        current-=4;
}

void Player::updateMousePos(const sf::RenderWindow *target) {
    this->mousePos = sf::Mouse::getPosition(*target);
    this->mousePosView = target->mapPixelToCoords(this->mousePos);
}

void Player::updateGun() {
    this->gun.setPosition(this->player.getGlobalBounds().left,this->player.getGlobalBounds().top);

    sf::Vector2f dir = mousePosView - this->player.getPosition();
    float x = dir.x;
    float y = dir.y;

    dir.x /= pow(pow(x,2) + pow(y,2) , 0.5);
    dir.y /= pow(pow(x,2) + pow(y,2) , 0.5);

    x = dir.x;
    y = dir.y;

    float angle = acos(x) * 180/PI;

    if (y < 0)
        this->gun.setRotation(-angle);
    else
        this->gun.setRotation(angle);

    this->gun.move(this->player.getGlobalBounds().width/4, this->player.getGlobalBounds().height/2);
}

void Player::update(const sf::RenderWindow * target) {


    // Bounds collision

    // Keyboard input
    updateInput();

    // Mouse Positions
    updateMousePos(target);

    // Gun direction
    updateGun();
}

void Player::render(sf::RenderTarget *target) {
    target->draw(this->player);
    target->draw(this->gun);
}