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
    if (!shape->isWithinBounds(width, height)) {
        std::cout << "Shape cannot be placed outside the board or is too large for the board." << std::endl;
        return;
    }
    for (const auto &s: shapes) {
        if (s->isSameSpot(*shape)) {
            std::cout << "Shape already exists at the same spot." << std::endl;
            return;
        }
    }

    shape->setId(nextShapeId++);
    shapes.push_back(shape);
}

void Blackboard::clear() {
    shapes.clear();
}

void Blackboard::listShapes() const {
    std::cout << "Shapes on the blackboard:\n";
    for (const auto &shape: shapes) {
        std::cout << "\tID: " << shape->getId() << ", Type: " << shape->getType()
                  << ", Position: (" << shape->getPosition().first
                  << ", " << shape->getPosition().second << "), ";

        if (shape->getType() == "Rectangle") {
            const auto *rect = dynamic_cast<const Rectangle *>(shape.get());
            std::cout << "Width: " << rect->getWidth() << ", Height: " << rect->getHeight();
        } else if (shape->getType() == "Circle") {
            const auto *circ = dynamic_cast<const Circle *>(shape.get());
            std::cout << "Radius: " << circ->getRadius();
        } else if (shape->getType() == "Triangle") {
            const auto *tri = dynamic_cast<const Triangle *>(shape.get());
            std::cout << "Height: " << tri->getHeight() << ", Width: " << tri->getWidth();
        } else if (shape->getType() == "Line") {
            const auto *line = dynamic_cast<const Line *>(shape.get());
            std::cout << "Length: " << line->getLength() << ", Angle: " << line->getAngle();
        }
        std::cout << std::endl;
    }
}

void Blackboard::undo() {
    if (!shapes.empty()) {
        shapes.pop_back();
        std::cout << "Last shape removed.\n";
    } else {
        std::cout << "No shapes to remove.\n";
    }
}

void Blackboard::save(const std::string &filePath) const {
    try {
        RaiiWrapper file(filePath, true);

        for (const auto &shape: shapes) {
            file.getOutputStream() << shape->getType() << ' ' << shape->getPosition().first << ' '
                                   << shape->getPosition().second;

            if (shape->getType() == "Rectangle") {
                const auto *rect = dynamic_cast<const Rectangle *>(shape.get());
                file.getOutputStream() << ' ' << rect->getWidth() << ' ' << rect->getHeight() << '\n';
            } else if (shape->getType() == "Circle") {
                const auto *circ = dynamic_cast<const Circle *>(shape.get());
                file.getOutputStream() << ' ' << circ->getRadius() << '\n';
            } else if (shape->getType() == "Triangle") {
                const auto *tri = dynamic_cast<const Triangle *>(shape.get());
                file.getOutputStream() << ' ' << tri->getHeight() << ' ' << tri->getWidth() << '\n';
            } else if (shape->getType() == "Line") {
                const auto *line = dynamic_cast<const Line *>(shape.get());
                file.getOutputStream() << ' ' << line->getLength() << ' ' << line->getAngle() << '\n';
            }
        }

        std::cout << "Blackboard saved to " << filePath << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Blackboard::load(const std::string &filePath) {
    std::vector<std::shared_ptr<Shape>> loadedShapes;
    try {
        RaiiWrapper file(filePath, false);

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
                int width, height;
                file.getInputStream() >> width >> height;
                if (width <= 0 || height <= 0) {
                    throw std::runtime_error("Invalid dimensions for Rectangle.");
                }
                auto rect = std::make_shared<Rectangle>(x, y, colour, fillMode, width, height);
                if (!rect->isWithinBounds(this->width, this->height)) {
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
                if (!circ->isWithinBounds(this->width, this->height)) {
                    throw std::runtime_error("Circle out of bounds.");
                }
                loadedShapes.push_back(circ);
            } else if (shapeType == "Triangle") {
                int height, width;
                file.getInputStream() >> height >> width;
                if (height <= 0 || width <= 0) {
                    throw std::runtime_error("Invalid dimensions for Triangle.");
                }
                auto tri = std::make_shared<Triangle>(x, y, colour, fillMode, height, width);
                if (!tri->isWithinBounds(this->width, this->height)) {
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
                if (!line->isWithinBounds(this->width, this->height)) {
                    throw std::runtime_error("Line out of bounds.");
                }
                loadedShapes.push_back(line);
            } else {
                throw std::runtime_error("Unknown shape type: " + shapeType);
            }
        }
        clear();
        shapes = std::move(loadedShapes);
        std::cout << "Blackboard loaded from " << filePath << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Failed to load blackboard: " << e.what() << std::endl;
    }
}

void Blackboard::removeShape(int shapeId) {
    if (shapeId < 0 || shapeId >= shapes.size()) {
        std::cout << "Invalid shape ID!" << std::endl;
        return;
    }
    shapes.erase(shapes.begin() + shapeId);
    std::cout << "Shape removed successfully." << std::endl;
}