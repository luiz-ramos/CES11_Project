//
// Created by João Pedro Barbosa on 14/11/2023.
//

#ifndef GAME_PROJECT_GAME_HPP
#define GAME_PROJECT_GAME_HPP

#include "Player.h"
#include "Enemy.h"
#include "Tree.h"
#include "Graph.hpp"

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
    bool choice1;
    bool choice2;

    // Levels
    std::vector<sf::Vector2f> * nodesPos;
    GameMap * gameMap;
    std::vector<bool> wonLevels;
    std::vector<Enemy> * enemies;
    int currentLevel;

    // Game Objects
    Player * player;
    std::vector<Enemy> * currentEnemies;

    // Mouse position
    sf::Vector2i mousePos;
    sf::Vector2f mousePosView;

    // Sprites, Shapes and Text
    sf::Sprite background;
    sf::RectangleShape outline;
    std::vector<sf::Text> * uiTexts;
    std::vector<sf::CircleShape> * uiShapes;
    std::vector<sf::Sprite> * uiSprites;
    std::vector<sf::Sprite> * playerBullets;
    std::vector<sf::Sprite> * enemyBullets;

    // Textures
    sf::Texture worldBackground;
    sf::Texture levelBackground;
    std::vector<sf::Texture> * textures;

    // Text specific
    sf::Font font;
    sf::Text text;

    // Initializers
    void initGameVars();
    void initEnemies();
    void initWindow();
    void initTextures();
    void initFont();
    void initTree();
    void initNodesPos();
    void initShapes();

    // Update functions
    void reset();
    void resetBullets();
    template <typename T>
    void updateOutline(T * objectsVector);
    void walk(int targetLevel);
    void updateCurrentEnemies();
    void updateMousePos();
    void updateBullets();

    // Render functions
    template <typename T>
    void renderVector(T * objectsVector);

    // UI related
    void initMenu();
    void initExit();
    void switchToMap();
    int levelToPos(int level);
    void save();
    void displayChars(std::string path, int scale);

    // Polling
    void pollEvents();
    int pollUiChoices();
    void pollMapChoice();
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
