//
// Created by João Pedro Barbosa on 22/11/2023.
//

#include "Graph.hpp"
#define INF 10000

// Graph

// Constructor
Graph::Graph(std::size_t tam) {
    matrix = std::vector<std::vector<int>>(tam, std::vector<int>(tam, 0));
}

// Protected Functions
void Graph::insertEdge (sf::Vector2f v1, sf::Vector2f v2, int n1, int n2) {
    matrix[n1][n2] = fabsf(v2.x - v1.x) + fabsf(v2.y - v1.y);
    matrix[n2][n1] = matrix[n1][n2];
}

std::vector<int> Graph::getPath(std::vector<int> const &parent, int j) {
    std::vector<int> path;
    while (j != -1) {
        path.push_back(j);
        j = parent[j];
    }
    reverse(path.begin(), path.end());
    return path;
}
void Graph::readGraph(std::string file) {
    std::ifstream fileIn;
    std::vector<sf::Vector2f> coordenates(22);
    int nEdge, from, to;

    fileIn.open(file);

    for (int i = 0; i<20 ; i++) {
        fileIn >> coordenates[i].x >> coordenates[i].y;
    }

    fileIn >> nEdge;

    for(int i = 0; i < nEdge; i++) {
        fileIn >> from >> to;
        insertEdge(coordenates[from - 1], coordenates[to - 1], from, to);
    }

    fileIn.close();
}

// Functions
std::vector<int> Graph::dijkstra(int src, int end) {
    int V = matrix[0].size();

    std::vector<int> dist(V, INF), parent(V, -1);
    std::vector<bool> visited(V, false);

    dist[src] = 0;

    for (int count = 0; count < V-1; count++) {
        int u = -1;

        for(int i=0; i<V; i++)
            if (!visited[i] && (u == -1 || dist[i] < dist[u]))
                u = i;

        if (u == -1)
            break;

        visited[u] = true;

        for (int v = 0; v < V; v++)
            if (matrix[u][v] != 0 && !visited[v] && dist[u] != INF
                && dist[u] + matrix[u][v] < dist[v]) {
                dist[v] = dist[u] + matrix[u][v];
                parent[v] = u;
            }
    }

    std::vector<int> path = getPath(parent, end);
    return path;
}

// Game map constructor
GameMap::GameMap() : Graph(22) {
    readGraph("../../src/graphMaps/map.txt");
}

// Game Levels

// Constructors and destructors
GameLevels::GameLevels() {
    this->levelsVector = new std::vector<Level>;
    this->levelsVector->reserve(5);
}

GameLevels::~GameLevels() = default;

// Accessors
std::vector<Level> * GameLevels::getLevels() const &{
    return levelsVector;
}

void GameLevels::addLevel(Level newLevel) {
    this->levelsVector->push_back(newLevel);
}