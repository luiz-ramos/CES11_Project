//
// Created by João Pedro Barbosa on 14/11/2023.
//

#include "Game.hpp"

// Private functions
void Game::initVars() {
    this->window = nullptr;
}

void Game::initWindow() {
    this->videoMode.height = 600;
    this->videoMode.width = 800;

    this->window = new sf::RenderWindow(this->videoMode, "AirportIncursion", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(144);
}

void Game::initBackground() {
    mainBackground.loadFromFile("../../src/sprites/mainBackGround.jpg");
    background.setTexture(mainBackground);
}

// Constructors and destructors

Game::Game() {
    this->initVars();
    this->initWindow();
    this->initBackground();
}

Game::~Game(){
    delete this->window;
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
        }
    }
}

void Game::update() {
    this->pollEvents();

    this->player.update(this->window);
}

void Game::render() {
    this->window->clear();

    // render objects
    this->player.render(this->window);
//    this->window->draw(background);

    this->window->display();
}
