//
// Created by João Pedro Barbosa on 14/11/2023.
//

#ifndef GAME_PROJECT_GAME_HPP
#define GAME_PROJECT_GAME_HPP

#include "Player.h"
#include "Enemy.h"

struct Tree{
    int value;
    Tree * lchild;
    Tree * rchild;
};

class Game {
private:
    sf::RenderWindow *window;
    sf::VideoMode videoMode;
    sf::Event ev;
    sf::Texture mainBackground;
    int bulletSpeed = 5.f;

    // Game state
    /*
     * state = 0 : character creation
     * state = 1 : map navigation
     * state = 2 : level
     */
    Tree * statsTree;
    bool characterChoice;
    bool gunChoice;

    // Game Objects
    Player * player;
    Enemy * firstEnemy;

    // Mouse position
    sf::Vector2i mousePos;
    sf::Vector2f mousePosView;

    // Sprites
    sf::Sprite background;
    sf::RectangleShape outline;
    std::vector<sf::Sprite> * playerBullets;
    std::vector<sf::Sprite> * enemyBullets;
    std::vector<sf::Texture> * mugshots;

    // Fonts
    sf::Font font;
    sf::Text text;

    // Private functions
    void initGameVars();
    void initWindow();
    void initTextures();
    void initFont();
    void displayChars(const std::string path, int scale);
    void setOutline();
    void setCharacter();
    void updateMousePos();
public:

    // Constructor and destructor
    Game();
    ~Game();

    // Accessors
    bool running() const&;

    // Functions

    void pollEvents();
    void updateGame();
    void updateBullets();
    bool bulletCheckCollisions(sf::Sprite * bullet);
    void renderVector(std::vector<sf::Sprite> * spriteVector);
    void renderGame();
};


#endif //GAME_PROJECT_GAME_HPP
