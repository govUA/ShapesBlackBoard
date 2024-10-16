#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <vector>
#include <memory>
#include "Shape.h"

class Blackboard {
private:
    int width, height, nextShapeId, shapeId;
    std::vector<std::vector<char>> board;
    std::vector<std::shared_ptr<Shape>> shapes;

public:
    Blackboard(int w, int h);

    void draw();

    void clearBoard();

    void addShape(const std::shared_ptr<Shape> &shape);

    void removeShape();

    void clear();

    void listShapes() const;

    void undo();

    void save(const std::string &filePath) const;

    void load(const std::string &filePath);

    void editParams(const std::vector<float> &values);

    void editPosition(int x, int y);

    void editColour(char colour);

    void selectId(int shapeId);

    void selectPosition(int x, int y);
};

#endif