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

class Character {
protected:
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

public:
    Character(int characterId, int gunId, float x = 0.f, float y = 0.f);
    ~Character();

    // Accessors
    sf::Sprite getGun() const&;
    sf::Texture getBulletTexture() const&;

    void upgrade(int healthUp, int damageUp);
    void render(sf::RenderTarget * target);
};


#endif //GAME_PROJECT_CHARACTER_H
