#include "Blackboard.h"

Blackboard::Blackboard(int w, int h) : width(w), height(h) {
    board.resize(height, std::vector<char>(width, ' '));
}

void Blackboard::draw() {
    clearBoard();

    for (const auto &shape: shapes) {
        shape->draw(board);
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << board[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

void Blackboard::clearBoard() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            board[i][j] = ' ';
        }
    }
}

void Blackboard::addShape(const std::shared_ptr<Shape> &shape) {
    for (const auto &s: shapes) {
        if (s->isSameSpot(*shape)) {
            std::cout << "Shape already exists at the same spot." << std::endl;
            return;
        }
    }

    shapes.push_back(shape);
}

void Blackboard::clear() {
    shapes.clear();
}
