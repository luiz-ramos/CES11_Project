//
// Created by ramos on 11/19/2023.
//

#include "Character.h"

// Private functions
void Character::initVars() {
    health = 100;
    damage = 25;
    characterTextures = new std::vector<sf::Texture>;
}

void Character::initCharTexture(int charId) {
    int run = 0;
    std::string path;

    for (const auto &entry : std::filesystem::directory_iterator("../../src/sprites/characters")){
        if (run == charId)
            path = entry.path().string();
        run++;
    }

    for (const auto &entry : std::filesystem::directory_iterator(path)){
        sf::Texture fileTexture;
        fileTexture.loadFromFile(entry.path().string());
        this->characterTextures->push_back(fileTexture);
    }

    current = 1;
    this->character.setTexture(characterTextures->at(current));
    this->character.setScale(2, 2);

    shadowTexture.loadFromFile("../../src/sprites/characters/mr_shadow.png");
    shadow.setTexture(shadowTexture);
    shadow.setScale(2,2);
}

void Character::initGunTexture(int GunId) {
    int run = 0;
    std::string path;

    for (const auto &entry : std::filesystem::directory_iterator("../../src/sprites/gun")){
        if (run == GunId){
            gunTexture.loadFromFile(entry.path().string());
            gun.setTexture(gunTexture);
            gun.setScale(2,2);
        }
        run++;
    }

    run = 0;

    for (const auto &entry : std::filesystem::directory_iterator("../../src/sprites/projectile")){
        if (run == GunId)
            bulletTexture.loadFromFile(entry.path().string());

        run++;
    }
}

// Constructors and Destructors
Character::Character(int characterId, int gunId, float x, float y) {
    this->character.setPosition(x, y);
    this->initVars();

    initCharTexture(characterId);

    initGunTexture(gunId);
}

Character::~Character() {
    delete characterTextures;
}

// Accessors

sf::Sprite Character::getGun() const &{
    return gun;
}

sf::Texture Character::getBulletTexture() const &{
    return bulletTexture;
}

// Functions
void Character::upgrade(int healthUp, int damageUp) {
    health += healthUp;
    damage += damageUp;
}

void Character::render(sf::RenderTarget *target) {
    target->draw(this->character);
    target->draw(this->shadow);
    target->draw(this->gun);
}
