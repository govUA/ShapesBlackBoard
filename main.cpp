#include <memory>
#include "Shape.h"
#include "Blackboard.h"


int main() {
    Blackboard blackboard(58, 32);

    blackboard.draw();
    blackboard.listShapes();

    return 0;
}