#include "GameManager.h"

int main()
{

	GameManager* game = new GameManager;
	game->Start();

	game->Update();

	game->Finish();

	return 0;
}