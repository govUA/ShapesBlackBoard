#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <vector>
#include <cmath>

class Shape {
protected:
    int x, y;
    char colour;
    bool fillMode;
public:
    Shape(int x, int y, char colour, bool fillMode);

    virtual ~Shape() = default;

    virtual void draw(std::vector<std::vector<char>> &board) const = 0;

    virtual bool isSameSpot(const Shape &other) const = 0;

    virtual bool isWithinBounds(int boardWidth, int boardHeight) const = 0;

    virtual std::string getType() const = 0;

    std::pair<int, int> getPosition() const;

    void setColour(char newColour) {
        colour = newColour;
    }

    char getColour() const {
        return colour;
    }

    void setFillMode(bool mode) {
        fillMode = mode;
    }

    bool getFillMode() const {
        return fillMode;
    }
};

class Rectangle : public Shape {
private:
    int width, height;

public:
    Rectangle(int x, int y, char colour, bool fillMode, int w, int h);

    void draw(std::vector<std::vector<char>> &board) const override;

    bool isSameSpot(const Shape &other) const override;

    std::string getType() const override;

    int getWidth() const;

    int getHeight() const;

    bool isWithinBounds(int boardWidth, int boardHeight) const;
};

class Circle : public Shape {
private:
    int radius;

public:
    Circle(int x, int y, char colour, bool fillMode, int r);

    void draw(std::vector<std::vector<char>> &board) const override;

    bool isSameSpot(const Shape &other) const override;

    std::string getType() const override;

    int getRadius() const;

    bool isWithinBounds(int boardWidth, int boardHeight) const;
};

class Triangle : public Shape {
private:
    int height;
    int width;

public:
    Triangle(int x, int y, char colour, bool fillMode, int h, int w);

    void draw(std::vector<std::vector<char>> &board) const override;

    bool isSameSpot(const Shape &other) const override;

    std::string getType() const override;

    int getHeight() const;

    int getWidth() const;

    bool isWithinBounds(int boardWidth, int boardHeight) const;
};

class Line : public Shape {
private:
    int length;
    double angle;

public:
    Line(int x, int y, char colour, bool fillMode, int l, double a);

    void draw(std::vector<std::vector<char>> &board) const override;

    bool isSameSpot(const Shape &other) const override;

    std::string getType() const override;

    int getLength() const;

    double getAngle() const;

    bool isWithinBounds(int boardWidth, int boardHeight) const;
};

#endif