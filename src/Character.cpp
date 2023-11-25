//
// Created by ramos on 11/19/2023.
//

#include "Character.h"
#define PI 3.14159265

// Private functions
void Character::initVars() {
    this->animationState = ANIMATION_STATES::IDLE_FRONT_RIGHT;
    this->animationSwitch = true;

    this->health = 5;
    this->damage = 1;
    this->characterTextures = new std::vector<sf::Texture>;
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

    this->current = 12;
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

void Character::flipSprite() {
    int width = this->character.getGlobalBounds().width;
    this->character.setScale(-2.f, 2.f);

    this->character.setOrigin(width/2.0, 0.f);
}

void Character::unFlipSprite() {
    this->character.setScale(2.f, 2.f);

    this->character.setOrigin(0.f, 0.f);
}

void Character::loadTexture() {
    this->character.setTexture(characterTextures->at(this->current));
}

void Character::updateTexture(int begin, int end) {
    if (this->animationTimer.getElapsedTime().asSeconds() >= 0.3f || this->getAnimSwitch()){
        this->current++;
        if (this->current < begin || this->current >= end)
            this->current = begin;

        this->animationTimer.restart();
        this->loadTexture();
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

sf::Sprite Character::getCharacter() const &{
    return character;
}

sf::Texture Character::getBulletTexture() const &{
    return bulletTexture;
}

int Character::getHealth() const &{
    return health;
}

bool Character::getAnimSwitch() {
    bool anim_switch = this->animationSwitch;

    if (this->animationSwitch)
        this->animationSwitch = false;

    return anim_switch;
}

// Functions
void Character::resetAnimationTimer() {
    this->animationTimer.restart();
    this->animationSwitch = true;
}

void Character::updateAnimations() {

    switch (animationState){
        // RUNNING
        case ANIMATION_STATES::RUN_FRONT_RIGHT:
            updateTexture(34,39);
            break;
        case ANIMATION_STATES::RUN_FRONT_LEFT:
            updateTexture(34,39);
            this->flipSprite();
            break;
        case ANIMATION_STATES::RUN_FRONT:
            updateTexture(28,33);
            break;
        case ANIMATION_STATES::RUN_BACK_RIGHT:
            updateTexture(22,27);
            break;
        case ANIMATION_STATES::RUN_BACK_LEFT:
            updateTexture(22,27);
            this->flipSprite();
            break;
        case ANIMATION_STATES::RUN_BACK:
            updateTexture(16,21);
            break;
            // IDLE
        case ANIMATION_STATES::IDLE_FRONT_RIGHT:
            updateTexture(12,15);
            break;
        case ANIMATION_STATES::IDLE_FRONT_LEFT:
            updateTexture(12,15);
            this->flipSprite();
            break;
        case ANIMATION_STATES::IDLE_FRONT:
            updateTexture(8,11);
            break;
        case ANIMATION_STATES::IDLE_BACK_RIGHT:
            updateTexture(4,7);
            break;
        case ANIMATION_STATES::IDLE_BACK_LEFT:
            updateTexture(4,7);
            this->flipSprite();
            break;
        case ANIMATION_STATES::IDLE_BACK:
            updateTexture(0,4);
            break;
    }
}

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

    if (angle >= 90 && angle <= 270){
        this->gun.setScale(2,-2);
        this->gun.setOrigin(0.f, this->gun.getGlobalBounds().width/4.f);
    }
    else{
        this->gun.setScale(2,2);
        this->gun.setOrigin(0.f,0.f);
    }

    this->gun.move(this->character.getGlobalBounds().width/2, 2 * this->character.getGlobalBounds().height/3);
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

void Character::updateStats(int healthUp, int damageUp) {
    health += healthUp;
    damage += damageUp;
}

void Character::render(sf::RenderTarget *target) {
    target->draw(this->character);
    target->draw(this->shadow);
    target->draw(this->gun);
}
