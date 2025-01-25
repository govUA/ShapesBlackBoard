#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>

class Shape {
protected:
    int x, y;
    char colour;
    bool fillMode;

    Shape(int x, int y, char colour, bool fillMode);

public:

    virtual ~Shape() = default;

    void editPosition(int nx, int ny) {
        x = nx;
        y = ny;
    };

    virtual void editSize(std::vector<float> sizes) = 0;

    virtual void draw(std::vector<std::vector<char>> &board) const = 0;

    virtual bool isSameSpot(const Shape &other) const = 0;

    virtual bool isWithinBounds(int boardWidth, int boardHeight) const = 0;

    virtual std::string getType() const = 0;

    virtual bool coversPoint(std::vector<std::vector<char>> &board, int x, int y) const = 0;

    virtual std::string describe() const = 0;

    virtual void serialize(std::ostream &os) const = 0;

    std::pair<int, int> getPosition() const;

    void editColour(char colour) { this->colour = colour; };

    char getColour() const {
        return colour;
    }

    bool getFillMode() const {
        return fillMode;
    }
};

class SRectangle : public Shape {
private:
    int width, height;

public:
    SRectangle(int x, int y, char colour, bool fillMode, int w, int h);

    virtual void editSize(std::vector<float> sizes) override;

    void draw(std::vector<std::vector<char>> &board) const override;

    bool isSameSpot(const Shape &other) const override;

    std::string getType() const override;

    bool coversPoint(std::vector<std::vector<char>> &board, int x, int y) const override;

    std::string describe() const override {
        std::ostringstream oss;
        oss << "Width: " << width << ", Height: " << height;
        return oss.str();
    }

    void serialize(std::ostream &os) const override {
        os << getType() << ' ' << getPosition().first << ' ' << getPosition().second << ' '
           << getColour() << ' ' << getFillMode() << ' ' << width << ' ' << height << '\n';
    }

    int getWidth() const;

    int getHeight() const;

    bool isWithinBounds(int boardWidth, int boardHeight) const;
};

class Circle : public Shape {
private:
    int radius;

public:
    Circle(int x, int y, char colour, bool fillMode, int r);

    virtual void editSize(std::vector<float> sizes) override;

    void draw(std::vector<std::vector<char>> &board) const override;

    bool isSameSpot(const Shape &other) const override;

    std::string getType() const override;

    bool coversPoint(std::vector<std::vector<char>> &board, int x, int y) const override;

    std::string describe() const override {
        std::ostringstream oss;
        oss << "Radius: " << radius;
        return oss.str();
    }

    void serialize(std::ostream &os) const override {
        os << getType() << ' ' << getPosition().first << ' ' << getPosition().second << ' '
           << getColour() << ' ' << getFillMode() << ' ' << radius << '\n';
    }

    int getRadius() const;

    bool isWithinBounds(int boardWidth, int boardHeight) const;
};

class Triangle : public Shape {
private:
    int height;
    int width;

public:
    Triangle(int x, int y, char colour, bool fillMode, int h, int w);

    void editSize(std::vector<float> sizes) override;

    void draw(std::vector<std::vector<char>> &board) const override;

    bool isSameSpot(const Shape &other) const override;

    std::string getType() const override;

    bool coversPoint(std::vector<std::vector<char>> &board, int x, int y) const override;

    std::string describe() const override {
        std::ostringstream oss;
        oss << "Width: " << width << ", Height: " << height;
        return oss.str();
    }

    void serialize(std::ostream &os) const override {
        os << getType() << ' ' << getPosition().first << ' ' << getPosition().second << ' '
           << getColour() << ' ' << getFillMode() << ' ' << height << ' ' << width << '\n';
    }

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

    void editSize(std::vector<float> sizes) override;

    void draw(std::vector<std::vector<char>> &board) const override;

    bool isSameSpot(const Shape &other) const override;

    std::string getType() const override;

    bool coversPoint(std::vector<std::vector<char>> &board, int x, int y) const override;

    std::string describe() const override {
        std::ostringstream oss;
        oss << "Length: " << length << ", Angle: " << angle;
        return oss.str();
    }

    void serialize(std::ostream &os) const override {
        os << getType() << ' ' << getPosition().first << ' ' << getPosition().second << ' '
           << getColour() << ' ' << getFillMode() << ' ' << length << ' ' << angle << '\n';
    }

    int getLength() const;

    double getAngle() const;

    bool isWithinBounds(int boardWidth, int boardHeight) const;
};

#endif