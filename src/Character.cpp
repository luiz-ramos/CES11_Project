//
// Created by ramos on 11/19/2023.
//

#include "Character.h"
#define PI 3.14159265

// Private functions
void Character::initVars() {
    health = 5;
    damage = 1;
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
    shadow.setScale(3,3);
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
void Character::updateGun(sf::Vector2f target) {
    this->gun.setPosition(this->character.getGlobalBounds().left,this->character.getGlobalBounds().top);

    sf::Vector2f dir = target - this->character.getPosition();
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

void Character::fireGun(std::vector<sf::Sprite> *bullets) {
    sf::Sprite bullet;
    float angle = this->getGun().getRotation();
    bullet.setTexture(this->getBulletTexture());
    bullet.setScale(1,1);
    bullet.setPosition(this->getGun().getPosition());
    bullet.setRotation(angle);
    angle = angle * PI/180;
    bullet.move(60 * cos(angle), 60 * sin(angle));

    bullets->push_back(bullet);
}

void Character::bulletCollision(std::vector<sf::Sprite> *bullets) {
    auto itr = bullets->begin();

    for (itr; itr < bullets->end(); itr++){
        if ((*itr).getGlobalBounds().intersects(this->character.getGlobalBounds())){
            bullets->erase(itr);
        }
    }
}

void Character::upgrade(int healthUp, int damageUp) {
    health += healthUp;
    damage += damageUp;
}

void Character::render(sf::RenderTarget *target) {
    target->draw(this->character);
    target->draw(this->shadow);
    target->draw(this->gun);
}
