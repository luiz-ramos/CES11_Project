//
// Created by João Pedro Barbosa on 14/11/2023.
//

#ifndef GAME_PROJECT_GAME_HPP
#define GAME_PROJECT_GAME_HPP

#include "Player.h"
#include "Enemy.h"
#include "Tree.h"

class Game {
private:
    sf::RenderWindow *window;
    sf::VideoMode videoMode;
    sf::Event ev;
    int bulletSpeed;
    bool exit;

    // Game state
    /*
     * state = -1: menu
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

    // Sprites, Shapes and Text
    sf::Sprite background;
    sf::RectangleShape outline;
    std::vector<sf::Text> * uiTexts;
    std::vector<sf::Sprite> * uiSprites;
    std::vector<sf::Sprite> * playerBullets;
    std::vector<sf::Sprite> * enemyBullets;

    // Textures
    sf::Texture worldBackground;
    sf::Texture levelBackground;
    std::vector<sf::Texture> * mugshots;

    // Text specific
    sf::Font font;
    sf::Text text;

    // Initializers
    void initGameVars();
    void initWindow();
    void initTextures();
    void initFont();
    void initTree();

    // Update functions
    void reset();
    template <typename T>
    void updateOutline(T * objectsVector);
    void updateMousePos();
    void updateBullets();
    bool updateBulletCollisions(sf::Sprite * bullet);

    // Render functions
    template <typename T>
    void renderVector(T * objectsVector);

    // UI related
    void initMenu();
    void initExit();
    void save();
    void displayChars(std::string path, int scale);

    // Polling
    void pollEvents();
    int pollUiChoices();
    int pollExit();
    void pollCharacterChoice();
public:

    // Constructor and destructor
    Game();
    ~Game();

    // Accessors
    bool running() const&;

    // Functions
    void updateGame();
    void renderGame();
};


#endif //GAME_PROJECT_GAME_HPP
