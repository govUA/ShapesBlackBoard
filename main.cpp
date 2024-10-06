#include <memory>
#include "Shape.h"
#include "Blackboard.h"


int main() {
    Blackboard blackboard(58, 32);

    std::shared_ptr<Shape> rect1 = std::make_shared<Rectangle>(1, 1, 9, 7);
    blackboard.addShape(rect1);

    std::shared_ptr<Shape> circ1 = std::make_shared<Circle>(9, 9, 7);
    blackboard.addShape(circ1);

    std::shared_ptr<Triangle> trig1 = std::make_shared<Triangle>(22, 1, 8, 15);
    blackboard.addShape(trig1);

    std::shared_ptr<Line> line1 = std::make_shared<Line>(7, 5, 18, 5);
    blackboard.addShape(line1);

    blackboard.draw();

    return 0;
}