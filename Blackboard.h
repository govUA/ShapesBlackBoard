#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <vector>
#include <memory>
#include "Shape.h"

class Blackboard {
private:
    int width;
    int height;
    std::vector<std::vector<char>> board;
    std::vector<std::shared_ptr<Shape>> shapes;

public:
    Blackboard(int w, int h);

    void draw();

    void clearBoard();

    void addShape(const std::shared_ptr<Shape> &shape);

    void clear();

    void listShapes() const;

    void undo();

    void save(const std::string &filePath) const;

    void load(const std::string &filePath);
};

#endif