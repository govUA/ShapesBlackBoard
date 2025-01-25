#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <string>
#include <sstream>
#include "Blackboard.h"

class CLI {
private:
    Blackboard &blackboard;

public:
    CLI(Blackboard &b);

    void run();

private:
    void processCommand(const std::string &command);

    void printAvailableShapes() const;

    bool addShape(std::istringstream &iss);

    void printHelp() const;

    void undo();

    int action = 0;
};

#endif