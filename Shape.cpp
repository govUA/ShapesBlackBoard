#include "Shape.h"

Shape::Shape(int x, int y) : x(x), y(y) {}

std::pair<int, int> Shape::getPosition() const {
    return {x, y};
}

Rectangle::Rectangle(int x, int y, int w, int h) : Shape(x, y), width(w), height(h) {}

void Rectangle::draw(std::vector<std::vector<char>> &board) const {
    int boardHeight = board.size();
    int boardWidth = board[0].size();

    for (
            int i = x;
            i < x +
                width && i < boardWidth;
            ++i) {
        if (y >= 0 && y < boardHeight) board[y][i] = '#';
        if (y + height - 1 >= 0 && y + height - 1 < boardHeight) board[y + height - 1][i] = '#';
    }

    for (
            int j = y;
            j < y +
                height && j < boardHeight;
            ++j) {
        if (x >= 0 && x < boardWidth) board[j][x] = '#';
        if (x + width - 1 >= 0 && x + width - 1 < boardWidth) board[j][x + width - 1] = '#';
    }
}

bool Rectangle::isSameSpot(const Shape &other) const {
    const auto *otherRect = dynamic_cast<const Rectangle *>(&other);
    return otherRect && otherRect->x == x && otherRect->y == y && otherRect->width == width &&
           otherRect->height == height;
}

std::string Rectangle::getType() const {
    return "Rectangle";
}

Circle::Circle(int x, int y, int r) : Shape(x, y), radius(r) {}


void Circle::draw(std::vector<std::vector<char>> &board) const {
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

bool Circle::isSameSpot(const Shape &other) const {
    const Circle *otherCircle = dynamic_cast<const Circle *>(&other);
    return otherCircle && otherCircle->x == x && otherCircle->y == y && otherCircle->radius == radius;
}

std::string Circle::getType() const {
    return "Circle";
}


Triangle::Triangle(int x, int y, int h, int w) : Shape(x, y), height(h), width(w) {}

void Triangle::draw(std::vector<std::vector<char>> &board) const {
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

bool Triangle::isSameSpot(const Shape &other) const {
    const Triangle *otherTriangle = dynamic_cast<const Triangle *>(&other);
    return otherTriangle && otherTriangle->x == x && otherTriangle->y == y && otherTriangle->height == height;
}

std::string Triangle::getType() const {
    return "Triangle";
}

Line::Line(int x, int y, int l, double a) : Shape(x, y), length(l), angle(a) {}

void Line::draw(std::vector<std::vector<char>> &board) const {
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

bool Line::isSameSpot(const Shape &other) const {
    const Line *otherLine = dynamic_cast<const Line *>(&other);
    return otherLine && otherLine->x == x && otherLine->y == y && otherLine->length == length;
}

std::string Line::getType() const {
    return "Line";
}