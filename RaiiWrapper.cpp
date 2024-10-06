#include "RaiiWrapper.h"

RaiiWrapper::RaiiWrapper(const std::string &filePath, bool isOutput) {
    if (isOutput) {
        outFile.open(filePath);
        if (!outFile) {
            throw std::runtime_error("Error opening file for writing: " + filePath);
        }
    } else {
        inFile.open(filePath);
        if (!inFile) {
            throw std::runtime_error("Error opening file for reading: " + filePath);
        }
    }
}

RaiiWrapper::~RaiiWrapper() {
    if (outFile.is_open()) {
        outFile.close();
    }
    if (inFile.is_open()) {
        inFile.close();
    }
}

std::ofstream &RaiiWrapper::getOutputStream() {
    return outFile;
}

std::ifstream &RaiiWrapper::getInputStream() {
    return inFile;
}

bool RaiiWrapper::isOutput() const {
    return outFile.is_open();
}