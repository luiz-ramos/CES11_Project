//
// Created by João Pedro Barbosa on 14/11/2023.
//

#include "Game.hpp"
#define PI 3.14159265

// Initializers
void Game::initGameVars() {
    this->uiTexts = new std::vector<sf::Text>;
    this->uiSprites = new std::vector<sf::Sprite>;
    this->playerBullets = new std::vector<std::shared_ptr<sf::Sprite>>;
    this->enemyBullets = new std::vector<std::shared_ptr<sf::Sprite>>;
    this->textures = new std::vector<sf::Texture>;
    this->gameMap = new GameMap();
    this->nodesPos = new std::vector<sf::Vector2f>;
    this->mapShapes = new std::vector<sf::CircleShape>;
    this->uiShapes = new std::vector<sf::RectangleShape>;
    this->enemies = new std::vector<std::shared_ptr<Enemy>>;
    this->currentEnemies = new std::vector<std::shared_ptr<Enemy>>;

    this->currentLevel = 0;
    this->exit = false;
    this->powerUp = false;

    this->textures->reserve(5);
    for (int i = 0; i < 5; i++)
        this->wonLevels.push_back(false);
    this->enemies->reserve(5);
    this->currentEnemies->reserve(5);
}

void Game::initEnemies() {
    // Level 1
    std::shared_ptr<Enemy> enemy1(new Enemy(0,2,0,100,100));
    this->enemies->push_back(enemy1);

    // Level 2
    std::shared_ptr<Enemy> enemy2(new Enemy(0,2,0,100,600));
    this->enemies->push_back(enemy2);

    // Level 3
    std::shared_ptr<Enemy> enemy3(new Enemy(0,2,0,500,600));
    this->enemies->push_back(enemy3);

    // Level 4
    std::shared_ptr<Enemy> enemy4(new Enemy(0,2,0,500,100));
    this->enemies->push_back(enemy4);

    // Level 5
    std::shared_ptr<Enemy> enemy5(new Enemy(1,2,0,300,100));
    this->enemies->push_back(enemy5);
}

void Game::initWindow() {
    this->videoMode.height = 728;
    this->videoMode.width = 728;

    this->window = new sf::RenderWindow(this->videoMode, "CityRaid", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(144);
}

void Game::initTextures() {
    menuBackground.loadFromFile("../../src/sprites/menu/menu.jpeg");
    levelBackground.loadFromFile("../../src/sprites/map/mapaResenha.jpg");
    worldBackground.loadFromFile("../../src/sprites/map/mapa.jpg");
    background.setTexture(menuBackground);

    outline.setOutlineColor(sf::Color::White);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineThickness(2);
}

void Game::initFont() {
    font.loadFromFile("../../src/fonts/Minecraft.ttf");
}

void Game::initTree() {
    this->statsTree = make(-1,
                           make(0,
                                make(0),
                                make(0)),
                           make(0,
                                make(0),
                                make(0))
    );

    // Since player choices were not made yet
    this->choice1 = false;
    this->choice2 = false;
}

void Game::initNodesPos() {
    std::ifstream fileIn;
    this->nodesPos->resize(this->gameMap->getSize());
    fileIn.open("../../src/graphMaps/map.txt");

    for (int i = 0; i < this->gameMap->getSize(); i++)
        fileIn >> this->nodesPos->at(i).x >> this->nodesPos->at(i).y;

    fileIn.close();
}

void Game::initMapShapes() {
    struct uiCircle{
        sf::CircleShape circle;
        uiCircle(sf::Vector2f pos){
            circle.setRadius(20.f);
            circle.setPosition(pos);
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineThickness(5.f);
            circle.setOutlineColor(sf::Color::Black);
        }
    };
    float width = this->player->getCharacter().getGlobalBounds().width;
    float height = this->player->getCharacter().getGlobalBounds().height;
    sf::Vector2f adjust = sf::Vector2f {width/2, height/2};

    // Level 1
    this->mapShapes->emplace_back(uiCircle(this->nodesPos->at(0) - adjust).circle);

    // Level 2
    this->mapShapes->emplace_back(uiCircle(this->nodesPos->at(9) - adjust).circle);

    // Level 3
    this->mapShapes->emplace_back(uiCircle(this->nodesPos->at(12) - adjust).circle);

    // Level 4
    this->mapShapes->emplace_back(uiCircle(this->nodesPos->at(15) - adjust).circle);

    // Level 5
    this->mapShapes->emplace_back(uiCircle(this->nodesPos->at(19) - adjust).circle);
}

// Update functions
void Game::reset() {
    this->exit = false;
    this->powerUp = false;

    this->resetBullets();
    while (!this->mapShapes->empty())
        this->mapShapes->pop_back();
    while (!this->uiTexts->empty())
        this->uiTexts->pop_back();

    choice1 = false;
    choice2 = false;
}

void Game::resetBullets() {
    while (!this->playerBullets->empty())
        this->playerBullets->pop_back();
    while (!this->enemyBullets->empty())
        this->enemyBullets->pop_back();
}

template <typename T>
void Game::updateOutline(T * objectsVector) {
    // Updates the outline of UI elements
    bool check = false;
    auto itr = objectsVector->begin();

    for (itr; itr < objectsVector->end(); itr++)
        if ((*itr).getGlobalBounds().contains(this->mousePosView)){
            this->outline.setOutlineColor(sf::Color::White);
            this->outline.setPosition((*itr).getGlobalBounds().getPosition());
            this->outline.setSize((*itr).getGlobalBounds().getSize());
            check = true;
        }

    if (!check)
        this->outline.setOutlineColor(sf::Color::Transparent);
}

void Game::updateUIShapes() {
    // Highlights a shape if hovered over
    auto itr = this->mapShapes->begin();

    for (itr; itr < this->mapShapes->end(); itr++)
        if ((*itr).getGlobalBounds().contains(this->mousePosView)){
            (*itr).setFillColor(sf::Color::Yellow);
            return;
        }

    for (int i = 0; i < this->mapShapes->size(); i++)
        switch (this->wonLevels[i]){
            case false:
                this->mapShapes->at(i).setFillColor(sf::Color::Transparent);

                break;
            case true:
                this->mapShapes->at(i).setFillColor(sf::Color::Green);

                break;
        }
}

void Game::walk(int targetLevel) {
    // Walks from one level to another

    // Translates the level to the node index
    int mapPos = this->levelToPos(this->currentLevel);
    int targetPos = this->levelToPos(targetLevel);

    // Path vector to the target level
    std::vector<int> path = gameMap->dijkstra(mapPos+1, targetPos+1);

    // Adjusts the position to align the player center with the level circle center
    float width = this->player->getCharacter().getGlobalBounds().width;
    float height = this->player->getCharacter().getGlobalBounds().height;
    sf::Vector2f adjust = sf::Vector2f {width/2, height};

    // Walk through the vector path
    for (int i = 1; i < path.size(); i++){
        mapPos = path[i] - 1;
        this->player->goTowards(this->nodesPos->at(mapPos) - adjust);
        sf::Vector2f dist = this->nodesPos->at(mapPos) - this->player->getCharacter().getPosition() - adjust;
        float distMod = sqrt(pow(dist.x,2) + pow(dist.y,2));

        while (distMod > 10.f){
            this->window->clear();

            this->player->update(this->window, mousePosView);
            this->player->moveTowards(this->nodesPos->at(mapPos) - adjust);

            dist = this->nodesPos->at(mapPos) - this->player->getCharacter().getPosition() -adjust;
            distMod = sqrt(pow(dist.x,2) + pow(dist.y,2));

            this->window->draw(background);
            this->renderVector(uiSprites);
            this->player->mapRender(this->window);

            this->window->display();
        };
        this->player->changePos(this->nodesPos->at(mapPos) - adjust);
    }

    // Update player level position
    this->currentLevel = targetLevel;
}

void Game::updateCurrentEnemies() {
    while (!this->currentEnemies->empty())
        this->currentEnemies->pop_back();

    for (int i = 0; i < this->currentLevel + 1; i++){;
        this->enemies->at(i)->cure();
        this->currentEnemies->push_back(this->enemies->at(i));
    }
}

void Game::updateMousePos() {
    this->mousePos = sf::Mouse::getPosition(*(this->window));
    this->mousePosView = this->window->mapPixelToCoords(this->mousePos);
}

void Game::updateBullets() {
    // Updates bullet positions and checks collisions

    auto itr = this->playerBullets->begin();

    for (itr; itr < playerBullets->end(); itr++){
        int bulletSpeed = this->player->getBulletSpeed();
        std::shared_ptr<sf::Sprite> bullet = *itr;
        float angle = (*bullet).getRotation() * PI/180;

        (*bullet).move(bulletSpeed * cos(angle), bulletSpeed * sin(angle));

        auto itr2 = this->currentEnemies->begin();

        // Checks for collision with enemies
        for (itr2; itr2 < this->currentEnemies->end(); itr2++)
            if ((*itr)->getGlobalBounds().intersects((*itr2)->getCharacter().getGlobalBounds())){
                this->playerBullets->erase(itr);
                (*itr2)->receiveDamage(this->player->getDamage());
            }

        float xUp = this->window->getSize().x;
        float yUp = this->window->getSize().y;

        // Checks if bullet is out-of-bounds
        if ((*itr)->getPosition().x > xUp || (*itr)->getPosition().x < 0.f ||
            (*itr)->getPosition().y > yUp || (*itr)->getPosition().y < 0.f)
            this->playerBullets->erase(itr);

    }

    itr = this->enemyBullets->begin();

    for (itr; itr < enemyBullets -> end(); itr++){
        int bulletSpeed = this->enemies->at(0)->getBulletSpeed();
        std::shared_ptr<sf::Sprite> bullet = *itr;
        float angle = (*bullet).getRotation() * PI/180;

        // Checks collision with player
        if ((*itr)->getGlobalBounds().intersects(this->player->getCharacter().getGlobalBounds())){
            this->enemyBullets->erase(itr);
            this->player->receiveDamage(this->enemies->at(0)->getDamage());
        }

        float xUp = this->window->getSize().x;
        float yUp = this->window->getSize().y;

        // Checks if bullet is out of bounds
        if ((*itr)->getPosition().x > xUp || (*itr)->getPosition().x < 0.f ||
            (*itr)->getPosition().y > yUp || (*itr)->getPosition().y < 0.f)
            this->enemyBullets->erase(itr);

        (*bullet).move(bulletSpeed * cos(angle), bulletSpeed * sin(angle));
    }
}

// Render functions
template <typename T>
void Game::renderVector(T * objectsVector) {
    auto itr = objectsVector->begin();

    for (itr; itr < objectsVector->end(); itr++)
        this->window->draw(*itr);
}

void Game::renderSprites(std::vector<std::shared_ptr<sf::Sprite>> * spriteVector) {
    auto itr = spriteVector->begin();

    for (itr; itr < spriteVector->end(); itr++)
        this->window->draw(**itr);
}

// UI related
void Game::initMenu() {

    sf::Text newGame;
    sf::Text loadGame;
    newGame.setFont(font);
    loadGame.setFont(font);
    newGame.setFillColor(sf::Color::White);
    loadGame.setFillColor(sf::Color::White);
    newGame.setPosition(280, 264);
    loadGame.setPosition(270, 350);
    newGame.setString("NEW GAME");
    loadGame.setString("LOAD GAME");

    this->uiTexts->push_back(newGame);
    this->uiTexts->push_back(loadGame);
    this->background.setTexture(menuBackground);
}

void Game::initExit() {
    struct ExitText{
        sf::Text text;
        ExitText(const sf::Font& textFont){
            text.setFont(textFont);
            text.setCharacterSize(45);
            text.setFillColor(sf::Color::White);
        }
    };

    ExitText mainText(this->font);
    ExitText yes(this->font);
    ExitText no(this->font);

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color::Black);
    backgroundShape.setOutlineThickness(5.f);
    backgroundShape.setOutlineColor(sf::Color::Red);

    mainText.text.setPosition(200, 100);
    yes.text.setPosition(300, 350);
    no.text.setPosition(315, 450);

    backgroundShape.setPosition(mainText.text.getPosition());

    mainText.text.setString(" DO YOU WISH \n    TO SAVE?");
    yes.text.setString(" YES");
    no.text.setString(" NO");

    backgroundShape.setSize(mainText.text.getGlobalBounds().getSize() + sf::Vector2f (0,370));

    this->uiTexts->push_back(mainText.text);
    this->uiTexts->push_back(yes.text);
    this->uiTexts->push_back(no.text);

    this->uiShapes->push_back(backgroundShape);
}

void Game::initPowerUp() {
    struct PowerText{
        sf::Text text;
        PowerText(const sf::Font& textFont){
            text.setFont(textFont);
            text.setCharacterSize(45);
            text.setFillColor(sf::Color::White);
        }
    };

    sf::RectangleShape firstBack;
    sf::RectangleShape secondBack;
    firstBack.setFillColor(sf::Color::Black);
    secondBack.setFillColor(sf::Color::Black);

    std::string powerUps[4] = {"     +1\n Damage ",
                               "    +4  \n    HP   ",
                               "       +2\n Character \n     Speed",
                               "     +2\n   Bullet   \n   Speed  "};

    PowerText mainText(this->font);
    PowerText first(this->font);
    PowerText second(this->font);

    mainText.text.setFillColor(sf::Color::Black);

    mainText.text.setPosition(215, 100);
    first.text.setPosition(110, 400);
    second.text.setPosition(400, 400);

    firstBack.setPosition(first.text.getPosition());
    secondBack.setPosition(second.text.getPosition());

    mainText.text.setString("CHOOSE AN\n  UPGRADE");
    first.text.setString(powerUps[this->currentLevel * 7 % 4]);
    second.text.setString(powerUps[(this->currentLevel * 7 + 1) % 4]);

    firstBack.setSize(first.text.getGlobalBounds().getSize() + sf::Vector2f (0, 30));
    secondBack.setSize(second.text.getGlobalBounds().getSize() + sf::Vector2f (0, 30));

    this->uiTexts->push_back(mainText.text);
    this->uiTexts->push_back(first.text);
    this->uiTexts->push_back(second.text);

    this->uiShapes->push_back(firstBack);
    this->uiShapes->push_back(secondBack);
}

void Game::switchToMap() {
    // Casual procedure for going to the map game state
    int playerMapPos = this->levelToPos(this->currentLevel);
    float width = this->player->getCharacter().getGlobalBounds().width;
    float height = this->player->getCharacter().getGlobalBounds().height;
    sf::Vector2f adjust = sf::Vector2f {width/2, height};

    background.setTexture(worldBackground);
    this->resetBullets();

    // Resetting UI
    while (!this->uiTexts->empty())
        this->uiTexts->pop_back();
    while (!this->uiShapes->empty())
        this->uiShapes->pop_back();

    // Player modification
    this->player->cure();
    this->player->changeSpeed(2.f);
    this->player->changePos(this->nodesPos->at(playerMapPos) - adjust);
    this->powerUp = false;
    this->statsTree->value = 1;
}

int Game::levelToPos(int level) {
    // Returns the position of a level node in the graph
    switch (level){
        case 0:
            return 0;
        case 1:
            return 9;
        case 2:
            return 12;
        case 3:
            return 15;
        case 4:
            return 19;
        default:
            break;
    }

    // Default value
    return -1;
}

void Game::save() {
    // Saves game state into .txt files
    std::ofstream file;
    file.open("../../src/saves/saveStats.txt", std::ios::out);
    printTree(statsTree, file);
    file.close();

    // Save won levels into .txt files
    file.open("../../src/saves/saveWon.txt", std::ios::out);
    for (int i = 0; i < this->wonLevels.size(); i++)
        switch (this->wonLevels[i]){
            case false:
                file << 0 << ' ';
                break;
            case true:
                file << 1 << ' ';
                break;
        }
    file << this->currentLevel;
    file.close();
}

void Game::load() {
    deleteTree(statsTree);
    std::ifstream file;

    // Load stats tree
    file.open("../../src/saves/saveStats.txt", std::ios::in);
    this->statsTree = read(file);
    file.close();

    // Player constructor
    this->player = new Player(this->statsTree->lChild->value,
                              this->statsTree->rChild->value,
                              364, 364);

    // Load upgrades
    int healthUpgrades = this->statsTree->lChild->lChild->value;
    int charSpeedUpgrades = this->statsTree->lChild->rChild->value;
    int bulletSpeedUpgrades = this->statsTree->rChild->rChild->value;
    int damageUpgrades = this->statsTree->rChild->lChild->value;

    for (int i = 0; i < healthUpgrades; i++)
        this->player->upgrade(4);
    for (int i = 0; i < charSpeedUpgrades; i++)
        this->player->upgrade(0,0,0,2);
    for (int i = 0; i < bulletSpeedUpgrades; i++)
        this->player->upgrade(0,0,2);
    for (int i = 0; i < damageUpgrades; i++)
        this->player->upgrade(0,1);

    // Load won levels
    file.open("../../src/saves/saveWon.txt", std::ios::in);
    for (int i = 0; i < this->wonLevels.size(); i++){
        int read;
        file >> read;
        this->wonLevels[i] = read;
    }
    file >> this->currentLevel;
    file.close();
}

void Game::displayChars(std::string path, int scale) {
    // Creates choice text
    if (!choice1){
        sf::Text choiceText;
        choiceText.setFont(this->font);
        choiceText.setFillColor(sf::Color::White);
        choiceText.setCharacterSize(20);
        choiceText.setPosition(210, 100);
        choiceText.setString("CHOOSE YOUR CHARACTER");

        this->uiTexts->push_back(choiceText);
    }

    // Displays sprite elements to choose from
    int run = 0;

    for (const auto &entry : std::filesystem::directory_iterator(path)){
        sf::Texture mugshotTexture;
        sf::Sprite charMugshot;

        mugshotTexture.loadFromFile(entry.path().string());

        this->textures->push_back(mugshotTexture);
        charMugshot.setTexture(textures->at(run++));

        this->uiSprites->push_back(charMugshot);
    }

    for (int i = 0; i < this->uiSprites->size(); i++){
        this->uiSprites->at(i).setPosition(160.f + 240.f * i, 250.f);
        this->uiSprites->at(i).setScale(scale, scale);
    }
}

// Polling
void Game::pollEvents() {
    while (this->window->pollEvent(this->ev)){
        switch (this->ev.type) {
            case sf::Event::Closed:
                // Closes window
                this->window->close();

                break;
            case sf::Event::KeyPressed:
                if (this->ev.key.code == sf::Keyboard::Escape) {
                    // Goes into the exit window menu
                    if (!this->exit){
                        this->exit = true;
                        this->initExit();
                    }

                    // Goes out of the exit window
                    else{
                        while (!this->uiTexts->empty())
                            this->uiTexts->pop_back();
                        while (!this->uiShapes->empty())
                            this->uiShapes->pop_back();
                        this->exit = false;
                    }

                }
                if (this->ev.key.code == sf::Keyboard::Enter){
                    switch (this->statsTree->value){
                        case 1:
                            // Enters a level if not completed yet
                            if (!this->wonLevels[this->currentLevel]){
                                this->statsTree->value = 2;
                                this->background.setTexture(levelBackground);
                                this->player->changeSpeed(this->player->getCharSpeed());
                                this->updateCurrentEnemies();
                            }

                            break;
                    }
                }

                break;
            case sf::Event::MouseButtonReleased:
                switch (statsTree->value) {
                    case 2:
                        // If key is released, character should change animation immediately
                        if (this->ev.key.code == sf::Keyboard::A || this->ev.key.code == sf::Keyboard::Left ||
                            this->ev.key.code == sf::Keyboard::W || this->ev.key.code == sf::Keyboard::Up   ||
                            this->ev.key.code == sf::Keyboard::S || this->ev.key.code == sf::Keyboard::Down ||
                            this->ev.key.code == sf::Keyboard::D || this->ev.key.code == sf::Keyboard::Right)
                        {
                            this->player->resetAnimationTimer();
                        }
                        break;
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (ev.mouseButton.button == sf::Mouse::Left) {
                    switch (statsTree->value) {
                        case -1:
                            // Menu
                            switch (pollUiChoices()){
                                case 0:
                                    // If new game is chosen, goes to character creation menu
                                    this->statsTree->value = 0;
                                    while (!this->uiTexts->empty())
                                        this->uiTexts->pop_back();
                                    this->displayChars("../../src/sprites/characters/mugshots", 1);

                                    break;
                                case 1:
                                    // Choice to load game
                                    this->load();

                                    this->background.setTexture(levelBackground);
                                    this->switchToMap();
                                    this->initMapShapes();

                                    break;
                            }

                            break;
                        case 0:
                            // Character creation
                            this->pollCharacterChoice();

                            // Switches to map if both choices have been made
                            if (choice1 && choice2){
                                this->player = new Player(this->statsTree->lChild->value,
                                                          this->statsTree->rChild->value,
                                                          364, 364);
                                this->switchToMap();
                                this->initMapShapes();
                            }

                            break;
                        case 1:
                            // Map navigation
                            if (!exit)
                                this->pollMapChoice();

                            // Polling exit window
                            else {
                                switch (pollUiChoices()){
                                    case 1:
                                        // Save option
                                        this->statsTree->value = 1;
                                        this->save();

                                    case 2:
                                        // Don't save option
                                        this->statsTree->value = -1;
                                        this->reset();
                                        this->initMenu();

                                        break;
                                }
                            }

                            break;
                        case 2:
                            // Level

                            // Polling exit window
                            if (exit){
                                switch (pollUiChoices()){
                                    case 1:
                                        this->statsTree->value = 1;
                                        this->save();

                                    case 2:
                                        this->statsTree->value = -1;
                                        this->reset();
                                        this->initMenu();
                                        break;
                                }
                            }
                            else if (powerUp){
                                int choice = this->pollUiChoices() - 1;

                                // Checks if clicked on a choice and proceeds accordingly
                                if (choice >= 0) {
                                    switch ((this->currentLevel * 7 + choice) % 4) {
                                        case 0:
                                            this->statsTree->rChild->lChild->value++;
                                            this->player->upgrade(0, 1, 0, 0);
                                            break;
                                        case 1:
                                            this->statsTree->lChild->lChild->value++;
                                            this->player->upgrade(4, 0, 0, 0);
                                            break;
                                        case 2:
                                            this->statsTree->lChild->rChild->value++;
                                            this->player->upgrade(0, 0, 0, 2);
                                        case 3:
                                            this->statsTree->rChild->rChild->value++;
                                            this->player->upgrade(0, 0, 2, 0);
                                            break;
                                    }
                                    this->wonLevels[this->currentLevel] = true;
                                    this->switchToMap();
                                }
                            }
                            else
                                this->player->fireGun(this->playerBullets);

                            break;
                    }
                    break;
                }
        }
    }
}

int Game::pollUiChoices() {
    for (int i = 0; i < uiTexts->size(); i++){
        if (this->uiTexts->at(i).getGlobalBounds().contains(this->mousePosView)){
            while (!this->uiTexts->empty())
                this->uiTexts->pop_back();

            return i;
        }
    }

    // Default value
    return -1;
}

void Game::pollMapChoice() {
    for (int i = 0; i < mapShapes->size(); i++)
        if (mapShapes->at(i).getGlobalBounds().contains(this->mousePosView))
            this->walk(i);

}

void Game::pollCharacterChoice() {
    // Checks if a choice has been made
    int choice = 0;

    auto itr = this->uiSprites->begin();

    for (itr; itr < this->uiSprites->end(); itr++) {
        if ((*itr).getGlobalBounds().contains(this->mousePosView)) {
            if (!choice1) {
                this->choice1 = true;
                this->statsTree->lChild->value = choice;
                this->uiTexts->at(0).move(50.f, 0.f);
            }
            else {
                choice2 = true;
                this->statsTree->rChild->value = choice;
            }

            // Resets UI elements used for the next choice
            while (!this->textures->empty())
                this->textures->pop_back();
            while (!this->uiSprites->empty())
                this->uiSprites->pop_back();

            if (!this->choice2) {
                displayChars("../../src/sprites/gun", 10);
                this->uiTexts->at(0).setString("CHOOSE YOUR GUN");
            }
        }
        choice++;
    }
}

// Constructors and destructors
Game::Game() {
    this->initTree();
    this->initWindow();
    this->initGameVars();
    this->initEnemies();
    this->initTextures();
    this->initFont();
    this->initMenu();
    this->initNodesPos();
}

Game::~Game(){
    delete this->window;
    delete this->uiSprites;
    delete this->playerBullets;
    delete this->enemyBullets;
    delete this->textures;
    deleteTree(statsTree);
}

// Accessors
bool Game::running() const &{
    return this->window->isOpen();
}

// Functions
void Game::updateGame() {
    this->pollEvents();
    this->updateMousePos();

    switch (statsTree->value){
        case -1:
            // Checks if mouse is hovering over UI elements and updates outline
            this->updateOutline <std::vector<sf::Text>> (this->uiTexts);

            break;
        case 0:
            // Checks if mouse is hovering over UI elements and updates outline
            this->updateOutline <std::vector<sf::Sprite>> (this->uiSprites);

            break;
        case 1:
            // Update character animations
            this->player->updateAnimations();

            // Update UI shapes
            this->updateUIShapes();

            // Exit window
            if (exit)
                this->updateOutline <std::vector<sf::RectangleShape>>(this->uiShapes);

            break;
        case 2:
            // If exit screen is up, does not update
            if (!(this->exit || this->powerUp)){
                // Updates bullets (duh)
                this->updateBullets();

                // Removes enemies from vector if dead
                for (int i = 0; i < this->currentEnemies->size(); i++){
                    this->currentEnemies->at(i)->updateEnemy(this->player->getCharacter().getPosition(), this->enemyBullets);

                    if (this->currentEnemies->at(i)->getHP() <= 0) {
                        std::shared_ptr<Enemy> swapEnemy = this->currentEnemies->back();
                        this->currentEnemies->at(i) = swapEnemy;
                        this->currentEnemies->pop_back();
                    }
                }

                // Update Player
                this->player->update(this->window, mousePosView);

                // Check Player Death
                if (this->player->getHP() <= 0){
                    this->switchToMap();
                }

                if (this->currentEnemies->empty()) {
                    this->powerUp = true;
                    this->initPowerUp();
                }
            }
            else
                this->updateOutline <std::vector<sf::RectangleShape>>(this->uiShapes);

            break;
    }
}

void Game::renderGame() {
    this->window->clear();

    // Draw background
    this->window->draw(this->background);

    switch (this->statsTree->value) {
        case -1:
            // Render UI features
            this->renderVector <std::vector<sf::Text>> (this->uiTexts);

            break;
        case 0:
            // Render UI features
            this->renderVector <std::vector<sf::Sprite>> (this->uiSprites);
            this->renderVector <std::vector<sf::Text>> (this->uiTexts);
            this->window->draw(this->outline);

            break;
        case 1:
            // Draw UI elements;
            this->renderVector <std::vector<sf::CircleShape>> (this->mapShapes);

            // Draw player
            this->player->mapRender(this->window);

            // Exit window
            if (exit){
                this->renderVector <std::vector<sf::RectangleShape>>(this->uiShapes);
                this->renderVector <std::vector<sf::Text>>(this->uiTexts);
            }


            break;
        case 2:
            // Draw player
            this->player->render(this->window);

            //Render Enemies
            for (int i = 0; i < this->currentEnemies->size(); i++)
                this->currentEnemies->at(i)->render(this->window);


            // Render Bullets
            this->renderSprites(this->playerBullets);
            this->renderSprites(this->enemyBullets);

            // Exit window
            if (exit || powerUp){
                this->renderVector <std::vector<sf::RectangleShape>>(this->uiShapes);
                this->renderVector <std::vector<sf::Text>>(this->uiTexts);
                if (!exit)
                    this->window->draw(this->outline);
            }


            break;
    }

    this->window->display();
}
