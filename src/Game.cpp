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

    this->bulletSpeed = 5.f;
    this->exit = false;

    textures->reserve(5);
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
    this->characterChoice = false;
    this->gunChoice = false;
}

// Update functions
void Game::reset() {
    this->exit = false;
    delete this->firstEnemy;

    this->firstEnemy = new Enemy{0,1,1, 600, 600};

    while (!this->playerBullets->empty())
        this->playerBullets->pop_back();
    while (!this->enemyBullets->empty())
        this->enemyBullets->pop_back();

    characterChoice = false;
    gunChoice = false;
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

void Game::updateMousePos() {
    this->mousePos = sf::Mouse::getPosition(*(this->window));
    this->mousePosView = this->window->mapPixelToCoords(this->mousePos);
}

void Game::updateBullets() {
    auto itr = this->playerBullets->begin();

    for (itr; itr < playerBullets->end(); itr++){
        sf::Sprite * bullet = &*itr;
        float angle = (*bullet).getRotation() * PI/180;

        if ((*itr).getGlobalBounds().intersects(this->firstEnemy->getCharSprite().getGlobalBounds())){
            playerBullets->erase(itr);
            this->firstEnemy->updateStats(-1, 0);
        }

        (*bullet).move(bulletSpeed * cos(angle), bulletSpeed * sin(angle));
    }

    itr = this->enemyBullets->begin();

    for (itr; itr < enemyBullets -> end(); itr++){
        sf::Sprite * bullet = &*itr;
        float angle = (*bullet).getRotation() * PI/180;

        (*bullet).move(bulletSpeed * cos(angle), bulletSpeed * sin(angle));
    }
}

bool Game::updateBulletCollisions(sf::Sprite * bullet) {
    sf::FloatRect playerBounds = bullet->getGlobalBounds();

    if (playerBounds.left <= 0.f)
        return true;

    else if (playerBounds.left + playerBounds.width >= window->getSize().x)
        return true;

    if (playerBounds.top <= 0.f)
        return true;

    else if (playerBounds.top + playerBounds.height >= window->getSize().y)
        return true;

    return false;
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
                                    break;
                            }
                            break;
                        case 0:
                            // Character creation
                            this->pollCharacterChoice();
                            if (characterChoice && gunChoice){
                                this->statsTree->value = 2;
                                this->player = new Player(this->statsTree->lChild->value,
                                                          this->statsTree->rChild->value,
                                                          364, 364);
                                this->background.setTexture(levelBackground);
                            }

                            break;
                        case 1:
                            // Map navigation

                            break;
                        case 2:
                            // Level
                            if (!exit)
                                this->player->fireGun(playerBullets);
                            else {
                                switch (pollUiChoices()){
                                    case 0:
                                        this->save();
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

void Game::pollCharacterChoice() {
    int choice = 0;
    auto itr = uiSprites->begin();

    for (itr; itr < uiSprites->end(); itr++) {
        if ((*itr).getGlobalBounds().contains(this->mousePosView)) {
            if (!characterChoice) {
                characterChoice = true;
                statsTree->lChild = new Tree{choice, nullptr};
                text.move(50.f, 0.f);
            }
            else {
                gunChoice = true;
                statsTree->rChild = new Tree{choice, nullptr};
            }

            while (!textures->empty())
                textures->pop_back();
            while (!uiSprites->empty())
                uiSprites->pop_back();

            if (!gunChoice) {
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
    this->initTextures();
    this->initFont();
    this->initMenu();

    firstEnemy = new Enemy(0, 1, 1, 100, 600);
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

            break;
        case 2:
            if (!exit){
                this->updateBullets();

                this->firstEnemy->updateEnemy(this->player->getCharacter().getPosition(), enemyBullets);
                this->player->update(this->window, mousePosView);
            }
            break;
    }
}

void Game::renderGame() {
    this->window->clear();

    switch (statsTree->value) {
        case -1:
            // Render UI features
            this->renderVector <std::vector<sf::Text>>(uiTexts);
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

            // Draw player
            this->player->render(this->window);

            break;
        case 2:
            // Draw background
            this->window->draw(background);

            // Draw player
            this->player->render(this->window);

            // Render Enemies
            if (firstEnemy->getHealth() > 0)
                firstEnemy->render(this->window);

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
