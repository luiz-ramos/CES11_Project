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
    void readGraph(std::string);
public:
    // Constructors and destructors
    Graph(std::size_t);

    // Accessors
    size_t getSize() const&;

    // Functions
    std::vector<int> dijkstra(int, int);
};

class GameMap : public Graph{
public:
    GameMap();
};


#endif //UNTITLED2_GRAPH_HPP