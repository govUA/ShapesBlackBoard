#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <vector>
#include <memory>
#include <windows.h>
#include "Shape.h"

class Blackboard {
private:
    int width, height, nextShapeId, shapeId;
    std::vector<std::vector<char>> board;
    std::vector<std::shared_ptr<Shape>> shapes;

    enum Colour {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        RED = 4,
        YELLOW = 6,
        WHITE = 7
    };

    Colour getCharColour(char colourChar) {
        switch (colourChar) {
            case 'k':
                return BLACK;
            case 'b':
                return BLUE;
            case 'g':
                return GREEN;
            case 'r':
                return RED;
            case 'y':
                return YELLOW;
            default:
                return WHITE;
        }
    }

    void setConsoleColour(Colour colour) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, colour);
    }

    std::vector<std::vector<std::shared_ptr<Shape>>> undoStack = {};

public:
    Blackboard(int w, int h);

    void draw();

    void clearBoard();

    bool addShape(const std::shared_ptr<Shape> &shape);

    bool removeShape();

    bool clear();

    void listShapes() const;

    bool save(const std::string &filePath) const;

    bool load(const std::string &filePath);

    bool editParams(const std::vector<float> &values);

    bool editPosition(int x, int y);

    bool editColour(char colour);

    void selectId(int shapeId);

    void selectPosition(int x, int y);
};

#endif