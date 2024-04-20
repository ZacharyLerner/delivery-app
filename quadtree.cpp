#include <iostream>
#include <vector>
#include "quadtree.h"



Quadtree::Quadtree(Rectangle boundary){
    // create a new quadtree with defined boundaries
    this->boundary = boundary;
    root = new QuadTreeNode(boundary);
}

void Quadtree::insert(Point p){
    // insert a point into the quadtree from the root
    insert(root, p);
}

void Quadtree::insert(QuadTreeNode* node, Point p){
    // if the point is outside of bounds, return nothing, not inserted
    if(!contains(node->boundary, p)){
        return;
    }
    // if the node has no children, insert the point into the node
    // if nw is null then all other children are null
    if(node->NW == nullptr){
        // if the node has less than 4 points insert the point into the nodes point vector
        node->points.push_back(p);
        // if the node has more than 4 points subdivide the node and insert the points into the correct child
        if(node->points.size() > 4){
            // subdivide the node
            subdivide(node);
            std::vector<Point> oldPoints = node->points;
            // clear the points vector
            node->points = std::vector<Point>();

            // reinsert the old points into the correct child
            for(Point oldP : oldPoints){
                insert(node, oldP);
            }
        }
        return;
    }
    // if the node has children, recursively insert the point into the correct child
    insert(node->NW, p);
    insert(node->NE, p);
    insert(node->SW, p);
    insert(node->SE, p);
}
bool Quadtree::contains(Point p){
    return contains(root->boundary, p);
}

bool Quadtree::contains(Rectangle boundary, Point p){
    // checks if the point is within the boundaries
    if (p.x >= boundary.x && p.x <= boundary.x + boundary.w && p.y >= boundary.y && p.y <= boundary.y + boundary.h){
        return true;
    }
    return false;
}

void Quadtree::subdivide(){
    subdivide(root);
}

void Quadtree::subdivide(QuadTreeNode* node){
    // creates 4 new rectangles that are half the size of the parent rectangle
    float x = node->boundary.x;
    float y = node->boundary.y;
    float w = node->boundary.w;
    float h = node->boundary.h;

    Rectangle NW = {x, y, w/2, h/2};
    Rectangle NE = {x + w/2, y, w/2, h/2};
    Rectangle SW = {x, y + h/2, w/2, h/2};
    Rectangle SE = {x + w/2, y + h/2, w/2, h/2};

    node->NW = new QuadTreeNode(NW);
    node->NE = new QuadTreeNode(NE);
    node->SW = new QuadTreeNode(SW);
    node->SE = new QuadTreeNode(SE);
}


void Quadtree::print(){
    print(root);
}

void Quadtree::print(QuadTreeNode* node){
    // print the boundary and points of the node
    std::cout << "Boundary: " << node->boundary.x << " " << node->boundary.y << " " << node->boundary.w << " " << node->boundary.h << std::endl;
    std::cout << "Points: ";
    for(Point p : node->points){
        std::cout << "(" << p.x << ", " << p.y << ") ";
    }
    std::cout << std::endl;

    if(node->NW == nullptr){
        return;
    }
    // recursively print the children of the node
    print(node->NW);
    print(node->NE);
    print(node->SW);
    print(node->SE);
}

void Quadtree::visualize(){
    // create a grid to visualize the quadtree
    int gridSize = root->boundary.w;
    std::vector<std::vector<std::string>> grid(gridSize, std::vector<std::string>(gridSize, "0"));
    visualize(root, grid);
}

// visualize the quadtree by printing out the points in the quadtree
void Quadtree::visualize(QuadTreeNode* node, std::vector<std::vector<std::string>>& grid) {
    // if the node is null, return nothing
    if (node == nullptr) {
        return;
    }
    // sets all points on the node 
    for (Point p : node->points) {
        int point_y = p.y;
        int point_x = p.x;
        grid[point_y][point_x] = p.c;
    }

    // recursively visualize the children of the node
    visualize(node->NW, grid);
    visualize(node->NE, grid);
    visualize(node->SW, grid);
    visualize(node->SE, grid);

    // print the grid once the entire quadtree has been explored
    if (node == root) {
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
    }
}






