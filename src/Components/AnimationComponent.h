#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include "SDL.h"

struct AnimationComponent{
    int numFrames;
    int currentFrame;
    int frameRateSpeed;
    bool isLoop;
    int startTime;
    AnimationComponent(int numFrames = 1,int frameRateSpeed = 1,bool isLoop = true){
        this->numFrames = numFrames;
        this->currentFrame = 1;
        this->frameRateSpeed = frameRateSpeed;
        this->isLoop = isLoop;
        this->startTime = SDL_GetTicks();
    }
};
#endif //ANIMATIONCOMPONENT_H
