#ifndef RAIIWRAPPER_H
#define RAIIWRAPPER_H

#include <fstream>
#include <iostream>
#include <string>

class RaiiWrapper {
private:
    std::ofstream outFile;
    std::ifstream inFile;

public:
    RaiiWrapper(const std::string &filePath, bool isOutput);

    ~RaiiWrapper();

    std::ofstream &getOutputStream();

    std::ifstream &getInputStream();

    bool isOutput() const;
};

#endif