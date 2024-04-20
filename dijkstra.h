#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <utility>

class Dijkstra {
public:
    Dijkstra(const std::vector<std::vector<int>>& grid);
    ~Dijkstra();
    std::vector<std::pair<int, int>> findShortestPath(int startX, int startY, int endX, int endY);

private:
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<int>> distances;
    std::vector<std::vector<std::pair<int, int>>> previous;
    int rows;
    int cols;

    bool isValid(int x, int y);
    std::vector<std::pair<int, int>> getNeighbors(int x, int y);
};

#endif