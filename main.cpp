#include <iostream>
#include <vector>
#include <memory>

class Shape {
protected:
    int x, y;
public:
    Shape(int x, int y) : x(x), y(y) {}

    virtual void draw(std::vector<std::vector<char>> &board) const = 0;

    virtual ~Shape() = default;

    std::pair<int, int> getPosition() const {
        return {x, y};
    }

    virtual bool isSameSpot(const Shape &other) const = 0;
};

class Rectangle : public Shape {
private:
    int width, height;

public:
    Rectangle(int x, int y, int w, int h) : Shape(x, y), width(w), height(h) {}

    void draw(std::vector<std::vector<char>> &board) const override {
        int boardHeight = board.size();
        int boardWidth = board[0].size();

        for (int i = x; i < x + width && i < boardWidth; ++i) {
            if (y >= 0 && y < boardHeight) board[y][i] = '#';
            if (y + height - 1 >= 0 && y + height - 1 < boardHeight) board[y + height - 1][i] = '#';
        }

        for (int j = y; j < y + height && j < boardHeight; ++j) {
            if (x >= 0 && x < boardWidth) board[j][x] = '#';
            if (x + width - 1 >= 0 && x + width - 1 < boardWidth) board[j][x + width - 1] = '#';
        }
    }

    bool isSameSpot(const Shape &other) const override {
        const auto *otherRect = dynamic_cast<const Rectangle *>(&other);
        return otherRect && otherRect->x == x && otherRect->y == y && otherRect->width == width &&
               otherRect->height == height;
    }
};

class Blackboard {
private:
    int width;
    int height;
    std::vector<std::vector<char>> board;
    std::vector<std::shared_ptr<Shape>> shapes;

public:
    Blackboard(int w, int h) : width(w), height(h) {
        board.resize(height, std::vector<char>(width, ' '));
    }

    void draw() {
        clearBoard();

        for (const auto &shape: shapes) {
            shape->draw(board);
        }

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                std::cout << board[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void clearBoard() {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                board[i][j] = ' ';
            }
        }
    }

    void addShape(const std::shared_ptr<Shape>& shape) {
        for (const auto &s: shapes) {
            if (s->isSameSpot(*shape)) {
                std::cout << "Shape already exists at the same spot." << std::endl;
                return;
            }
        }

        shapes.push_back(shape);
    }

    void clear() {
        shapes.clear();
    }
};

int main() {
    Blackboard blackboard(20, 10);

    std::shared_ptr<Shape> rect1 = std::make_shared<Rectangle>(1, 1, 8, 6);
    blackboard.addShape(rect1);

    std::shared_ptr<Shape> rect2 = std::make_shared<Rectangle>(3, 4, 7, 5);
    blackboard.addShape(rect2);

    blackboard.draw();

    blackboard.clear();

    blackboard.draw();

    blackboard.addShape(rect2);

    blackboard.draw();

    return 0;
}