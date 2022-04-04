#pragma once
#include "Sprite.h"
#include "Object.h"

class Player :
    public Sprite
    //public Object
{
public:
    Player() {}
    ~Player() {}

    //Verifica se está no chão
    inline bool grounded() { if (position.y <= ground) { return true; } else { return false; } }

    //Pular
    void jump(float force, float speed);
    inline void jump(bool _jumping) { jumped = _jumping; }

    inline bool getFalling() { return falling; }
    inline bool getJumping() { return jumping; }
    inline bool getEndJump() { return endJump; }
    inline void setEndJump(bool _endJump) { endJump = _endJump; }
    inline void resetJump() { jumping = false; falling = false; jumped = false; endJump = false; jumpHeight = 0; }


private:

    bool jumping = false;
    bool falling = false;
    bool jumped = false;
    bool endJump = false;

    float jumpHeight = 0;
    const int ground = 62; //Altura do chão + alturado do player/2


};

