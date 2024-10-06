cmake_minimum_required(VERSION 3.28)
project(ShapesBlackBoard)

set(CMAKE_CXX_STANDARD 17)

add_executable(ShapesBlackBoard main.cpp
        Shape.cpp
        Shape.h
        Blackboard.cpp
        Blackboard.h
        RaiiWrapper.cpp
        RaiiWrapper.h)
