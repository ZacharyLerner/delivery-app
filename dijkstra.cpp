#include "dijkstra.h" 
#include <queue>
#include <limits>
#include <iostream>
#include <vector>

Dijkstra::Dijkstra(const std::vector<std::vector<int>>& grid) : grid(grid) {
    // rows and collumns are set to the grid size.
    rows = grid.size();
    cols = grid[0].size();

    // vectors are created for the visited squares, distances, and previous squares.
    visited = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false));
    // distances are set to the max value of an int, as this is how dijkstras works
    distances = std::vector<std::vector<int>>(rows, std::vector<int>(cols, std::numeric_limits<int>::max()));
    previous = std::vector<std::vector<std::pair<int, int>>>(rows, std::vector<std::pair<int, int>>(cols, std::make_pair(-1, -1)));
}
Dijkstra::~Dijkstra() {   
    // no dynamic memory currently so no need to delete anything
}

bool Dijkstra::isValid(int x, int y) {
    // checks if the x and y values are within the grid
    return x >= 0 && x < cols && y >= 0 && y < rows;
}

std::vector<std::pair<int, int>> Dijkstra::getNeighbors(int x, int y) {
    // uses pairs to find grid cords of neighbors
    std::vector<std::pair<int, int>> neighbors;

    // checks in 4 directs to find a neighbor
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto& dir : directions) {
        int newX = x + dir.first;
        int newY = y + dir.second;
        // if it is in the bounds of the grid and isnt = 0, which is our current value for an obisticle, add it to the neighbors vector
        if (isValid(newX, newY) && grid[newY][newX] != 0) {
            neighbors.push_back(std::make_pair(newX, newY));
        }
    }
    // return the neighbors
    return neighbors;
}

// finds the shortest path from two points on the grid, returns this in a vector of pair cords
std::vector<std::pair<int, int>> Dijkstra::findShortestPath(int startX, int startY, int endX, int endY) {
    // ceates a priority queue that stores distances and cords of the grid from smallest to largest
    std::priority_queue<std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>, std::greater<std::pair<int, std::pair<int, int>>>> pq;

    // sets the distance of the start point to 0
    pq.push(std::make_pair(0, std::make_pair(startX, startY)));
    distances[startY][startX] = 0;

   // loops until the priority queue is empty
    while (!pq.empty()) {
        // gets the distance of the current node
        int dist = pq.top().first;

        // x and y values of the current node
        int x = pq.top().second.first;
        int y = pq.top().second.second;
        
        // pop the node of the smallest distance from the priority queue
        pq.pop();
        
        if (x == endX && y == endY) {
            // if the current node is the end node then break the loop
            break;
        }
        
        // if the current node has not been visited before, mark it as visited
        if (!visited[y][x]) {
            visited[y][x] = true;
            // for each unvisited neighbor of the current node, calculate the new distance to reach that neighbor through the current node
            for (const auto& neighbor : getNeighbors(x, y)) {
                int newX = neighbor.first;
                int newY = neighbor.second;
                int newDist = dist + 1; // due to our current sturcture weights are all 1, which is not a great use of dijkstras

                // if the new distance is smaller than the previously recorded distance for that neighbor, update the distance and add the neighbor to the priority queue
                if (newDist < distances[newY][newX]) {
                    distances[newY][newX] = newDist;
                    previous[newY][newX] = std::make_pair(x, y);
                    pq.push(std::make_pair(newDist, std::make_pair(newX, newY)));
                }
            }
        }
    }
    // creates a vector of pairs to store the path
    std::vector<std::pair<int, int>> path;
    int x = endX;
    int y = endY;

    // while the x and y values are not -1, add the cords to the path vector
    while (x != -1 && y != -1) {
        // add the cords to the path vector
        path.push_back(std::make_pair(x, y));

        // get the previous cords
        // uses auto to get the type of the previous cords
        auto prev = previous[y][x];
        x = prev.first;
        y = prev.second;
    }

    // reverse the path vector and return it
    std::reverse(path.begin(), path.end());
    return path;
}


