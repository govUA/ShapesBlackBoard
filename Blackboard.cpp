#include <fstream>
#include "Blackboard.h"
#include "RaiiWrapper.h"

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

void Blackboard::listShapes() const {
    std::cout << "Shapes on the blackboard:\n";
    for (size_t i = 0; i < shapes.size(); ++i) {
        const auto &shape = shapes[i];
        std::cout << "ID: " << i << ", Type: " << shape->getType()
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

        std::cout << '\n';
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

            // Add shape-specific parameters
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
    try {
        RaiiWrapper file(filePath, false);

        clear();

        std::string shapeType;
        while (file.getInputStream() >> shapeType) {
            int x, y;
            file.getInputStream() >> x >> y;

            if (shapeType == "Rectangle") {
                int width, height;
                file.getInputStream() >> width >> height;
                addShape(std::make_shared<Rectangle>(x, y, width, height));
            } else if (shapeType == "Circle") {
                int radius;
                file.getInputStream() >> radius;
                addShape(std::make_shared<Circle>(x, y, radius));
            } else if (shapeType == "Triangle") {
                int height, width;
                file.getInputStream() >> height >> width;
                addShape(std::make_shared<Triangle>(x, y, height, width));
            } else if (shapeType == "Line") {
                int length;
                double angle;
                file.getInputStream() >> length >> angle;
                addShape(std::make_shared<Line>(x, y, length, angle));
            } else {
                std::cerr << "Unknown shape type in file: " << shapeType << std::endl;
            }
        }

        std::cout << "Blackboard loaded from " << filePath << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}