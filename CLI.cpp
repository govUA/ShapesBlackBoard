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
    } else if (cmd == "remove") {
        int shapeId;
        iss >> shapeId;
        blackboard.removeShape(shapeId);
    } else if (cmd == "undo") {
        blackboard.undo();
    } else if (cmd == "clear") {
        blackboard.clear();
    } else if (cmd == "edit") {
        int shapeId;
        float value;
        std::vector<float> values;
        iss >> shapeId;
        while (iss >> value) {
            values.push_back(value);
        }
        blackboard.editParams(shapeId, values);
    } else if (cmd == "move") {
        int shapeId, x, y;
        iss >> shapeId >> x >> y;
        blackboard.editPosition(shapeId, x, y);
    } else if (cmd == "paint") {
        int shapeId;
        char colour;
        iss >> shapeId >> colour;
        blackboard.editColour(shapeId, colour);
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
                 "\tdraw                         - Draw blackboard to the console.\n"
                 "\tlist                         - Print all added shapes with their IDs and parameters.\n"
                 "\tshapes                       - Print a list of all available shapes and parameters for add call.\n"
                 "\tadd <shape> <parameters>     - Add shape to the blackboard.\n"
                 "\tundo                         - Remove the last added shape from the blackboard.\n"
                 "\tclear                        - Remove all shapes from the blackboard.\n"
                 "\tedit <shape-id> <parameters> - Edit shape parameters.\n"
                 "\tmove <shape-id> <x> <y>      - Move shape to new coordinates.\n"
                 "\tpaint <shape-id> <colour>    - Paint shape new colour.\n"
                 "\tsave <file-path>             - Save the blackboard to the file.\n"
                 "\tload <file-path>             - Load a blackboard from the file.\n"
                 "\thelp                         - Show this help message.\n"
                 "\texit                         - Exit.\n";
}

void CLI::printAvailableShapes() const {
    std::cout << "Available shapes:\n";
    std::cout << "\trectangle <x> <y> <colour> <fill/frame> <width> <height>\n";
    std::cout << "\tcircle <x> <y> <colour> <fill/frame> <radius>\n";
    std::cout << "\ttriangle <x> <y> <colour> <fill/frame> <height> <width>\n";
    std::cout << "\tline <x> <y> <colour> <length> <angle>\n";
}

void CLI::addShape(std::istringstream &iss) {
    int x, y;
    std::string shapeType, fillOrFrame;
    iss >> shapeType;

    if (shapeType == "rectangle") {
        int width, height;
        char colour;
        bool fillMode;
        iss >> x >> y >> colour >> fillOrFrame >> width >> height;
        if (fillOrFrame == "fill") fillMode += 1;
        blackboard.addShape(std::make_shared<Rectangle>(x, y, colour, fillMode, width, height));
    } else if (shapeType == "circle") {
        int radius;
        char colour;
        bool fillMode;
        iss >> x >> y >> colour >> fillOrFrame >> radius;
        if (fillOrFrame == "fill") fillMode += 1;
        blackboard.addShape(std::make_shared<Circle>(x, y, colour, fillMode, radius));
    } else if (shapeType == "triangle") {
        int height, width;
        char colour;
        bool fillMode;
        iss >> x >> y >> colour >> fillOrFrame >> height >> width;
        if (fillOrFrame == "fill") fillMode += 1;
        blackboard.addShape(std::make_shared<Triangle>(x, y, colour, fillMode, height, width));
    } else if (shapeType == "line") {
        int length;
        double angle;
        char colour;
        bool fillMode;
        iss >> x >> y >> colour >> length >> angle;
        blackboard.addShape(std::make_shared<Line>(x, y, colour, fillMode, length, angle));
    } else {
        std::cout << "Unknown shape type: " << shapeType << std::endl;
    }
}
