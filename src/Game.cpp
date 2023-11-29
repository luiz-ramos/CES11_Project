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

    textures->reserve(5);
    this->enemies->reserve(5);
    this->currentEnemies->reserve(5);
}

void Game::initEnemies() {
    this->enemies->emplace_back(0, 0, 0, 100, 100);
    this->enemies->emplace_back(0,0,0,100,600);
    this->enemies->emplace_back(0,0,0,500,600);
    this->enemies->emplace_back(0,0,0,500,100);
    this->enemies->emplace_back(1,0,0,100,100);
}

void Game::initWindow() {
    this->videoMode.height = 728;
    this->videoMode.width = 728;

    this->window = new sf::RenderWindow(this->videoMode, "AirportIncursion", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(144);
}

void Game::initTextures() {
    levelBackground.loadFromFile("../../src/sprites/mapaResenha.jpg");
    worldBackground.loadFromFile("../../src/sprites/mapa.jpg");
    background.setTexture(levelBackground);

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

    while (!this->playerBullets->empty())
        this->playerBullets->pop_back();
    while (!this->enemyBullets->empty())
        this->enemyBullets->pop_back();
    while (!this->uiShapes->empty())
        this->uiShapes->pop_back();

    choice1 = false;
    choice2 = false;
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

void Game::walk(int targetLevel) {
    int mapPos = this->levelToPos(this->currentLevel);
    int targetPos = this->levelToPos(targetLevel);

    std::vector<int> path = gameMap->dijkstra(mapPos+1, targetPos+1);

    float width = this->player->getCharacter().getGlobalBounds().width;
    float height = this->player->getCharacter().getGlobalBounds().height;
    sf::Vector2f adjust = sf::Vector2f {width/2, height};

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
            this->player->render(this->window);

            this->window->display();
        };
        this->player->changePos(this->nodesPos->at(mapPos) - adjust);
    }

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
    auto itr = this->playerBullets->begin();

    for (itr; itr < playerBullets->end(); itr++){
        sf::Sprite * bullet = &*itr;
        float angle = (*bullet).getRotation() * PI/180;

        auto itr2 = this->currentEnemies->begin();
        for (itr2; itr2 < this->currentEnemies->end(); itr2++)
            if ((*itr).getGlobalBounds().intersects((*itr2).getCharacter().getGlobalBounds())){
                this->playerBullets->erase(itr);
                (*itr2).updateStats(-1,0);
            }

        float xUp = this->window->getSize().x;
        float yUp = this->window->getSize().y;

        if ((*itr).getPosition().x > xUp || (*itr).getPosition().x < 0.f ||
            (*itr).getPosition().y > yUp || (*itr).getPosition().y < 0.f)
            this->playerBullets->erase(itr);


        (*bullet).move(bulletSpeed * cos(angle), bulletSpeed * sin(angle));
    }

    itr = this->enemyBullets->begin();

    for (itr; itr < enemyBullets -> end(); itr++){
        sf::Sprite * bullet = &*itr;
        float angle = (*bullet).getRotation() * PI/180;

        if ((*itr).getGlobalBounds().intersects(this->player->getCharacter().getGlobalBounds())){
            this->enemyBullets->erase(itr);
            this->player->updateStats(-1,0);
        }

        float xUp = this->window->getSize().x;
        float yUp = this->window->getSize().y;

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
    float width = this->player->getCharacter().getGlobalBounds().width;
    float height = this->player->getCharacter().getGlobalBounds().height;
    sf::Vector2f adjust = sf::Vector2f {width/2, height};
    background.setTexture(worldBackground);

    switch (this->currentLevel){
        case 0:
            this->player->changePos(this->nodesPos->at(0) - adjust);
            break;
        case 1:
            this->player->changePos(this->nodesPos->at(9) - adjust);
            break;
        case 2:
            this->player->changePos(this->nodesPos->at(12) - adjust);
            break;
        case 3:
            this->player->changePos(this->nodesPos->at(15) - adjust);
            break;
        case 4:
            this->player->changePos(this->nodesPos->at(19) - adjust);
            break;
    }

    this->player->changeSpeed(2.f);
    this->statsTree->value = 1;
}

int Game::levelToPos(int level) {
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
            std::cout << "ERROR ";
            std::cout << this->uiShapes->size() << ' ' << this->currentLevel;
            break;
    }
}

void Game::save() {
    std::ofstream file;
    file.open("../../src/saves/save.txt", std::ios::out);
    printTree(statsTree, file);
    file.close();
}

void Game::displayChars(std::string path, int scale) {
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
        this->uiSprites->at(i).setPosition(160 + 240 * i, 250);
        this->uiSprites->at(i).setScale(scale, scale);
        this->window->draw(this->uiSprites->at(i));
    }
}

// Polling
void Game::pollEvents() {
    while (this->window->pollEvent(this->ev)){
        switch (this->ev.type) {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyPressed:
                if (this->ev.key.code == sf::Keyboard::Escape) {
                    exit = true;
                    initExit();
                }
                if (this->ev.key.code == sf::Keyboard::Enter){
                    switch (this->statsTree->value){
                        case 1:
                            this->statsTree->value = 2;
                            this->background.setTexture(levelBackground);
                            this->player->changeSpeed(3.f);
                            this->updateCurrentEnemies();
                            break;
                    }
                }
                break;
            case sf::Event::MouseButtonReleased:
                switch (statsTree->value) {
                    case 2:
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
                                    this->statsTree->value = 0;
                                    this->displayChars("../../src/sprites/characters/mugshots", 1);
                                    this->text.setString("CHOOSE YOUR CHARACTER");
                                    break;
                                case 1:
                                    deleteTree(statsTree);
                                    std::ifstream file;
                                    file.open("../../src/saves/save.txt", std::ios::in);
                                    this->statsTree = read(file);
                                    file.close();

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
                            this->pollMapChoice();

                            break;
                        case 2:
                            // Level
                            if (!exit)
                                this->player->fireGun(playerBullets);
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

    return 5;
}

void Game::pollMapChoice() {
    for (int i = 0; i < uiShapes->size(); i++){
        if (uiShapes->at(i).getGlobalBounds().contains(this->mousePosView)){
            this->walk(i);
        }
    }
}

void Game::pollCharacterChoice() {
    int choice = 0;
    auto itr = uiSprites->begin();

    for (itr; itr < uiSprites->end(); itr++) {
        if ((*itr).getGlobalBounds().contains(this->mousePosView)) {
            if (!choice1) {
                choice1 = true;
                statsTree->lChild = new Tree{choice, nullptr};
                text.move(50.f, 0.f);
            }
            else {
                choice2 = true;
                statsTree->rChild = new Tree{choice, nullptr};
            }

            while (!textures->empty())
                textures->pop_back();
            while (!uiSprites->empty())
                uiSprites->pop_back();

            if (!choice2) {
                displayChars("../../src/sprites/gun", 10);
                text.setString("CHOOSE YOUR GUN");
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
            this->updateOutline <std::vector<sf::Text>> (this->uiTexts);

            break;
        case 0:
            this->updateOutline <std::vector<sf::Sprite>> (this->uiSprites);

            break;
        case 1:
            this->player->updateAnimations();
            this->player->updateGun(this->mousePosView);

            break;
        case 2:
            if (!exit){
                this->updateBullets();

                for (int i = 0; i < this->currentEnemies->size(); i++){
                    this->currentEnemies->at(i).updateEnemy(this->player->getCharacter().getPosition(), enemyBullets);
                    if (this->currentEnemies->at(i).getHealth() <= 0) {
                        Enemy swapEnemy = Enemy(this->currentEnemies->back().getEnemyLevel(),
                                                this->currentEnemies->back().getCharacterId(),
                                                this->currentEnemies->back().getGunId(),
                                                this->currentEnemies->back().getCharacter().getPosition().x,
                                                this->currentEnemies->back().getCharacter().getPosition().y);

                        this->currentEnemies->at(i) = swapEnemy;
                        this->currentEnemies->pop_back();
                    }
                }

                this->player->update(this->window, mousePosView);

                if (this->currentEnemies->empty())
                    this->switchToMap();
            }
            break;
    }
}

void Game::renderGame() {
    this->window->clear();

    switch (statsTree->value) {
        case -1:
            // Render UI features
            this->renderVector <std::vector<sf::Text>> (uiTexts);
            break;
        case 0:
            // Render UI features
            this->renderVector <std::vector<sf::Sprite>> (uiSprites);
            this->window->draw(outline);
            this->window->draw(text);

            break;
        case 1:
            // Draw background
            this->window->draw(background);

            // Draw UI elements;
            this->renderVector <std::vector<sf::CircleShape>> (uiShapes);

            // Draw player
            this->player->render(this->window);

            break;
        case 2:
            // Draw background
            this->window->draw(background);

            // Draw player
            this->player->render(this->window);

            //Render Enemies
            for (int i = 0; i < this->currentEnemies->size(); i++)
                this->currentEnemies->at(i).render(this->window);

            // Render Bullets
            this->renderVector(playerBullets);
            this->renderVector(enemyBullets);

            if (exit){
                this->window->draw(text);
                this->renderVector <std::vector<sf::Text>>(uiTexts);
            }

            break;
    }

    this->window->display();
}
