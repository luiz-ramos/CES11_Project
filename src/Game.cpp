//
// Created by João Pedro Barbosa on 14/11/2023.
//

#include "Game.hpp"
#define PI 3.14159265

// Initializers
void Game::initGameVars() {
    this->uiSprites = new std::vector<sf::Sprite>;
    this->playerBullets = new std::vector<sf::Sprite>;
    this->enemyBullets = new std::vector<sf::Sprite>;
    this->mugshots = new std::vector<sf::Texture>;

    mugshots->reserve(5);
}

void Game::initWindow() {
    this->videoMode.height = 728;
    this->videoMode.width = 728;

    this->window = new sf::RenderWindow(this->videoMode, "AirportIncursion", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(144);
}

void Game::initTextures() {
    mainBackground.loadFromFile("../../src/sprites/mapaResenha.jpg");
    background.setTexture(mainBackground);

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
    this->statsTree = make(0,
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
void Game::updateOutline() {
    bool check = false;
    auto itr = uiSprites->begin();

    for (itr; itr < uiSprites->end(); itr++)
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
void Game::renderVector(std::vector<sf::Sprite> * spriteVector) {
    auto itr = spriteVector->begin();

    for (itr; itr < spriteVector->end(); itr++)
        this->window->draw(*itr);
}

// UI related
void Game::setCharacter() {
    displayChars("../../src/sprites/characters/mugshots", 1);
    while (!characterChoice || !gunChoice){
        this->window->clear();
        this->pollEvents();

        this->updateMousePos();

        if (!characterChoice)
            text.setString("CHOOSE YOUR CHARACTER");
        else
            text.setString("CHOOSE YOUR GUN");

        this->updateOutline();

        this->renderVector(uiSprites);
        this->window->draw(outline);
        this->window->draw(text);

        this->window->display();
    }

    statsTree->value = 2;
}

void Game::displayChars(std::string path, int scale) {
    int run = 0;

    for (const auto &entry : std::filesystem::directory_iterator(path)){
        sf::Texture mugshotTexture;
        sf::Sprite mugshot;

        mugshotTexture.loadFromFile(entry.path().string());

        this->mugshots->push_back(mugshotTexture);
        mugshot.setTexture(mugshots->at(run++));

        this->uiSprites->push_back(mugshot);
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
                if (this->ev.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;
            case sf::Event::MouseButtonPressed:
                if (ev.mouseButton.button == sf::Mouse::Left) {
                    switch (statsTree->value) {
                        case 0:
                            this->pollCharacterChoice();
                            break;
                        case 1:
                            break;
                        case 2:
                            this->player->fireGun(playerBullets);
                            break;
                    }
                    break;
                }
        }
    }
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
            } else {
                gunChoice = true;
                statsTree->rChild = new Tree{choice, nullptr};
            }

            while (!mugshots->empty())
                mugshots->pop_back();
            while (!uiSprites->empty())
                uiSprites->pop_back();

            if (!gunChoice)
                displayChars("../../src/sprites/gun", 10);
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
    this->setCharacter();

    firstEnemy = new Enemy(0, 1, 1, 600, 600);
    player = new Player(statsTree->lChild->value,
                        statsTree->rChild->value
                        ,364,364);
}

Game::~Game(){
    delete this->window;
    delete this->uiSprites;
    delete this->playerBullets;
    delete this->enemyBullets;
    delete this->mugshots;
}

// Accessors
bool Game::running() const &{
    return this->window->isOpen();
}

// Functions
void Game::updateGame() {
    switch (statsTree->value){
        case 1:
            break;
        case 2:
            this->pollEvents();

            this->updateBullets();
            this->updateMousePos();

            firstEnemy->updateGun(this->player->getCharacter().getPosition());
            firstEnemy->shootCycle(enemyBullets);

            this->player->update(this->window, mousePosView);
            break;
    }
}

void Game::renderGame() {
    this->window->clear();

    this->window->draw(background);
    this->player->render(this->window);

    switch (statsTree->value) {
        case 1:
            break;
        case 2:
            // Render Enemies
            if (firstEnemy->getHealth() > 0)
                firstEnemy->render(this->window);

            // Render Bullets
            this->renderVector(playerBullets);
            this->renderVector(enemyBullets);

            break;
    }

    this->window->display();
}
