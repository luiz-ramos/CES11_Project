//
// Created by João Pedro Barbosa on 14/11/2023.
//

#include "Game.hpp"
#define PI 3.14159265

// Initializers
void Game::initGameVars() {
    this->uiTexts = new std::vector<sf::Text>;
    this->uiSprites = new std::vector<sf::Sprite>;
    this->playerBullets = new std::vector<sf::Sprite>;
    this->enemyBullets = new std::vector<sf::Sprite>;
    this->textures = new std::vector<sf::Texture>;
    this->nodesPos = new std::vector<sf::Vector2f>;
    this->uiShapes = new std::vector<sf::CircleShape>;
    this->gameMap = new GameMap();
    this->enemies = new std::vector<Enemy>;
    this->currentEnemies = new std::vector<Enemy>;

    this->bulletSpeed = 5.f;
    this->currentLevel = 0;
    this->exit = false;

    this->textures->reserve(5);
    for (int i = 0; i < 5; i++)
        this->wonLevels.push_back(false);
    this->enemies->reserve(5);
    this->currentEnemies->reserve(5);
}

void Game::initEnemies() {
    // Level 1
    this->enemies->emplace_back(0,2,0,100,100);

    // Level 2
    this->enemies->emplace_back(0,2,0,100,600);

    // Level 3
    this->enemies->emplace_back(0,2,0,500,600);

    // Level 4
    this->enemies->emplace_back(0,2,0,500,100);

    // Level 5
    this->enemies->emplace_back(1,2,0,300,100);
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
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setPosition(210, 100);
    text.setCharacterSize(20);
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

    // Since choices were not made yet
    this->choice1 = false;
    this->choice2 = false;
}

void Game::initNodesPos() {
    std::vector<sf::Vector2f> v{{612, 567}, {431, 567}, {431, 541}, {335, 541}, {335, 420}, {298, 420}, {298, 442}, {400, 420}, {400, 302}, {494, 302}, {400, 229}, {340, 229}, {340, 160}, {300, 229}, {212, 229}, {212, 94},{172, 229}, {172, 320},{128, 320},{128, 442}};
    for (int i = 0; i < v.size(); i++)
        this->nodesPos->push_back(v[i]);
}

void Game::initShapes() {
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
    this->uiShapes->emplace_back(uiCircle(this->nodesPos->at(0) - adjust).circle);

    // Level 2
    this->uiShapes->emplace_back(uiCircle(this->nodesPos->at(9) - adjust).circle);

    // Level 3
    this->uiShapes->emplace_back(uiCircle(this->nodesPos->at(12) - adjust).circle);

    // Level 4
    this->uiShapes->emplace_back(uiCircle(this->nodesPos->at(15) - adjust).circle);

    // Level 5
    this->uiShapes->emplace_back(uiCircle(this->nodesPos->at(19) - adjust).circle);
}

// Update functions
void Game::reset() {
    this->exit = false;

    this->resetBullets();
    while (!this->uiShapes->empty())
        this->uiShapes->pop_back();

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
    bool check = false;
    auto itr = objectsVector->begin();

    for (itr; itr < objectsVector->end(); itr++)
        if ((*itr).getGlobalBounds().contains(this->mousePosView)){
            outline.setOutlineColor(sf::Color::White);
            outline.setPosition((*itr).getGlobalBounds().getPosition());
            outline.setSize((*itr).getGlobalBounds().getSize());
            check = true;
        }

    if (!check)
        outline.setOutlineColor(sf::Color::Transparent);
}

void Game::updateUIShapes() {
    // Highlights a shape if hovered over
    auto itr = this->uiShapes->begin();

    for (itr; itr < this->uiShapes->end(); itr++)
        if ((*itr).getGlobalBounds().contains(this->mousePosView)){
            (*itr).setFillColor(sf::Color::Yellow);
            return;
        }

    for (int i = 0; i < this->uiShapes->size(); i++)
        switch (this->wonLevels[i]){
            case false:
                this->uiShapes->at(i).setFillColor(sf::Color::Transparent);

                break;
            case true:
                this->uiShapes->at(i).setFillColor(sf::Color::Green);

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

    for (int i = 0; i < this->currentLevel + 1; i++)
        this->currentEnemies->push_back(this->enemies->at(i));
}

void Game::updateMousePos() {
    this->mousePos = sf::Mouse::getPosition(*(this->window));
    this->mousePosView = this->window->mapPixelToCoords(this->mousePos);
}

void Game::updateBullets() {
    // Updates bullet positions and checks collisions


    auto itr = this->playerBullets->begin();

    for (itr; itr < playerBullets->end(); itr++){
        sf::Sprite * bullet = &*itr;
        float angle = (*bullet).getRotation() * PI/180;

        auto itr2 = this->currentEnemies->begin();

        // Checks for collision with enemies
        for (itr2; itr2 < this->currentEnemies->end(); itr2++)
            if ((*itr).getGlobalBounds().intersects((*itr2).getCharacter().getGlobalBounds())){
                this->playerBullets->erase(itr);
                (*itr2).receiveDamage(1);
            }

        float xUp = this->window->getSize().x;
        float yUp = this->window->getSize().y;

        // Checks if bullet is out-of-bounds
        if ((*itr).getPosition().x > xUp || (*itr).getPosition().x < 0.f ||
            (*itr).getPosition().y > yUp || (*itr).getPosition().y < 0.f)
            this->playerBullets->erase(itr);


        (*bullet).move(bulletSpeed * cos(angle), bulletSpeed * sin(angle));
    }

    itr = this->enemyBullets->begin();

    for (itr; itr < enemyBullets -> end(); itr++){
        sf::Sprite * bullet = &*itr;
        float angle = (*bullet).getRotation() * PI/180;

        // Checks collision with player
        if ((*itr).getGlobalBounds().intersects(this->player->getCharacter().getGlobalBounds())){
            this->enemyBullets->erase(itr);
            this->player->receiveDamage(1);
        }

        float xUp = this->window->getSize().x;
        float yUp = this->window->getSize().y;

        // Checks if bullet is out of bounds
        if ((*itr).getPosition().x > xUp || (*itr).getPosition().x < 0.f ||
            (*itr).getPosition().y > yUp || (*itr).getPosition().y < 0.f)
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
}

void Game::initExit() {
    sf::Text yes;
    sf::Text no;
    yes.setFont(font);
    no.setFont(font);
    yes.setFillColor(sf::Color::Black);
    no.setFillColor(sf::Color::Black);
    yes.setPosition(300, 350);
    no.setPosition(315, 450);
    yes.setString("YES");
    no.setString("NO");
    yes.setCharacterSize(45);
    no.setCharacterSize(45);
    this->text.setString("DO YOU WISH \n   TO SAVE?");
    this->text.setPosition(200, 100);
    this->text.setCharacterSize(45);
    this->text.setFillColor(sf::Color::Black);

    this->uiTexts->push_back(yes);
    this->uiTexts->push_back(no);
}

void Game::switchToMap() {
    // Casual procedure for going to the map game state
    int playerMapPos = this->levelToPos(this->currentLevel);
    float width = this->player->getCharacter().getGlobalBounds().width;
    float height = this->player->getCharacter().getGlobalBounds().height;
    sf::Vector2f adjust = sf::Vector2f {width/2, height};

    background.setTexture(worldBackground);
    this->resetBullets();

    // Player modification
    this->player->cure();
    this->player->changeSpeed(2.f);
    this->player->changePos(this->nodesPos->at(playerMapPos) - adjust);
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

void Game::displayChars(std::string path, int scale) {
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
        this->window->draw(this->uiSprites->at(i));
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
                    else
                        this->exit = false;
                }
                if (this->ev.key.code == sf::Keyboard::Enter){
                    switch (this->statsTree->value){
                        case 1:
                            // Enters a level if not completed yet
                            if (!this->wonLevels[this->currentLevel]){
                                this->statsTree->value = 2;
                                this->background.setTexture(levelBackground);
                                this->player->changeSpeed(3.f);
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
                                    this->displayChars("../../src/sprites/characters/mugshots", 1);
                                    this->text.setString("CHOOSE YOUR CHARACTER");

                                    break;
                                case 1:
                                    // Loads game from .txt file
                                    deleteTree(statsTree);
                                    std::ifstream file;

                                    // Load stats tree
                                    file.open("../../src/saves/saveStats.txt", std::ios::in);
                                    this->statsTree = read(file);
                                    file.close();

                                    // Load won levels
                                    file.open("../../src/saves/saveWon.txt", std::ios::in);
                                    for (int i = 0; i < this->wonLevels.size(); i++){
                                        int read;
                                        file >> read;
                                        this->wonLevels[i] = read;
                                    }
                                    file >> this->currentLevel;
                                    file.close();

                                    // Player constructor and switches to map
                                    this->player = new Player(this->statsTree->lChild->value,
                                                              this->statsTree->rChild->value,
                                                              364, 364);
                                    this->background.setTexture(levelBackground);
                                    this->switchToMap();
                                    this->initShapes();

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
                                this->initShapes();
                            }

                            break;
                        case 1:
                            // Map navigation
                            if (!exit)
                                this->pollMapChoice();

                            // Polling exit window
                            else {
                                switch (pollUiChoices()){
                                    case 0:
                                        // Save option
                                        this->statsTree->value = 1;
                                        this->save();

                                    case 1:
                                        // Don't save option
                                        this->statsTree->value = -1;
                                        this->initMenu();
                                        this->reset();

                                        break;
                                }
                            }

                            break;
                        case 2:
                            // Level
                            if (!exit)
                                this->player->fireGun(this->playerBullets);

                            // Polling exit window
                            else {
                                switch (pollUiChoices()){
                                    case 0:
                                        this->statsTree->value = 1;
                                        this->save();
                                        this->statsTree->value = -1;
                                        this->initMenu();
                                        this->reset();
                                    case 1:
                                        this->statsTree->value = -1;
                                        this->initMenu();
                                        this->reset();
                                        break;
                                }
                            }

                            break;
                    }
                    break;
                }
        }
    }
}

int Game::pollUiChoices() {
    for (int i = 0; i < uiTexts->size(); i++){
        if (uiTexts->at(i).getGlobalBounds().contains(this->mousePosView)){
            while (!this->uiTexts->empty())
                this->uiTexts->pop_back();

            return i;
        }
    }

    // Default value
    return -1;
}

void Game::pollMapChoice() {
    for (int i = 0; i < uiShapes->size(); i++)
        if (uiShapes->at(i).getGlobalBounds().contains(this->mousePosView))
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
                this->statsTree->lChild = new Tree{choice, nullptr};
                this->text.move(50.f, 0.f);
            }
            else {
                choice2 = true;
                statsTree->rChild = new Tree{choice, nullptr};
            }

            // Resets UI elements used for the next choice
            while (!this->textures->empty())
                this->textures->pop_back();
            while (!this->uiSprites->empty())
                this->uiSprites->pop_back();

            if (!this->choice2) {
                displayChars("../../src/sprites/gun", 10);
                this->text.setString("CHOOSE YOUR GUN");
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

            break;
        case 2:
            // If exit screen is up, does not update
            if (!this->exit){
                // Updates bullets (duh)
                this->updateBullets();

                // Removes enemies from vector if dead
                for (int i = 0; i < this->currentEnemies->size(); i++){
                    this->currentEnemies->at(i).updateEnemy(this->player->getCharacter().getPosition(), enemyBullets);

                    if (this->currentEnemies->at(i).getHP() <= 0) {
                        Enemy swapEnemy = Enemy(this->currentEnemies->back().getEnemyLevel(),
                                                this->currentEnemies->back().getCharacterId(),
                                                this->currentEnemies->back().getGunId(),
                                                this->currentEnemies->back().getCharacter().getPosition().x,
                                                this->currentEnemies->back().getCharacter().getPosition().y);

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

                // Check if level is completed
                if (this->currentEnemies->empty()) {
                    this->switchToMap();
                    this->wonLevels[this->currentLevel] = true;
                }
            }
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
            this->window->draw(this->outline);
            this->window->draw(this->text);

            break;
        case 1:
            // Draw UI elements;
            this->renderVector <std::vector<sf::CircleShape>> (this->uiShapes);

            // Draw player
            this->player->mapRender(this->window);

            // Exit window
            if (exit){
                this->window->draw(text);
                this->renderVector <std::vector<sf::Text>>(this->uiTexts);
            }

            break;
        case 2:
            // Draw player
            this->player->render(this->window);

            //Render Enemies
            for (int i = 0; i < this->currentEnemies->size(); i++)
                this->currentEnemies->at(i).render(this->window);

            // Render Bullets
            this->renderVector(this->playerBullets);
            this->renderVector(this->enemyBullets);

            // Exit window
            if (exit){
                this->window->draw(this->text);
                this->renderVector <std::vector<sf::Text>>(this->uiTexts);
            }

            break;
    }

    this->window->display();
}
