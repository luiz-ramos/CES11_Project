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
    bool exit;
    bool powerUp;

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
    std::vector<std::shared_ptr<Enemy>> * enemies;
    int currentLevel;

    // Game Objects
    Player * player;
    std::vector<std::shared_ptr<Enemy>> * currentEnemies;

    // Mouse position
    sf::Vector2i mousePos;
    sf::Vector2f mousePosView;

    // Sprites, Shapes and Text
    sf::Sprite background;
    sf::RectangleShape outline;
    std::vector<sf::Text> * uiTexts;
    std::vector<sf::CircleShape> * mapShapes;
    std::vector<sf::RectangleShape> * uiShapes;
    std::vector<sf::Sprite> * uiSprites;
    std::vector<std::shared_ptr<sf::Sprite>> * playerBullets;
    std::vector<std::shared_ptr<sf::Sprite>> * enemyBullets;

    // Textures
    sf::Texture menuBackground;
    sf::Texture worldBackground;
    sf::Texture levelBackground;
    std::vector<sf::Texture> * textures;

    // Text specific
    sf::Font font;

    // Initializers
    void initGameVars();
    void initEnemies();
    void initWindow();
    void initTextures();
    void initFont();
    void initTree();
    void initNodesPos();
    void initMapShapes();

    // Update functions
    void reset();
    void resetBullets();
    template <typename T>
    void updateOutline(T * objectsVector);
    void updateUIShapes();
    void walk(int targetLevel);
    void updateCurrentEnemies();
    void updateMousePos();
    void updateBullets();

    // Render functions
    template <typename T>
    void renderVector(T * objectsVector);
    void renderSprites(std::vector<std::shared_ptr<sf::Sprite>> * spriteVector);

    // UI related
    void initMenu();
    void initExit();
    void initPowerUp();
    void switchToMap();
    int levelToPos(int level);
    void save();
    void load();
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
