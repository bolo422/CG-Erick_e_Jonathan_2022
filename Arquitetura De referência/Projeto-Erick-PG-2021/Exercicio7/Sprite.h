#pragma once
#include "Object.h"
class Sprite :
    public Object
{
public:
    Sprite() {}
    ~Sprite() {}

    void initialize(GLuint _texID, int _nAnimations, int _nFrames);
    inline void setAnimation(int _iAnimation) { iAnimation = _iAnimation; }
    inline void setFrame(int _iFrame) { iFrame = _iFrame; }
    void update();



protected:
    int nAnimations, nFrames, iAnimation, iFrame;
    float dx, dy;



};

