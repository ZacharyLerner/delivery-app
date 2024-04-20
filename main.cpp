#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include "quadtree.h"
#include "dijkstra.h"

int main() {
    // read in points from a file
    std::ifstream file("test2.txt");
    std::vector<std::vector<std::string>> points;
    std::string line;
    int width = 0;
    int height = 0;

    // break up the points based on spaces
    while (std::getline(file, line)) {
        std::vector<std::string> point;
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ',')) {
            point.push_back(token);
        }
        points.push_back(point);
        width = point.size();
        height++;
    }

    // print out the points
    for (std::vector<std::string> point : points) {
        for (std::string c : point) {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }

    // create a quadtree with a boundary of the vector
    Rectangle boundary = {0, 0, (float)width, (float)height};
    Quadtree quadtree(boundary);

    // insert the points into the quadtree
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points[i].size(); j++) {
            if (points[i][j] != "0") {
                float x = j;
                float y = i;
                std::string c = points[i][j];
                std::cout << c << std::endl;
                Point p = {x, y, c};
                quadtree.insert(p);
            }
        }
    }
    quadtree.print();
    quadtree.visualize();

    // create a grid for the dijkstra algorithm
    std::vector<std::vector<int>> grid(height, std::vector<int>(width, 0));
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points[i].size(); j++) {
            if (points[i][j] != "0") {
                grid[i][j] = 1;
            }
        }
    }

    // create a dijkstra object with the grid

   // go through every "house" represetned by a number above 0, and find its paths to itself and the other houses
   
   // create a vector of points that have houses
    std::vector<Point> houses;
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points[i].size(); j++) {
            if (std::stoi(points[i][j]) > 0) {
                float x = j;
                float y = i;
                std::string c = points[i][j];
                Point p = {x, y, c};
                houses.push_back(p);
            }
        }
    }
    Dijkstra dijkstra(grid);

    std::vector<std::pair<int, int>> paths = dijkstra.findShortestPath(7, 6, 103, 102);

    for (std::pair<int, int> p : paths) {
        std::cout << p.first << " " << p.second << std::endl;
    }

    std::cout << paths.size() << std::endl;

    /*
    for(int i = 0; i < houses.size(); i ++){
        for(int j = 0; j < houses.size();  j++){
            //IMPORTANT: CREATE A NEW DIJKSTRA OBJECT FOR EACH PAIR OF HOUSES
            Dijkstra dijkstra(grid);
            std::vector<std::pair<int, int>> paths;
            std::cout << "Path from " << houses[i].c << " to " << houses[j].c << std::endl;
            paths = dijkstra.findShortestPath(houses[i].x, houses[i].y, houses[j].x, houses[j].y);
            for (std::pair<int, int> p : paths) {
                std::cout << p.first << " " << p.second << std::endl;
            }
            std::cout << paths.size() << std::endl;
        }

    }
    */
}


