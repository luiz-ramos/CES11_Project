//
// Created by ramos on 11/19/2023.
//

#include "Character.h"
#define PI 3.14159265

// Private functions
void Character::initVars() {
    // Default animation state
    this->animationState = ANIMATION_STATES::IDLE_FRONT_RIGHT;
    this->animationSwitch = true;

    this->HPMax = 5;
    this->HP = 5;
    this->damage = 1;
}

void Character::initCharTexture(int charId) {
    std::string path;

    switch (charId){
        case 0:
            path = "../../src/sprites/characters/convict";
            break;
        case 1:
            path = "../../src/sprites/characters/marine";
            break;
        case 2:
            path = "../../src/sprites/characters/slinger";
            break;
    }

    for (const auto &entry : std::filesystem::directory_iterator(path)){
        sf::Texture fileTexture;
        fileTexture.loadFromFile(entry.path().string());
        this->characterTextures.push_back(fileTexture);
    }

    // Sets idle texture index
    this->currentIndex = 12;
    this->character.setTexture(characterTextures[this->currentIndex]);
    this->character.setScale(2, 2);

    // Sets shadow configurations
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

void Character::initGunSound(int GunId) {
//    int run = 0;
//    std::string path;
//
//    for (const auto &entry : std::filesystem::directory_iterator("../../src/sounds/guns")){
//        if (run == GunId){
//            this->gunSoundBuffer.loadFromFile(entry.path().string());
//            this->gunSound.setBuffer(this->gunSoundBuffer);
//        }
//        run++;
//    }
}

void Character::initHPBar() {
    this->HPOutline.setPosition(this->character.getPosition() - sf::Vector2f (0.f, 20.f));
    this->HPBar.setPosition(this->character.getPosition() - sf::Vector2f (0.f, 20.f));

    this->HPOutline.setSize(sf::Vector2f (this->character.getGlobalBounds().width,
                                          10.f));
    this->HPOutline.setFillColor(sf::Color::Black);

    this->HPBar.setFillColor(sf::Color::Red);
    this->HPBar.setOutlineColor(sf::Color::Yellow);
    this->HPBar.setOutlineThickness(2.f);
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
    // Redefines the characters sprite's texture
    this->character.setTexture(characterTextures[this->currentIndex]);
}

void Character::updateTexture(int begin, int end) {
    // Loops character animation through specific states
    if (this->animationTimer.getElapsedTime().asSeconds() >= 0.3f || this->getAnimSwitch()){
        this->currentIndex++;
        if (this->currentIndex < begin || this->currentIndex >= end)
            this->currentIndex = begin;

        this->animationTimer.restart();
        this->loadTexture();
    }
}

// Constructors and Destructors
Character::Character(int characterId, int gunId, float x, float y) {
    this->character.setPosition(x, y);
    this->initVars();

    this->initCharTexture(characterId);
    this->initGunTexture(gunId);
    this->initGunSound(gunId);
    this->initHPBar();
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

int Character::getHP() const &{
    return HP;
}

int Character::getMaxHP() const &{
    return HPMax;
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

void Character::updateHPBar() {
    this->HPOutline.setPosition(this->character.getPosition() - sf::Vector2f (0.f, 20.f));
    this->HPBar.setPosition(this->character.getPosition() - sf::Vector2f (0.f, 20.f));

    float metric = this->character.getGlobalBounds().width / this->HPMax;
    float healthSize = metric * HP;
    this->HPBar.setSize(sf::Vector2f (healthSize, 10.f));
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

//    this->gunSound.play();
    bullets->push_back(bullet);
}

void Character::cure() {
    this->HP = this->HPMax;
}

void Character::receiveDamage(int dmg) {
    this->HP -= dmg;
}

void Character::upgrade(int HPUp, int damageUp) {
    this->HPMax += HPUp;
    this->damage += damageUp;
}

void Character::render(sf::RenderTarget *target) {
    target->draw(this->character);
    target->draw(this->shadow);
    target->draw(this->HPOutline);
    target->draw(this->HPBar);
    target->draw(this->gun);
}

void Character::mapRender(sf::RenderTarget *target) {
    target->draw(this->character);
}
