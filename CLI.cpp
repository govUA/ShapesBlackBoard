#include "CLI.h"

CLI::CLI(Blackboard &b) : blackboard(b) {};

void CLI::run() {
    std::string command;
    printHelp();
    blackboard.save("temp" + std::to_string(action));
    while (true) {
        std::cout << ">";
        std::getline(std::cin, command);
        if (command.empty()) continue;
        if (command == "exit") break;
        processCommand(command);
    }
}

void CLI::processCommand(const std::string &command) {
    bool change = false;
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
        change = addShape(iss);
    } else if (cmd == "remove") {
        int shapeId;
        iss >> shapeId;
        change = blackboard.removeShape();
    } else if (cmd == "undo") {
        if (action > 0) {
            action--;
            blackboard.load("temp"+std::to_string(action));
            std::cout << "Reverted previous change." << std::endl;
        } else std::cout<<"No more changes to revert."<<std::endl;
    } else if (cmd == "clear") {
        change = blackboard.clear();
    } else if (cmd == "select") {
        std::vector<int> params;
        int param;
        while (iss >> param) {
            params.push_back(param);
        }
        if (params.size() == 1) {
            blackboard.selectId(params[0]);
        } else if (params.size() == 2) {
            blackboard.selectPosition(params[0], params[1]);
        } else {
            std::cout << "Invalid amount of arguments." << std::endl;
        }
    } else if (cmd == "edit") {
        float value;
        std::vector<float> values;
        while (iss >> value) {
            values.push_back(value);
        }
        change = blackboard.editParams(values);
    } else if (cmd == "move") {
        int x, y;
        iss >> x >> y;
        change = blackboard.editPosition(x, y);
    } else if (cmd == "paint") {
        char colour;
        iss >> colour;
        change = blackboard.editColour(colour);
    } else if (cmd == "save") {
        std::string filePath;
        iss >> filePath;
        if (blackboard.save(filePath)) std::cout << "Blackboard saved to " << filePath << std::endl;
    } else if (cmd == "load") {
        std::string filePath;
        iss >> filePath;
        change = blackboard.load(filePath);
        if (change) std::cout << "Blackboard loaded from " << filePath << std::endl;
    } else if (cmd == "help") {
        printHelp();
    } else {
        std::cout << "Unknown command: " << cmd << std::endl;
    }
    if (change) {
        action++;
        blackboard.save("temp" + std::to_string(action));
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
                 "\tselect <id|position>         - Select shape by id or position.\n"
                 "\tedit <parameters>            - Edit shape parameters.\n"
                 "\tmove <x> <y>                 - Move shape to new coordinates.\n"
                 "\tpaint <colour>               - Paint shape new colour.\n"
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

bool CLI::addShape(std::istringstream &iss) {
    int x, y;
    std::string shapeType, fillOrFrame;
    iss >> shapeType;

    if (shapeType == "rectangle") {
        int width, height;
        char colour;
        iss >> x >> y >> colour >> fillOrFrame >> width >> height;
        bool fillMode = (fillOrFrame == "fill") ? 1 : 0;
        blackboard.addShape(std::make_shared<SRectangle>(x, y, colour, fillMode, width, height));
        return true;
    } else if (shapeType == "circle") {
        int radius;
        char colour;
        iss >> x >> y >> colour >> fillOrFrame >> radius;
        bool fillMode = (fillOrFrame == "fill") ? 1 : 0;
        blackboard.addShape(std::make_shared<Circle>(x, y, colour, fillMode, radius));
        return true;
    } else if (shapeType == "triangle") {
        int height, width;
        char colour;
        iss >> x >> y >> colour >> fillOrFrame >> height >> width;
        bool fillMode = (fillOrFrame == "fill") ? 1 : 0;
        blackboard.addShape(std::make_shared<Triangle>(x, y, colour, fillMode, height, width));
        return true;
    } else if (shapeType == "line") {
        int length;
        double angle;
        char colour;
        bool fillMode;
        iss >> x >> y >> colour >> length >> angle;
        blackboard.addShape(std::make_shared<Line>(x, y, colour, fillMode, length, angle));
        return true;
    }
    std::cout << "Unknown shape type: " << shapeType << std::endl;
    return false;
}
