#include <fstream>
#include "Blackboard.h"
#include "RaiiWrapper.h"

Blackboard::Blackboard(int w, int h) : width(w), height(h), nextShapeId(0) {
    board.resize(height, std::vector<char>(width, ' '));
}

void Blackboard::draw() {
    clearBoard();

    for (const auto &shape: shapes) {
        shape->draw(board);
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            char symbol = board[i][j];
            if (symbol != ' ') {
                Colour colour = getCharColour(symbol);
                setConsoleColour(colour);
                std::cout << board[i][j] << ' ';
                setConsoleColour(WHITE);
            } else {
                std::cout << "  ";
            }
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

bool Blackboard::addShape(const std::shared_ptr<Shape> &shape) {
    if (!shape->isWithinBounds(width, height)) {
        std::cout << "Shape cannot be placed outside the board or is too large for the board." << std::endl;
        return false;
    }
    for (const auto &s: shapes) {
        if (s->isSameSpot(*shape)) {
            std::cout << "Shape already exists at the same spot." << std::endl;
            return false;
        }
    }

    undoStack.push_back(shapes);
    shapes.push_back(shape);
    return true;
}

bool Blackboard::clear() {
    undoStack.push_back(shapes);
    shapes.clear();
    return true;
}

void Blackboard::listShapes() const {
    std::cout << "Shapes on the blackboard:\n";
    for (size_t i = 0; i < shapes.size(); ++i) {
        const auto &shape = shapes[i];
        std::cout << "\tID: " << i << ", Type: " << shape->getType()
                  << ", Position: (" << shape->getPosition().first
                  << ", " << shape->getPosition().second << "), "
                  << shape->describe() << std::endl;
    }
}

bool Blackboard::save(const std::string &filePath) const {
    try {
        RaiiWrapper file(filePath, true);

        file.getOutputStream() << width << ' ' << height << '\n';

        for (const auto &shape : shapes) {
            shape->serialize(file.getOutputStream());
        }
        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool Blackboard::load(const std::string &filePath) {
    std::vector<std::shared_ptr<Shape>> loadedShapes;
    try {
        RaiiWrapper file(filePath, false);

        int newWidth, newHeight;
        file.getInputStream() >> newWidth >> newHeight;

        if (newWidth <= 0 || newHeight <= 0) {
            throw std::runtime_error("Invalid board dimensions.");
        }

        width = newWidth;
        height = newHeight;

        board.resize(height);
        for (auto &row: board) {
            row.resize(width);
        }

        clear();

        std::string shapeType;
        while (file.getInputStream() >> shapeType) {
            int x, y;
            char colour;
            bool fillMode;
            file.getInputStream() >> x >> y >> colour >> fillMode;

            if (x < 0 || y < 0 || x >= width || y >= height) {
                throw std::runtime_error("Invalid position for shape.");
            }

            if (shapeType == "Rectangle") {
                int w, h;
                file.getInputStream() >> w >> h;
                if (w <= 0 || h <= 0) {
                    throw std::runtime_error("Invalid dimensions for Rectangle.");
                }
                auto rect = std::make_shared<SRectangle>(x, y, colour, fillMode, w, h);
                if (!rect->isWithinBounds(width, height)) {
                    throw std::runtime_error("Rectangle out of bounds.");
                }
                loadedShapes.push_back(rect);
            } else if (shapeType == "Circle") {
                int radius;
                file.getInputStream() >> radius;
                if (radius <= 0) {
                    throw std::runtime_error("Invalid radius for Circle.");
                }
                auto circ = std::make_shared<Circle>(x, y, colour, fillMode, radius);
                if (!circ->isWithinBounds(width, height)) {
                    throw std::runtime_error("Circle out of bounds.");
                }
                loadedShapes.push_back(circ);
            } else if (shapeType == "Triangle") {
                int h, w;
                file.getInputStream() >> h >> w;
                if (h <= 0 || w <= 0) {
                    throw std::runtime_error("Invalid dimensions for Triangle.");
                }
                auto tri = std::make_shared<Triangle>(x, y, colour, fillMode, h, w);
                if (!tri->isWithinBounds(width, height)) {
                    throw std::runtime_error("Triangle out of bounds.");
                }
                loadedShapes.push_back(tri);
            } else if (shapeType == "Line") {
                int length;
                double angle;
                file.getInputStream() >> length >> angle;
                if (length <= 0) {
                    throw std::runtime_error("Invalid length for Line.");
                }
                auto line = std::make_shared<Line>(x, y, colour, fillMode, length, angle);
                if (!line->isWithinBounds(width, height)) {
                    throw std::runtime_error("Line out of bounds.");
                }
                loadedShapes.push_back(line);
            } else {
                throw std::runtime_error("Unknown shape type: " + shapeType);
            }
        }
        undoStack.push_back(shapes);
        clear();
        shapes = std::move(loadedShapes);
        return true;
    } catch (const std::exception &e) {
        std::cerr << "Failed to load blackboard: " << e.what() << std::endl;
        return false;
    }
}

bool Blackboard::removeShape() {
    if (shapeId < 0 || shapeId >= shapes.size()) {
        std::cout << "Invalid shape ID!" << std::endl;
        return false;
    }
    undoStack.push_back(shapes);
    shapes.erase(shapes.begin() + shapeId);
    std::cout << "Shape removed successfully." << std::endl;
    return true;
}

bool Blackboard::editParams(const std::vector<float> &values) {
    if (shapeId < 0 || shapeId >= shapes.size()) {
        std::cout << "Invalid shape ID!" << std::endl;
        return false;
    }
    undoStack.push_back(shapes);
    shapes[shapeId]->editSize(values);
    return true;
}

bool Blackboard::editPosition(int x, int y) {
    if (shapeId < 0 || shapeId >= shapes.size()) {
        std::cout << "Invalid shape ID!" << std::endl;
        return false;
    }
    if (x >= 0 && y >= 0 && x < width && y < height) {
        undoStack.push_back(shapes);
        shapes[shapeId]->editPosition(x, y);
        std::cout << "Shape #" << shapeId << " moved to (" << x << ", " << y << ") successfully." << std::endl;
        return true;
    }
    std::cout << "Position out of bounds." << std::endl;
    return false;
}

bool Blackboard::editColour(char colour) {
    if (shapeId < 0 || shapeId >= shapes.size()) {
        std::cout << "Invalid shape ID!" << std::endl;
        return false;
    }
    undoStack.push_back(shapes);
    shapes[shapeId]->editColour(colour);
    return true;
}

void Blackboard::selectId(int id) {
    if (id >= 0 && id < shapes.size()) {
        shapeId = id;
        std::cout << "Shape #" << id << " selected.\n";
    } else {
        std::cout << "Invalid shape index!" << std::endl;
    }
}

void Blackboard::selectPosition(int x, int y) {
    for (size_t i = 0; i < shapes.size(); ++i) {
        const auto &shape = shapes[shapes.size() - i - 1];
        if (shape->coversPoint(board, x, y)) {
            shapeId = shapes.size() - i - 1;
            std::cout << "Shape detected at (" << x << ", " << y << ")." << std::endl;
            return;
        } else {
            shapeId = -1;
        }
    }
    std::cout << "No shape detected at (" << x << ", " << y << ")." << std::endl;
}
