//
// Created by João Pedro Barbosa on 14/11/2023.
//

#include "Game.hpp"
#define PI 3.14159265

// Private functions
void Game::initGameVars() {
    this->window = nullptr;
    this->playerBullets = new std::vector<sf::Sprite>;
    this->enemyBullets = new std::vector<sf::Sprite>;
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
}

// Constructors and destructors

Game::Game() : Player(1,0,364,364){
    this->initGameVars();
    this->initWindow();
    this->initTextures();

    firstEnemy = new Enemy(0, 0, 1, 600, 600);
}

Game::~Game(){
    delete this->window;
    delete this->playerBullets;
    delete this->enemyBullets;
}

// Accessors

bool Game::running() const &{
    return this->window->isOpen();
}

// Functions

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
                if (ev.mouseButton.button == sf::Mouse::Left)
                    this->fireGun(playerBullets);
                break;
        }
    }
}

bool Game::bulletCheckCollisions(sf::Sprite * bullet) {
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

void Game::updateBullets() {
    auto itr = this->playerBullets->begin();

    for (itr; itr < playerBullets->end(); itr++){
        sf::Sprite * bullet = &*itr;
        float angle = (*bullet).getRotation() * PI/180;
        if ((*itr).getGlobalBounds().intersects(this->firstEnemy->getCharSprite().getGlobalBounds()))
            playerBullets->erase(itr);

//        if (bulletCheckCollisions(bullet))
//            bullets->erase(itr);

        (*bullet).move(bulletSpeed * cos(angle), bulletSpeed * sin(angle));
    }

    itr = this->enemyBullets->begin();

    for (itr; itr < enemyBullets -> end(); itr++){
        sf::Sprite * bullet = &*itr;
        float angle = (*bullet).getRotation() * PI/180;


//        if (bulletCheckCollisions(bullet))
//            bullets->erase(itr);

        (*bullet).move(bulletSpeed * cos(angle), bulletSpeed * sin(angle));
    }
}

void Game::updateGame() {
    this->pollEvents();

    this->updateBullets();

    firstEnemy->updateGun(this->character.getPosition());
    firstEnemy->shootCycle(enemyBullets);

    this->update(this->window);
}

void Game::renderVector(std::vector<sf::Sprite> * spriteVector) {
    auto itr = spriteVector->begin();

    for (itr; itr < spriteVector->end(); itr++)
        this->window->draw(*itr);
}

void Game::renderGame() {
    this->window->clear();

    // render objects
    this->window->draw(background);

    // Render Characters
    this->render(this->window);
    firstEnemy->render(this->window);

    // Render Bullets
    this->renderVector(playerBullets);
    this->renderVector(enemyBullets);

    this->window->display();
}
