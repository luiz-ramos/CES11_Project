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
GameMap::GameMap() : Graph(21) {
    sf::Vector2f v1(612, 567), v2(431, 567), v3(431, 541), v4(335, 541), v5(335, 420), v6(298, 420), v7(298, 442), v8(400, 420), v9(400, 302), v10(494, 302), v11(400, 229), v12(340, 229), v13(340, 160), v14(300, 229), v15(212, 229), v16(212, 94),v17(172, 229), v18(172, 320),v19(128, 320),v20(128, 442);
    this->insertEdge(v1, v2, 1, 2);
    this->insertEdge(v2, v3, 2, 3);
    this->insertEdge(v3, v4, 3, 4);
    this->insertEdge(v4, v5, 4, 5);
    this->insertEdge(v8, v5, 8, 5);
    this->insertEdge(v5, v6, 5, 6);
    this->insertEdge(v7, v6, 7, 6);
    this->insertEdge(v9, v8, 9, 8);
    this->insertEdge(v10, v9, 10, 9);
    this->insertEdge(v11, v9, 11, 9);
    this->insertEdge(v11, v12, 11, 12);
    this->insertEdge(v13, v12, 13, 12);
    this->insertEdge(v14, v12, 14, 12);
    this->insertEdge(v14, v15, 14, 15);
    this->insertEdge(v14, v6, 14, 6);
    this->insertEdge(v16, v15, 16, 15);
    this->insertEdge(v17, v15, 17, 16);
    this->insertEdge(v17, v18, 17, 18);
    this->insertEdge(v19, v18, 19, 18);
    this->insertEdge(v19, v20, 19, 20);
    this->insertEdge(v7, v20, 7, 20);
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