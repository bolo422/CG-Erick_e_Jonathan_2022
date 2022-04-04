#include "Player.h"




void Player::jump(float force, float speed)
{
	if (!falling && jumped) {
		if (force > 150)
			force = 150;

		jumpHeight = force + position.y;
		jumped = false;
		jumping = true;
	}

	if (jumping && position.y < jumpHeight) {
		addPositionY(3.5 * speed);
		if (position.y >= jumpHeight) {
			falling = true;
			jumping = false;
		}
	}
	else if (falling && position.y > ground) {
		removePositionY(3.0f * speed);
		if (position.y <= ground) {
			position.y = 62;
			falling = false;
			endJump = true;
		}
	}

}