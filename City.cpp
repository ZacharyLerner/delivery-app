// Created by david on 4/10/2024.

#include "City.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

// Constructors

City::City() {
    this->rows = 64;
    this->cols = 64;
    this->mapSize = 64;
    this->cityMap = std::vector<std::vector<int>>(cols, std::vector<int>(rows, EMPTY));

    generateMap();
}

City::City(int size) {
    // ensure its large enough to generate anything meaningful
    if (size <= 1) {
        size = 3;
    // ensure its small enough to output
    } else if (size >= 2) {
        size = 4;
    }

    this->mapSize = (int)pow(4,size);
    this->rows = (int)pow(4,size);
    this->cols = (int)pow(4, size);

    this->cityMap = std::vector<std::vector<int>>(cols, std::vector<int>(rows, EMPTY));

    generateMap();
}
// end of constructors

void City::printMapToFile(const std::vector<std::vector<int>>& matrix) {
    std::ofstream outFile("map.txt"); // Create an ofstream object for output to a file

    if (!outFile) {
        std::cerr << "Error opening file for writing.\n";
        return;
    }

    // Output the 2D vector to the file with double spaces
    for (const auto &row : matrix) {
        for (int elem : row) {
            if (elem == -1) {
                outFile << "-1"; // Double space after each character
            } else if (elem == -2) {
                outFile << "-2";
            } else if (elem == -3) {
                outFile << "0";
            } else if (elem > 0) {
                outFile << elem;
            }
            outFile << ",";
        }
        outFile << "\n"; // New line after each row
    }

    outFile.close(); // Close the file after writing
}

// random number generators and associated random generators

/**
 * Generate a random number from the min to the max (inclusive)
 * @param gen
 * @param min the minimum number to generate
 * @param max the maximum number to generate
 */
int City::generateRandomNumber(std::mt19937& gen, int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

int City::generateRandomHighwayLength(std::pair<int,int> coordinates, std::pair<int,int> currentDirection, std::mt19937& gen) {
    int maxLength = probeDirection(coordinates,currentDirection) * 4;
    int minLength = probeDirection(coordinates,currentDirection) * 2;
    int length = generateRandomNumber(gen, minLength, maxLength);

    if (length > maxLength || length < minLength) {
        length = 0; // if an invalid range is used the rng will spit out codebreaking numbers so error check
    }

    return length;
}

int City::generateRandomStreetLength(std::pair<int,int> coordinates, std::pair<int,int> currentDirection, std::mt19937& gen) {
    int maxLength = probeDirection(coordinates,currentDirection) - 3;
    int minLength = probeDirection(coordinates,currentDirection) / 2;
    int length = generateRandomNumber(gen, minLength, maxLength);

    if (length > maxLength || length < minLength) {
        length = 0; // if an invalid range is used the rng will spit out codebreaking numbers so error check
    }

    return length;
}

int City::generateRandomNeighborhoodLength(std::pair<int,int> coordinates, std::pair<int,int> currentDirection, std::mt19937& gen) {
    int maxLength = probeDirection(coordinates,currentDirection) / 2;
    int minLength = probeDirection(coordinates,currentDirection) / 8;
    int length = generateRandomNumber(gen, minLength, maxLength);

    if (length > maxLength || length < minLength) {
        length = 0; // if an invalid range is used the rng will spit out codebreaking numbers so error check
    }

    return length;
}

// ####################################################################################################################
// validation and positional checking functions

bool City::isHouse(std::pair<int,int> coordinates) {
    // if the value at that point is greater than road it's a house
    if (this->cityMap[coordinates.first][coordinates.second] > ROAD) {
        return true;
    }
    return false;
}

bool City::isRoad(std::pair<int,int> coordinates) {
    if (this->cityMap[coordinates.first][coordinates.second] == ROAD) {
        return true;
    }
    return false;
}

bool City::isEmpty(std::pair<int,int> coordinates) {
    if (this->cityMap[coordinates.first][coordinates.second] == EMPTY) {
        return true;
    }
    return false;
}

/**
 * Checks if a spot is valid by getting the spot and checking if it is empty
 * @return true if the spot is valid, false if it is not
 */
bool City::isValid(std::pair<int,int> coordinates) {
    // if the position isn't in bounds return false
    if (!inBounds(coordinates)) {
        return false;
    }

    // if the position is empty return true
    if (isEmpty(coordinates)) {
        return true; // the spot is empty and therefor valid
    }
    // otherwise return false as the spot is invalid
    return false;
}

/**
 * Checks if a spot is in bounds depending on its coordinates and the max row and col size
 * @param coordinates
 * @return true if in bounds false if not
 */
bool City::inBounds(std::pair<int,int> coordinates) const {
    if (coordinates.first < getMaxRows() && coordinates.second < getMaxCols()) {
        if (coordinates.first >= 0 && coordinates.second >= 0) {
            return true;
        }
    }

    return false;
}

// end of validation and positional checking
// ####################################################################################################################
// public getter functions

/**
 * Gets the spot at row and col from the cityMap
 * @param row the row of the spot to check
 * @param col the col of the spot to check
 * @return the information stored at row, col
 */
int City::getSpot(int row, int col) {
    return this->cityMap[row][col];
}

// getter for max row
int City::getMaxRows() const {
    return this->rows;
}

// getter for max col
int City::getMaxCols() const {
    return this->cols;
}

int City::getHouseCount() const {
    return this->houseCount;
}

std::pair<int,int> City::getHubLocation() const {
    return std::make_pair(this->hubx, this->huby);
}

// end of getters
// ####################################################################################################################
// Polymorphic street building methods for roads and infrastructure

/**
 * Builds a road on the specified coordinates
 * @param coordinates
 */
void City::buildRoad(std::pair<int,int> coordinates) {
    if(isValid(coordinates)) {
        cityMap[coordinates.first][coordinates.second] = ROAD;
    }
}

/**
 * Builds a house on the specified coordinates as long as it is a valid spot
 * The house count is incremented and then the house is places as the house count
 * this gives every house a unique number allowing differentiation between them
 * @param coordinates
 */
void City::buildHouse(std::pair<int,int> coordinates, std::pair<int,int> direction) {
    std::pair<int,int> newCoordinates = updateCoordinates(coordinates,direction);
    if(isValid(newCoordinates)) {
        this->houseCount++;
        cityMap[newCoordinates.first][newCoordinates.second] = houseCount;
    }
}

/**
 * Builds a long and straight road and calls the street builder to build branches from this "highway"
 * terminates if the new coordinates will be out of bounds, exceed max length, or has too many adjacent roads
 * @param curCoordinates the current coordinates to build a road on
 * @param curDirection the current direction we are going
 * @param maxLength the maximum length of the road possible including turns
 * @param gen the random generator
 */
void City::buildHighway(std::pair<int,int> curCoordinates, std::pair<int,int> curDirection, int maxLength, std::mt19937& gen) {
    bool isRandomChosen = false;    // if we have chosen the right random number to make a sidestreet
    bool isGenerating = true;       // flag for generation
    int currentLengthOfRoad = 0;    // the current length of the road
    int iterationsNoBranch = 0;     // currrent iterations without branching off
    int maxIterationsNoBranch = 12; // the max iterations without branching off


    while (isGenerating) {
        // build a road on the current spot
        buildRoad(curCoordinates);

        if (countAdjacentRoads(curCoordinates)) {
            return; // if there are too many adjacent roads after placing that road then terminate
        }

        // Update our coordinates to the next spot in the direction we are going
        curCoordinates = updateCoordinates(curCoordinates,curDirection);

        if (!inBounds(curCoordinates) || currentLengthOfRoad > maxLength) {
            return; // if out of bounds, or maxLength exceeded, terminate generation
        }

        // if there isn't much room left for the road then turn to an optimal direction for it to continue
        if (probeBounds(curCoordinates,curDirection) <= 3) {
            curDirection = choseOptimalDirection(curCoordinates,curDirection,gen);
            iterationsNoBranch = 0; // we count a turn as a branch otherwise it generates poorly
        }

        // generate a random number 1 -> 12
        int randomNumber = generateRandomNumber(gen,1,12);
        if (randomNumber == 1) {
            isRandomChosen = true; // we can make a branch at the next available spot
        }

        // if we have met the conditions to branch i.e.
        // (isRandomChosen; No branches for 6 tiles; or max iterations without branch is reached)
        if (isRandomChosen && iterationsNoBranch >= 6 || iterationsNoBranch > maxIterationsNoBranch) {
            std::pair<int,int> newDirection = choseSidewaysDirection(curCoordinates,curDirection,gen);
            std::pair<int,int> newCoordinates = updateCoordinates(curCoordinates,newDirection);

            // make a street with the new direction and coordinates
            int newMaxLength = generateRandomStreetLength(newCoordinates,newDirection,gen);
            buildStreet(newCoordinates,newDirection,newMaxLength,gen);

            // reset our branch conditions as one was just made
            iterationsNoBranch = 0;
            isRandomChosen = false;
        }
        // increment iterations without branch and length of road every time we move
        iterationsNoBranch++;
        currentLengthOfRoad++;
    }
}

void City::buildStreet(std::pair<int,int> curCoordinates, std::pair<int,int> curDirection, int maxLength, std::mt19937& gen) {
    bool isRandomChosen = false;    // if we have chosen the right random number to make a sidestreet
    bool isGenerating = true;       // flag for generation
    int currentLengthOfRoad = 0;    // the current length of the road
    int iterationsNoBranch = 0;     // currrent iterations without branching off
    int maxIterationsNoBranch = 6; // the max iterations without branching off


    while (isGenerating) {
        // build a road on the current spot
        buildRoad(curCoordinates);

        if (countAdjacentRoads(curCoordinates)) {
            return; // if there are too many adjacent roads after placing that road then terminate
        }

        // Update our coordinates to the next spot in the direction we are going
        curCoordinates = updateCoordinates(curCoordinates,curDirection);

        if (!inBounds(curCoordinates) || currentLengthOfRoad > maxLength) {
            return; // if out of bounds, or maxLength exceeded, terminate generation
        }

        // if there isn't much room left for the road then turn to an optimal direction for it to continue
        if (probeBounds(curCoordinates,curDirection) <= 3) {
            curDirection = choseOptimalDirection(curCoordinates,curDirection,gen);
            iterationsNoBranch = 0; // we count a turn as a branch otherwise it generates poorly
        }

        // generate a random number 1 -> 8
        int randomNumber = generateRandomNumber(gen,1,8);
        if (randomNumber == 1) {
            isRandomChosen = true; // we can make a branch at the next available spot
        }

        // if we have met the conditions to branch i.e.
        // (isRandomChosen; No branches for 4 tiles; or max iterations without branch is reached)
        if (isRandomChosen && iterationsNoBranch >= 3 || iterationsNoBranch > maxIterationsNoBranch) {
            std::pair<int,int> newDirection = choseSidewaysDirection(curCoordinates,curDirection,gen);
            std::pair<int,int> newCoordinates = updateCoordinates(curCoordinates,newDirection);

            // make a street with the new direction and coordinates
            int newMaxLength = generateRandomNeighborhoodLength(newCoordinates,newDirection,gen);
            buildNeighborhood(newCoordinates,newDirection,newMaxLength,gen);

            // reset our branch conditions as one was just made
            iterationsNoBranch = 0;
            isRandomChosen = false;
        }
        // increment iterations without branch and length of road every time we move
        iterationsNoBranch++;
        currentLengthOfRoad++;
    }
}

void City::buildNeighborhood(std::pair<int,int> curCoordinates, std::pair<int,int> curDirection, int maxLength, std::mt19937& gen) {
    bool isRandomChosen = false;    // if we have chosen the right random number to make a sidestreet
    bool isGenerating = true;       // flag for generation
    int currentLengthOfRoad = 0;    // the current length of the road
    int iterationsNoHouse = 0;     // currrent iterations without branching off
    int maxIterationsNoHouse = 2; // the max iterations without branching off


    while (isGenerating) {
        // build a road on the current spot
        buildRoad(curCoordinates);

        if (countAdjacentRoads(curCoordinates)) {
            return; // if there are too many adjacent roads after placing that road then terminate
        }

        // Update our coordinates to the next spot in the direction we are going
        curCoordinates = updateCoordinates(curCoordinates,curDirection);

        if (!inBounds(curCoordinates) || currentLengthOfRoad > maxLength) {
            return; // if out of bounds, or maxLength exceeded, terminate generation
        }

        // if there isn't much room left for the road then turn to an optimal direction for it to continue
        if (probeBounds(curCoordinates,curDirection) <= 3) {
            curDirection = choseOptimalDirection(curCoordinates,curDirection,gen);
            iterationsNoHouse = 0; // we count a turn as a branch otherwise it generates poorly
        }

        // generate a random number 1 -> 3
        int randomNumber = generateRandomNumber(gen,1,3);
        if (randomNumber == 1) {
            isRandomChosen = true; // we can make a branch at the next available spot
        }

        // if we have met the conditions to branch i.e.
        // (isRandomChosen; No branches for 3 tiles; or max iterations without branch is reached)
        if (isRandomChosen && iterationsNoHouse >= 1 || iterationsNoHouse > maxIterationsNoHouse) {
            // make houses along the street
            std::pair<int,int> sidewaysDirection = choseSidewaysDirection(curCoordinates,curDirection,gen);
            buildHouse(curCoordinates,sidewaysDirection);

            // reset our branch conditions as one was just made
            iterationsNoHouse = 0;
            isRandomChosen = false;
        }
        // increment iterations without branch and length of road every time we move
        iterationsNoHouse++;
        currentLengthOfRoad++;
    }
}

/**
 * Place the hub randomly on the cityMap then generate a highway going N/S or E/W adjacent to the hub
 * this will prompt the rest of the maps generation. The random spot this choses is basically the origin point
 * @param gen the random number generator
 * @returns a vector of two pairs, the first is the coordinates of the hub, the second is the direction to go
 */
std::vector<std::pair<int,int>> City::buildHub(std::mt19937& gen) {
    // randomly find hub location
    std::pair<int,int> currentLocation = pickRandomSpot(gen);
    this->hubx = currentLocation.first;
    this->huby = currentLocation.second;
    this->cityMap[hubx][huby] = HUB;

    // at this location find the optimal direction (can also be done mathematically)
    std::pair<int,int> optimalDirection = choseOptimalDirection(currentLocation,std::make_pair(0,0),gen);
    std::pair<int,int> adjacentCoordinates;

    // if we want to go north or south, go to the east or west of the hub
    if (optimalDirection.first != 0) {
        for (int i = 2; i < 4; i++) {
            adjacentCoordinates = updateCoordinates(currentLocation,this->DIRECTIONS[i]);
            if (isValid(adjacentCoordinates)) {
                break;
            }
        }
    } else if (optimalDirection.second != 0) {
        for (int i = 0; i < 2; i++) {
            adjacentCoordinates = updateCoordinates(currentLocation, this->DIRECTIONS[i]);
            if (isValid(adjacentCoordinates)) {
                break;
            }
        }
    }

    // make a return vector to return the coordinates of the adjacent tile aswell as the direction to build
    std::vector<std::pair<int,int>> information;
    information.push_back(adjacentCoordinates);
    information.push_back(optimalDirection);

    return information;
}

// end of polymorphic street building methods for roads and infrastructure
// ####################################################################################################################
// Spatial localization and orientation

/**
 * Given current coordinates and a direction update the coordinates to the location of the directions
 * @param coordinates the current coordinates
 * @param direction the direction you are moving as given by choseNewDirection
 * @return
 */
std::pair<int,int> City::updateCoordinates(std::pair<int,int> coordinates, std::pair<int,int> direction) {
    std::pair<int,int> newCoordinates;
    newCoordinates.first = coordinates.first + direction.first;
    newCoordinates.second = coordinates.second + direction.second;
    return newCoordinates;
}

/**
 * Count how many adjacent roads there are. This should return 1 until you meet more roads assuming youre coming
 * from a road. When this returns > 1 you know youve encountered roads that are parallel. So generate that last road
 * and then terminate generation to prevent parallel road generation
 * @param coordinates your current coordinates
 * @return the number of adjacent roads
 */
bool City::countAdjacentRoads(std::pair<int,int> coordinates) {
    int adjacentRoads = 0;
    for (std::pair<int,int> direction : this->DIRECTIONS) {
        std::pair<int,int> newCoordinates = updateCoordinates(coordinates,direction);
        // if the newDirection is in bounds (check first) and is a road (check second)
        if(inBounds(newCoordinates)) {
            if(isRoad(newCoordinates)) {
                // increment adjacent Roads
                adjacentRoads++;
            }
        }
    }

    if (adjacentRoads > 2) {
        return true;
    }
    return false;
}

/**
 * Choses a direction to the left or right of the current road generation
 * @param coordinates
 * @param currentDirection
 * @return the new direction thats perpendicular to the current one
 */
std::pair<int,int> City::choseSidewaysDirection(std::pair<int,int> coordinates, std::pair<int,int> currentDirection, std::mt19937& gen) {
    int random;
    if (currentDirection.first != 0 ) {         // we must be going north or south so go east or west (by chosing 2 or 3)
        random = generateRandomNumber(gen,2,3);

    } else if (currentDirection.second != 0) {  // we must be going east or west so go north or south (by chosing 0 or 1)
        random = generateRandomNumber(gen,0,1);
    }

    return this->DIRECTIONS[random]; // return the direction to move
}

/**
 * Given coordinates chose the optimal direction to move (optimal meaning the direction with the longest
 * unobstructed road path that doesnt cause parallel road generation
 * @param coordinates
 * @param direction
 * @return
 */
std::pair<int,int> City::choseOptimalDirection(std::pair<int,int> coordinates, std::pair<int,int> currentDirection, std::mt19937& gen) {
    std::pair<int,int> finalDirection;
    int maxLen = 0;
    int probeLength;

    // for each direction in direction
    for (std::pair<int,int> newDirection : this->DIRECTIONS) {
        // if the selected direction is not equal to the current direction
        if (newDirection != currentDirection) {
            // check how far it goes
            probeLength = probeDirection(coordinates,newDirection);

            // if it is the furthest one we found yet that is our new final direction to return
            if (probeLength > maxLen) {
                maxLen = probeLength;
                finalDirection = newDirection;
            }
        }
    }

    // return the furthest direction found
    return finalDirection;
}

/**
 * Sees how far in each direction you can go without the tile becoming invalid, helper method for
 * choseOptimalDirection(coordinates, direction) to determine the optimal direction with the most empty space
 * @param coordinates
 * @param direction
 * @return the length that direction goes without being interrupted
 */
int City::probeDirection(std::pair<int,int> coordinates, std::pair<int,int> direction) {
    std::pair<int,int> newCoordinates = updateCoordinates(coordinates,direction);
    int distance = 0;
    while (isValid(newCoordinates)) {
        newCoordinates = updateCoordinates(newCoordinates,direction);
        distance++;
    }
    return distance;
}

/**
 * Polymorphic probe direction method that instead checks to make sure we are in bounds and wont hit a house
 * this WILL NOT count roads. This allows roads to connect when using this method for validation
 * @param coordinates the current coordinates
 * @param direction the current direction
 * @return the length before a house or boundary is reached
 */
int City::probeBounds(std::pair<int,int> coordinates, std::pair<int,int> direction) {
    std::pair<int,int> newCoordinates = updateCoordinates(coordinates,direction);
    int distance = 0;
    while (inBounds(newCoordinates) && !isHouse(newCoordinates)) {
        newCoordinates = updateCoordinates(newCoordinates,direction);
        distance++;
    }
    return distance;
}


/**
 * Pick a random spot on the board in O(1) time. This spot may already be occupied
 * @param gen the random number generator
 * @return the random spot as coordinates
 */
std::pair<int,int> City::pickRandomSpot(std::mt19937& gen) const {
    // generate a random number between the range of 0 to maxRows(and cols) - 1
    std::uniform_int_distribution<int> distribution_row(0, this->mapSize - 1);
    std::uniform_int_distribution<int> distribution_col(0, this->mapSize - 1);

    int random_row = distribution_row(gen);
    int random_col = distribution_col(gen);

    // return a pair of random x y coordinates using the given distribution ranges
    return std::make_pair(random_row,random_col);
}

// end spatial localization and orientation
// ####################################################################################################################
// Main generation methods

/**
 * Creates a random number generator and passes it into the city builder function
 * Basically calls the city generation through generateCity(std::mt19937 gen)
 */
void City::generateMap() {
    std::random_device rd;  // a random seed for the mt19937
    std::mt19937 gen(rd()); // random number generator with a random seed
    generateCity(gen);
}

/**
 * Main city generator, called by generateMap()
 * @param gen
 */
void City::generateCity(std::mt19937& gen) {
    std::pair<int,int> currentSpot;
    std::pair<int,int> currentDirection;


    // build the hub and return the coordinates of a valid position next to the hub to build a road
    std::vector<std::pair<int,int>> hubAdjacency;
    hubAdjacency = buildHub(gen);
    currentSpot = hubAdjacency[0];
    currentDirection = hubAdjacency[1];

    int maxLength = generateRandomHighwayLength(currentSpot,currentDirection,gen);

    // ensure we generate a map with houses, it is random after all
    while (this->houseCount == 0) {
        buildHighway(currentSpot,currentDirection,maxLength,gen);
    }

    //printMap(this->cityMap); // prints the map to the console
    printMapToFile(this->cityMap);
}

// end main generation methods
// ####################################################################################################################