#include "Blackboard.h"
#include "CLI.h"

int main() {
    int width, height;

    std::cout << "Enter the width of the blackboard: ";
    std::cin >> width;
    std::cout << "Enter the height of the blackboard: ";
    std::cin >> height;

    if (width <= 0 || height <= 0) {
        std::cerr << "Invalid board dimensions. Exiting program." << std::endl;
        return 1;
    }

    Blackboard blackboard(width, height);
    CLI cli(blackboard);

    cli.run();

    return 0;
}