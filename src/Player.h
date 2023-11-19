//
// Created by Luiz Ramos on 11/18/2023.
//

#ifndef GAME_PROJECT_PLAYER_H
#define GAME_PROJECT_PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <filesystem>
#include <string>
#include <cmath>
#include <iostream>

class Player {
private:
    sf::Sprite player;
    sf::Sprite gun;
    std::vector<sf::Texture> * textures;
    sf::Texture gunTexture;
    int current;
    int health;
    float movementSpeed;

    // Mouse position
    sf::Vector2i mousePos;
    sf::Vector2f mousePosView;

    // Player position
    sf::Vector2f pos;

    // Private functions
    void initVars();
    void initTexture();
    void loadTexture();

public:

    // Constructors and Destructors
    Player(float x = 0.f, float y = 0.f);
    ~Player();

    // Functions
    void update(const sf::RenderWindow * target);
    void updateInput();
    void updateMousePos(const sf::RenderWindow * target);
    void updateGun();
    void updateColiisions();
    void render(sf::RenderTarget * target);
};


#endif //GAME_PROJECT_PLAYER_H
