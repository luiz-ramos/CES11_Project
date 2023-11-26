//
// Created by ramos on 11/19/2023.
//

#ifndef GAME_PROJECT_CHARACTER_H
#define GAME_PROJECT_CHARACTER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <filesystem>
#include <string>
#include <cmath>
#include <iostream>

enum ANIMATION_STATES {IDLE_FRONT, IDLE_BACK, IDLE_FRONT_RIGHT, IDLE_FRONT_LEFT, IDLE_BACK_RIGHT, IDLE_BACK_LEFT,
    RUN_FRONT, RUN_BACK, RUN_FRONT_RIGHT, RUN_FRONT_LEFT, RUN_BACK_RIGHT, RUN_BACK_LEFT};

class Character {
protected:
    sf::Clock animationTimer;
    short animationState;
    bool animationSwitch;

    sf::Sprite character;
    std::vector<sf::Texture> * characterTextures;

    sf::Sprite shadow;
    sf::Texture shadowTexture;

    sf::Sprite gun;
    sf::Texture gunTexture;

    sf::Texture bulletTexture;

    int current;
    int health;
    int damage;

    // Private functions
    void initVars();
    void initCharTexture(int charId);
    void initGunTexture(int GunId);
    void flipSprite();
    void unFlipSprite();
    void loadTexture();
    void updateTexture(int begin, int end);
public:
    Character(int characterId, int gunId, float x = 0.f, float y = 0.f);
    ~Character();

    // Accessors
    sf::Sprite getCharacter() const&;
    sf::Sprite getGun() const&;
    sf::Texture getBulletTexture() const&;
    int getHealth() const&;
    bool getAnimSwitch();

    // Functions
    void resetAnimationTimer();
    void updateAnimations();
    void updateStats(int healthUp, int damageUp);
    void updateGun(sf::Vector2f target);
    void fireGun(std::vector<sf::Sprite> * bullets);
    void bulletCollision(std::vector<sf::Sprite> * bullets);
    void render(sf::RenderTarget * target);
};


#endif //GAME_PROJECT_CHARACTER_H
