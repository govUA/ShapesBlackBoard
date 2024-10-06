#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

class Shape {
protected:
    int x, y;
public:
    Shape(int x, int y) : x(x), y(y) {}

    virtual void draw(std::vector<std::vector<char>> &board) const = 0;

    virtual ~Shape() = default;

    virtual bool isSameSpot(const Shape &other) const = 0;

    virtual std::string getType() const = 0;

    std::pair<int, int> getPosition() const {
        return {x, y};
    }
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

    std::string getType() const override {
        return "Rectangle";
    }
};

class Circle : public Shape {
private:
    int radius;

public:
    Circle(int x, int y, int r) : Shape(x, y), radius(r) {}

    void draw(std::vector<std::vector<char>> &board) const override {
        int boardHeight = board.size();
        int boardWidth = board[0].size();

        for (int i = -radius; i <= radius; ++i) {
            for (int j = -radius; j <= radius; ++j) {
                int dist = i * i + j * j;
                if (dist >= radius * radius - radius && dist <= radius * radius + radius) {
                    int drawX = x + j;
                    int drawY = y + i;
                    if (drawX >= 0 && drawX < boardWidth && drawY >= 0 && drawY < boardHeight) {
                        board[drawY][drawX] = '#';
                    }
                }
            }
        }
    }

    bool isSameSpot(const Shape &other) const override {
        const Circle *otherCircle = dynamic_cast<const Circle *>(&other);
        return otherCircle && otherCircle->x == x && otherCircle->y == y && otherCircle->radius == radius;
    }

    std::string getType() const override {
        return "Circle";
    }
};

class Triangle : public Shape {
private:
    int height;
    int width;

public:
    Triangle(int x, int y, int h, int w) : Shape(x, y), height(h), width(w) {}

    void draw(std::vector<std::vector<char>> &board) const override {
        int boardHeight = board.size();
        int boardWidth = board[0].size();

        for (int i = 0; i < height; ++i) {
            int leftX = x - (i * width / height) / 2;
            int rightX = x + (i * width / height) / 2;
            int drawY = y + i;

            if (drawY >= 0 && drawY < boardHeight) {
                if (leftX >= 0 && leftX < boardWidth) board[drawY][leftX] = '#';
                if (rightX >= 0 && rightX < boardWidth) board[drawY][rightX] = '#';
            }
        }

        for (int j = x - width / 2; j <= x + width / 2 && j < boardWidth; ++j) {
            if (y + height - 1 >= 0 && y + height - 1 < boardHeight) {
                board[y + height - 1][j] = '#';
            }
        }
    }

    bool isSameSpot(const Shape &other) const override {
        const Triangle *otherTriangle = dynamic_cast<const Triangle *>(&other);
        return otherTriangle && otherTriangle->x == x && otherTriangle->y == y && otherTriangle->height == height;
    }

    std::string getType() const override {
        return "Triangle";
    }
};

class Line : public Shape {
private:
    int length;
    double angle;

public:
    Line(int x, int y, int l, double a) : Shape(x, y), length(l), angle(a) {}

    void draw(std::vector<std::vector<char>> &board) const override {
        int boardHeight = board.size();
        int boardWidth = board[0].size();

        double radAngle = angle * M_PI / 180.0;

        for (int i = 0; i < length; ++i) {
            int drawX = x + static_cast<int>(i * cos(radAngle));
            int drawY = y + static_cast<int>(i * sin(radAngle));

            if (drawX >= 0 && drawX < boardWidth && drawY >= 0 && drawY < boardHeight) {
                board[drawY][drawX] = '#';
            }
        }
    }

    bool isSameSpot(const Shape &other) const override {
        const Line *otherLine = dynamic_cast<const Line *>(&other);
        return otherLine && otherLine->x == x && otherLine->y == y && otherLine->length == length;
    }

    std::string getType() const override {
        return "Line";
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
                std::cout << board[i][j] << ' ';
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

    void addShape(const std::shared_ptr<Shape> &shape) {
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
    Blackboard blackboard(58, 32);

    std::shared_ptr<Shape> rect1 = std::make_shared<Rectangle>(1, 1, 9, 7);
    blackboard.addShape(rect1);

    std::shared_ptr<Shape> circ1 = std::make_shared<Circle>(9, 9, 7);
    blackboard.addShape(circ1);

    std::shared_ptr<Triangle> trig1 = std::make_shared<Triangle>(22, 1, 8, 15);
    blackboard.addShape(trig1);

    std::shared_ptr<Line> line1 = std::make_shared<Line>(7, 5, 18, 5);
    blackboard.addShape(line1);

    blackboard.draw();

    return 0;
}