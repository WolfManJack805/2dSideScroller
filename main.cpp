#include <SDL.h>
#include <stdio.h>

#include "CommonVariables.h"
#include "GameManager.h"


using namespace std;

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_renderer = NULL;
static SDL_Texture* g_texture = NULL;
static SDL_Rect g_offset = { 0, 0, 0, 0};

int main( int argc, char* args[])
{
	GameManager gm;

	if(!gm.Init(SCREEN_WIDTH, SCREEN_HEIGHT, 50, 50))
	{
		return -1;
	}

	if(!gm.LoadLevel("level1.txt"))
		return -1;

	// way of making a game loop so that you can play game
	bool isRunning = true;

	while(isRunning)
	{
		isRunning = gm.Update();
	}


	gm.Close();

	return 0;
}