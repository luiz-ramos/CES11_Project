//
// Created by João Pedro Barbosa on 22/11/2023.
//
#ifndef UNTITLED2_GRAPH_HPP
#define UNTITLED2_GRAPH_HPP

#include "Enemy.h"

class Graph {
protected:
    std::size_t size;
    std::vector<std::vector<int>> matrix;

    // Protected functions
    std::vector<int> getPath(std::vector<int> const&, int);
    void insertEdge(sf::Vector2f, sf::Vector2f, int, int);
public:
    // Constructors and destructors
    Graph(std::size_t);

    // Functions
    std::vector<int> dijkstra(int, int);
};

class GameMap : public Graph{
public:
    GameMap();
};

struct Level{
    bool won;
    std::vector<Enemy> * enemies;
    Level(){
        this->won = false;
        this->enemies = new std::vector<Enemy>;
        this->enemies->reserve(5);
    };
    ~Level(){
        delete enemies;
    }
};

class GameLevels{
private:
    std::vector<Level> * levelsVector;

public:
    // Constructor
    GameLevels();
    ~GameLevels();

    // Accessor
    std::vector<Level> * getLevels() const&;
};

#endif //UNTITLED2_GRAPH_HPP