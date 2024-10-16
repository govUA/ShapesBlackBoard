#include "Shape.h"

Shape::Shape(int x, int y, char colour, bool fillMode) : x(x), y(y), colour(colour), fillMode(fillMode) {}

std::pair<int, int> Shape::getPosition() const {
    return {x, y};
}

Rectangle::Rectangle(int x, int y, char colour, bool fillMode, int w, int h) : Shape(x, y, colour, fillMode), width(w),
                                                                               height(h) {}

void Rectangle::editSize(std::vector<float> sizes) {
    if (sizes.size() == 2) {
        this->width = int(sizes[0]);
        this->height = int(sizes[1]);
    } else {
        std::cout << "Rectangle requires 2 size parameters (width and height)." << std::endl;
    }
}

void Rectangle::draw(std::vector<std::vector<char>> &board) const {
    int boardHeight = board.size();
    int boardWidth = board[0].size();

    char symbol = getColour();

    if (getFillMode()) {
        for (int j = y; j < y + height && j < boardHeight; ++j) {
            for (int i = x; i < x + width && i < boardWidth; ++i) {
                if (i >= 0 && j >= 0) {
                    board[j][i] = symbol;
                }
            }
        }
    } else {
        for (int i = x; i < x + width && i < boardWidth; ++i) {
            if (y >= 0 && y < boardHeight) board[y][i] = symbol;
            if (y + height - 1 >= 0 && y + height - 1 < boardHeight) board[y + height - 1][i] = symbol;
        }
        for (int j = y; j < y + height && j < boardHeight; ++j) {
            if (x >= 0 && x < boardWidth) board[j][x] = symbol;
            if (x + width - 1 >= 0 && x + width - 1 < boardWidth) board[j][x + width - 1] = symbol;
        }
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

int Rectangle::getWidth() const {
    return width;
}

int Rectangle::getHeight() const {
    return height;
}

bool Rectangle::isWithinBounds(int boardWidth, int boardHeight) const {
    return x >= 0 && y >= 0 && x < boardWidth && y < boardHeight && width <= boardWidth && height <= boardHeight;
}

Circle::Circle(int x, int y, char colour, bool fillMode, int r) : Shape(x, y, colour, fillMode), radius(r) {}

void Circle::editSize(std::vector<float> sizes) {
    if (sizes.size() == 1) {
        this->radius = int(sizes[0]);
    } else {
        std::cout << "Circle requires 1 size parameter (radius)." << std::endl;
    }
}

void Circle::draw(std::vector<std::vector<char>> &board) const {
    int boardHeight = board.size();
    int boardWidth = board[0].size();

    char symbol = getColour();

    for (int i = -radius; i <= radius; ++i) {
        for (int j = -radius; j <= radius; ++j) {
            int dist = i * i + j * j;
            if ((getFillMode() && dist <= radius * radius) ||
                (!getFillMode() && dist >= radius * radius - radius && dist <= radius * radius + radius)) {
                int drawX = x + j;
                int drawY = y + i;
                if (drawX >= 0 && drawX < boardWidth && drawY >= 0 && drawY < boardHeight) {
                    board[drawY][drawX] = symbol;
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

int Circle::getRadius() const {
    return radius;
}

bool Circle::isWithinBounds(int boardWidth, int boardHeight) const {
    return x >= 0 && y >= 0 && x < boardWidth && y < boardHeight &&
           radius <= (sqrt(pow(boardHeight, 2) + pow(boardWidth, 2)));
}

Triangle::Triangle(int x, int y, char colour, bool fillMode, int h, int w) : Shape(x, y, colour, fillMode), height(h),
                                                                             width(w) {}

void Triangle::editSize(std::vector<float> sizes) {
    if (sizes.size() == 2) {
        this->width = int(sizes[0]);
        this->height = int(sizes[1]);
    } else {
        std::cout << "Triangle requires 2 size parameters (width and height)." << std::endl;
    }
}

void Triangle::draw(std::vector<std::vector<char>> &board) const {
    int boardHeight = board.size();
    int boardWidth = board[0].size();

    char symbol = getColour();

    if (getFillMode()) {
        for (int i = 0; i < height; ++i) {
            int leftX = x - (i * width / height) / 2;
            int rightX = x + (i * width / height) / 2;
            int drawY = y + i;

            if (drawY >= 0 && drawY < boardHeight) {
                for (int j = leftX; j <= rightX && j < boardWidth; ++j) {
                    if (j >= 0) board[drawY][j] = symbol;
                }
            }
        }
    } else {
        for (int i = 0; i < height; ++i) {
            int leftX = x - (i * width / height) / 2;
            int rightX = x + (i * width / height) / 2;
            int drawY = y + i;

            if (drawY >= 0 && drawY < boardHeight) {
                if (leftX >= 0 && leftX < boardWidth) board[drawY][leftX] = symbol;
                if (rightX >= 0 && rightX < boardWidth) board[drawY][rightX] = symbol;
            }
        }
        for (int j = x - width / 2; j <= x + width / 2 && j < boardWidth; ++j) {
            if (y + height - 1 >= 0 && y + height - 1 < boardHeight) {
                board[y + height - 1][j] = symbol;
            }
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

int Triangle::getHeight() const {
    return height;
}

int Triangle::getWidth() const {
    return width;
}

bool Triangle::isWithinBounds(int boardWidth, int boardHeight) const {
    return x >= 0 && y >= 0 && x < boardWidth && y < boardHeight && width <= boardWidth && height <= boardHeight;
}

Line::Line(int x, int y, char colour, bool fillMode, int l, double a) : Shape(x, y, colour, fillMode), length(l),
                                                                        angle(a) {}

void Line::editSize(std::vector<float> sizes) {
    if (sizes.size() == 1) {
        this->length = int(sizes[0]);
        this->angle = sizes[1];
    } else {
        std::cout << "Triangle requires 2 size parameters (length and angle)." << std::endl;
    }
}

void Line::draw(std::vector<std::vector<char>> &board) const {
    int boardHeight = board.size();
    int boardWidth = board[0].size();

    char symbol = getColour();

    double radAngle = angle * M_PI / 180.0;

    for (int i = 0; i < length; ++i) {
        int drawX = x + static_cast<int>(i * cos(radAngle));
        int drawY = y + static_cast<int>(i * sin(radAngle));

        if (drawX >= 0 && drawX < boardWidth && drawY >= 0 && drawY < boardHeight) {
            board[drawY][drawX] = symbol;
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

double Line::getAngle() const {
    return angle;
}

int Line::getLength() const {
    return length;
}

bool Line::isWithinBounds(int boardWidth, int boardHeight) const {

    return x >= 0 && y >= 0 && x < boardWidth && y < boardHeight &&
           length <= (sqrt(pow(boardHeight, 2) + pow(boardWidth, 2)));
}