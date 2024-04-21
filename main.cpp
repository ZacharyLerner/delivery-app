#include <iostream>
#include <random>
#include <set>
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "bucketsort.h"
#include "City.h"
#include "quadtree.h"
#include "dijkstra.h"

/**
 * Find the original index of the lowest value in numbers by comparing it to the unsorted vector
 * @param numbers
 * @return the lowest index
 */
int findLowestIndex(std::vector<int> sortedNumbers, std::vector<int> unsortedNumbers) {
    int min = sortedNumbers[0];
    for (int index = 0; index < unsortedNumbers.size(); index++) {
        if (unsortedNumbers[index] == min) {
            return index;
        }
    }
    return -1; // this should never run as it will break the program. Both vectors should contain all the same numbers
}

/**
 * Simple contains method as C++11 doesnt have one for vectors
 * @param numbers
 * @param searchNum
 * @return
 */
bool contains(std::vector<int>& numbers, int searchNum) {
    for (int element : numbers) {
        if (element == searchNum) {
            return true;
        }
    }
    return false;
}

/**
 * Find the coordinates of a house given the house number and the vector of all house points made from the quadtree
 * @param houseNumber house you would like to find
 * @param houses vector of house locations as points
 * @return the location as a pair
 */
std::pair<int,int> findCoordinates(int houseNumber, std::vector<Point>& houses) {
    for (const Point point : houses) {
        if (point.c == std::to_string(houseNumber)) {
            return std::make_pair(point.y, point.x);
        }
    }
    std::cerr << "Invalid coordinate generated." << std::endl;
    return std::make_pair(-1,-1); // this should break the program but should also never run
}

/**
 * Generate a random amount of deliveries (house locations) to deliver to given a rng, citymap, and all houses
 * @param gen the random number generator
 * @param cityMap the cityMap for totalHouses and random number generation
 * @param houses a vector of every house on the cityMap and its coordinates
 * @return
 */
std::vector<std::pair<int,int>> generateDeliveries(std::mt19937& gen, City& cityMap, std::vector<Point>& houses) {
    int MAX_ORDERS = 7;
    int MIN_ORDERS = 2;
    std::vector<std::pair<int,int>> houseCoordinates;
    std::vector<int> pickedHouses;

    // generate a random number of deliveries from 2 - 7
    int deliveries = cityMap.generateRandomNumber(gen,MIN_ORDERS,MAX_ORDERS);

    // for as many deliveries generated find houses to deliver to
    for (int i = 0; i < deliveries; i++) {
        // generate a random house number that exists
        int randomHouse = cityMap.generateRandomNumber(gen,1,cityMap.getHouseCount());
        // if this house is already set for a delivery find another one
        while (contains(pickedHouses, randomHouse)) {
            randomHouse = cityMap.generateRandomNumber(gen,1,cityMap.getHouseCount());
        }
        pickedHouses.push_back(randomHouse);
        // find the coordinates of the picked house and push them to the list (the coordinates are flipped to be usable)
        houseCoordinates.push_back(findCoordinates(randomHouse, houses));
    }
    return houseCoordinates;
}

/**
 * Find the path distances from the start to all houseLocations passed as a vector of pairs. This helps in finding the shortest
 * @param start starting location as pair
 * @param houseLocations all house locations as vector of pairs
 * @param grid the dijkstra grid
 * @return
 */
std::vector<int> findPathDistances(std::pair<int,int> start, std::vector<std::pair<int,int>> houseLocations, std::vector<std::vector<int>>& grid) {
    std::vector<int> pathLengths;
    for (std::pair<int,int> houseLocation : houseLocations) {
        Dijkstra dijkstra(grid);
        std::vector<std::pair<int, int>> paths = dijkstra.findShortestPath(start.second,start.first, houseLocation.second, houseLocation.first);
        pathLengths.push_back((int)paths.size());
    }
    return pathLengths;
}

int main(int argc, char* argv[]) {
    // make a random number generator
    std::random_device rd;  // a random seed for the mt19937
    std::mt19937 gen(rd()); // random number generator with a random seed

    // generate a randomized city accessible through the output file
    int size = std::stoi(argv[1]); // size must be either 1 or 2.
    City cityMap(size);

    // get the total house count for order generation
    int totalHouses = cityMap.getHouseCount();

    // Generate a quadtree and begin quadtree setup --------------------------------------------------------------------
    // read in points from a file
    std::ifstream file("map.txt");
    std::vector<std::vector<std::string>> points;
    std::string line;
    int width = 0;
    int height = 0;

    // break up the points based on commas
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
                //std::cout << c << std::endl;
                Point p = {x, y, c};
                quadtree.insert(p);
            }
        }
    }

    // create a grid for the dijkstra algorithm
    std::vector<std::vector<int>> grid(height, std::vector<int>(width, 0));
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points[i].size(); j++) {
            if (points[i][j] != "0") {
                grid[i][j] = 1;
            }
        }
    }

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
    // quadtree setup complete, now we can use dijkstras ---------------------------------------------------------------

    // Create a string stream to buffer the output
    std::stringstream buffer;
    // with everything ready start making the deliveries
    // generate up from 2 to 7 deliveries and store their locations in pickedHouses
    std::vector<std::pair<int,int>> houseLocations = generateDeliveries(gen, cityMap, houses);

    // now that we have our houses to deliver to lets make some deliveries by chosing the one closest to the hub
    // get the distance to all houses from the hub
    std::vector<int> pathLengths;
    pathLengths = findPathDistances(cityMap.getHubLocation(), houseLocations, grid);
    std::vector<int> initialPathLengths = pathLengths;

    // bucket sort the pathLengths to find the lowest length
    BucketSortInt bucketSort(1);
    // for such a small amount of distances we can use a high bucket count for speed
    bucketSort.bucket_sort(pathLengths, 10);
    // with our path lengths sorted lowest to highest pick the lowest distance as shortestIndex (the index location of the shortest distance)
    int shortestIndex = findLowestIndex(pathLengths,initialPathLengths);

    // store the delivery house location
    int lastHouseX = houseLocations[shortestIndex].first;
    int lastHouseY = houseLocations[shortestIndex].second;
    // std::cout << lastHouseY << ":" << lastHouseX << std::endl; // debug to show house locations

    // write the path output to a file
    // Create an ofstream object for file output
    std::ofstream outfile("outputPath.txt");
    // Check if the file is successfully opened
    if (!outfile) {
        std::cerr << "Error opening file." << std::endl;
        return 1; // Return with error code
    }
    Dijkstra dijkstraHub(grid);
    std::pair<int,int> hubLocation = cityMap.getHubLocation();
    std::vector<std::pair<int, int>> paths = dijkstraHub.findShortestPath(hubLocation.second,hubLocation.first, lastHouseY, lastHouseX);
    // Loop over the paths vector and write each pair to the file
    for (const std::pair<int, int>& p : paths) {
        buffer << p.first << " " << p.second << std::endl;
    }

    houseLocations.erase(houseLocations.begin()+shortestIndex); // delete the house we already visited
    shortestIndex = 0; // reset our shortest index for the next orders
    // we have successfully plotted our first delivery which comes from the hub (special case), lets do the rest of the orders

    // Polymorphic implementation of the above code to use a for loop for the remaining orders
    // for all orders we have left
    for (int order = 0; order < houseLocations.size(); order++) {
        // get the distance to all houses from the last house (our current location)
        pathLengths = findPathDistances(std::make_pair(lastHouseX, lastHouseY), houseLocations, grid);
        initialPathLengths = pathLengths;

        // for such a small amount of distances we can use a high bucket count for speed
        bucketSort.bucket_sort(pathLengths, 10);
        // with our path lengths sorted lowest to highest pick the lowest distance as shortestIndex (the index location of the shortest distance)
        shortestIndex = findLowestIndex(pathLengths,initialPathLengths);

        // write the path output to a file
        Dijkstra dijkstraHouses(grid);
        paths = dijkstraHouses.findShortestPath(lastHouseY,lastHouseX, houseLocations[shortestIndex].second, houseLocations[shortestIndex].first);

        // store the delivery house location
        lastHouseX = houseLocations[shortestIndex].first;
        lastHouseY = houseLocations[shortestIndex].second;
        // std::cout << lastHouseY << ":" << lastHouseX << std::endl; // debug to show house locations

        // Loop over the paths vector and write each pair to the file
        for (const std::pair<int, int>& p : paths) {
            buffer << p.first << " " << p.second << std::endl;
        }

        houseLocations.erase(houseLocations.begin()+shortestIndex); // delete the house we already visited
        order--;
        shortestIndex = 0; // reset our shortest index for the next orders
    }
    std::string output = buffer.str();
    outfile << output;
    return 0;
}



