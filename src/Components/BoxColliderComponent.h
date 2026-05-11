#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include "glm/vec2.hpp"

struct BoxColliderComponent {
    glm::vec2 offset;
    double width;
    double height;
    bool isTrigger;

    BoxColliderComponent(double width = 0, double height = 0, glm::vec2 offset = glm::vec2(0, 0),
                         bool isTrigger = false) {
        this->width = width;
        this->height = height;
        this->offset = offset;
        this->isTrigger = isTrigger;
    }
};

#endif //BOXCOLLIDERCOMPONENT_H
