#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <iostream>

struct Point{
    // x, y values of a point
    float x;
    float y;
    std::string c;
};
struct Rectangle{
    // x, y, width, height
    float x;
    float y;
    float w;
    float h;
};

struct QuadTreeNode{
    Rectangle boundary;
    std::vector<Point> points;
    QuadTreeNode* NW;
    QuadTreeNode* NE;
    QuadTreeNode* SW;
    QuadTreeNode* SE;

    QuadTreeNode(Rectangle boundary){
        this->boundary = boundary;
        NW = nullptr;
        NE = nullptr;
        SW = nullptr;
        SE = nullptr;
    }    
};
class Quadtree {
    public:
        Quadtree(Rectangle boundary);
        void insert(Point p);
        std::vector<Point> query(Rectangle range);
        void subdivide();
        void print();
        void visualize();
        bool contains(Point p);
        bool search(Point p);
        Rectangle getBoundary();
        QuadTreeNode* root;


    private:
        std::vector<Point> points;
        Rectangle boundary;
        bool contains(Rectangle boundary, Point p);
        void insert(QuadTreeNode* node, Point p);
        bool intersects(Rectangle boundary, Rectangle range);
        void subdivide(QuadTreeNode* node);
        
        bool seach(QuadTreeNode* node, Point p);
        
        void visualize(QuadTreeNode* node, std::vector<std::vector<std::string>>& grid);
        void print(QuadTreeNode* node);

};


#endif