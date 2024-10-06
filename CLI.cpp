#include "CLI.h"

CLI::CLI(Blackboard &b) : blackboard(b) {};

void CLI::run() {
    std::string command;
    printHelp();
    while (true) {
        std::cout << ">";
        std::getline(std::cin, command);
        if (command.empty()) continue;
        if (command == "exit") break;
        processCommand(command);
    }
}

void CLI::processCommand(const std::string &command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "draw") {
        blackboard.draw();
    } else if (cmd == "list") {
        blackboard.listShapes();
    } else if (cmd == "shapes") {
        printAvailableShapes();
    } else if (cmd == "add") {
        addShape(iss);
    } else if (cmd == "undo") {
        blackboard.undo();
    } else if (cmd == "clear") {
        blackboard.clear();
    } else if (cmd == "save") {
        std::string filePath;
        iss >> filePath;
        blackboard.save(filePath);
    } else if (cmd == "load") {
        std::string filePath;
        iss >> filePath;
        blackboard.load(filePath);
    } else if (cmd == "help") {
        printHelp();
    } else {
        std::cout << "Unknown command: " << cmd << std::endl;
    }
}

void CLI::printHelp() const {
    std::cout << "Available commands:\n"
                 "\tdraw                     - Draw blackboard to the console.\n"
                 "\tlist                     - Print all added shapes with their IDs and parameters.\n"
                 "\tshapes                   - Print a list of all available shapes and parameters for add call.\n"
                 "\tadd <shape> <parameters> - Add shape to the blackboard.\n"
                 "\tundo                     - Remove the last added shape from the blackboard.\n"
                 "\tclear                    - Remove all shapes from the blackboard.\n"
                 "\tsave <file-path>         - Save the blackboard to the file.\n"
                 "\tload <file-path>         - Load a blackboard from the file.\n"
                 "\thelp                     - Show this help message.\n"
                 "\texit                     - Exit.\n";
}

void CLI::printAvailableShapes() const {
    std::cout << "Available shapes:\n";
    std::cout << "\trectangle <x> <y> <width> <height>\n";
    std::cout << "\tcircle <x> <y> <radius>\n";
    std::cout << "\ttriangle <x> <y> <height> <width>\n";
    std::cout << "\tline <x> <y> <length> <angle>\n";
}

void CLI::addShape(std::istringstream &iss) {
    std::string shapeType;
    iss >> shapeType;

    if (shapeType == "rectangle") {
        int x, y, width, height;
        iss >> x >> y >> width >> height;
        blackboard.addShape(std::make_shared<Rectangle>(x, y, width, height));
    } else if (shapeType == "circle") {
        int x, y, radius;
        iss >> x >> y >> radius;
        blackboard.addShape(std::make_shared<Circle>(x, y, radius));
    } else if (shapeType == "triangle") {
        int x, y, height, width;
        iss >> x >> y >> height >> width;
        blackboard.addShape(std::make_shared<Triangle>(x, y, height, width));
    } else if (shapeType == "line") {
        int x, y, length;
        double angle;
        iss >> x >> y >> length >> angle;
        blackboard.addShape(std::make_shared<Line>(x, y, length, angle));
    } else {
        std::cout << "Unknown shape type: " << shapeType << std::endl;
    }
}
