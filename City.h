// Created by david on 4/10/2024.

#ifndef UNTITLED17_CITY_H
#define UNTITLED17_CITY_H

#include <string>
#include <vector>
#include <cmath>
#include <random>

/*
 * City generator class, given a size as integer procedurally generates a city
 * that will have a max rows and columns such that rows or cols are evenly divisible by 4
 */
class City {
private:
    // Constants to represent different types of cells in the city map
    const int EMPTY = -3;
    const int HUB = -2;
    const int ROAD = -1;

    // Directions N,S,E,W
    const std::vector<std::pair<int,int>> DIRECTIONS = {{-1,0},{1,0},{0,1},{0,-1}};

    // Class attributes
    std::vector<std::vector<int>> cityMap;
    int rows, cols;
    int mapSize;
    int roadCount = 0;
    int houseCount = 0;

    int hubx = -1;
    int huby = -1;

    // validation and positional checking
    bool isHouse(std::pair<int,int> coordinates);
    bool isRoad(std::pair<int,int> coordinates);
    bool isEmpty(std::pair<int,int> coordinates);
    bool isValid(std::pair<int,int> coordinates);
    bool inBounds(std::pair<int,int> coordinates) const;

    // spacial localization and orientation
    int getSpot(int row, int col);
    std::pair<int,int> pickRandomSpot(std::mt19937& gen) const;
    bool countAdjacentRoads(std::pair<int,int> coordinates);
    std::pair<int,int> updateCoordinates(std::pair<int,int> coordinates, std::pair<int,int> direction);

    // direction choosing
    std::pair<int,int> choseOptimalDirection(std::pair<int,int> coordinates, std::pair<int,int> currentDirection, std::mt19937& gen);
    std::pair<int,int> choseSidewaysDirection(std::pair<int,int> coordinates, std::pair<int,int> currentDirection, std::mt19937& gen);

    // road and infrastructure building
    int probeDirection(std::pair<int,int> coordinates, std::pair<int,int> direction);
    int probeBounds(std::pair<int,int> coordinates, std::pair<int,int> direction);

    void buildRoad(std::pair<int,int> coordinates);
    void buildHouse(std::pair<int,int> coordinates, std::pair<int,int> direction);

    void buildHighway(std::pair<int,int> curCoordinates, std::pair<int,int> curDirection, int maxLength, std::mt19937& gen);
    void buildStreet(std::pair<int,int> curCoordinates, std::pair<int,int> curDirection, int maxLength, std::mt19937& gen);
    void buildNeighborhood(std::pair<int,int> coordinates, std::pair<int,int> direction, int maxLength, std::mt19937& gen);

    std::vector<std::pair<int,int>> buildHub(std::mt19937& gen);

    // main generators / runners
    void generateCity(std::mt19937& gen);
    void generateMap();

    // random number generators
    int generateRandomNeighborhoodLength(std::pair<int,int> coordinates, std::pair<int,int> currentDirection, std::mt19937& gen);
    int generateRandomStreetLength(std::pair<int,int> coordinates, std::pair<int,int> currentDirection, std::mt19937& gen);
    int generateRandomHighwayLength(std::pair<int,int> coordinates, std::pair<int,int> currentDirection,std::mt19937& gen);

public:
    // City constructors
    City();
    explicit City(int size);

    // City getter methods
    int getMaxRows() const;
    int getMaxCols() const;
    int getHouseCount() const;
    std::pair<int,int> getHubLocation() const;


    // public random number generator for utility
    int generateRandomNumber(std::mt19937& gen, int min, int max);

    // print the map to file
    void printMapToFile(const std::vector<std::vector<int>>& matrix);
};

#endif //UNTITLED17_CITY_H
