#include "Emulator.h"

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return -1;
	}

	chipotto::Emulator emulator;

	if (emulator.IsValid())
	{
		emulator.LoadFromFile(".\\PUZZLE");
		while (true)
		{
			if (!emulator.Tick())
				break;
		}
	}

	SDL_Quit();
	return 0;
}