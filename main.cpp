#include <iostream>
#include <vector>

class Blackboard {
private:
    int width;
    int height;
    std::vector<std::vector<char>> board;

public:
    Blackboard(int w, int h) : width(w), height(h) {
        board.resize(height, std::vector<char>(width, ' '));
    }

    void draw() const {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                std::cout << board[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void clear() {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                board[i][j] = '.';
            }
        }
    }

    void addShape() {
        // to do
    }
};

int main() {
    Blackboard blackboard(10, 5);
    blackboard.draw();
    return 0;
}