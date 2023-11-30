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
    // Animation Related
    sf::Clock animationTimer;
    short animationState;
    bool animationSwitch;
    int currentIndex;

    // Character sprite
    sf::Sprite character;
    std::vector<sf::Texture> characterTextures;

    // Shadow
    sf::Sprite shadow;
    sf::Texture shadowTexture;

    // Gun
    sf::Sprite gun;
    sf::Texture gunTexture;
    sf::Texture bulletTexture;

    // HP related
    int HPMax;
    int HP;
    sf::RectangleShape HPOutline;
    sf::RectangleShape HPBar;

    int damage;

    // Private functions
    void initVars();
    void initCharTexture(int charId);
    void initGunTexture(int GunId);
    void initHPBar();
    void flipSprite();
    void unFlipSprite();
    void loadTexture();
    void updateTexture(int begin, int end);

public:
    Character(int characterId, int gunId, float x = 0.f, float y = 0.f);
    ~Character() = default;

    // Accessors
    sf::Sprite getCharacter() const&;
    sf::Sprite getGun() const&;
    sf::Texture getBulletTexture() const&;
    int getHP() const&;
    int getMaxHP() const&;
    bool getAnimSwitch();

    // Functions
    void resetAnimationTimer();
    void updateAnimations();
    void updateHPBar();
    void updateStats(int healthUp = 0, int damageUp = 0);
    void updateGun(sf::Vector2f target);
    void fireGun(std::vector<sf::Sprite> * bullets);
    void render(sf::RenderTarget * target);
    void mapRender(sf::RenderTarget * target);
};


#endif //GAME_PROJECT_CHARACTER_H
