#include "Blackboard.h"
#include "CLI.h"

int main() {
    Blackboard blackboard(16, 16);
    CLI cli(blackboard);

    cli.run();

    return 0;
}