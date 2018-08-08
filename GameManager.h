#ifndef _GAME_MANAGER_H
#define _GAME_MANAGER_H

#include <SDL.h>
#include <string>
#include <vector>
#include "Pathway.h"
#include <SDL_ttf.h>


class GameObject;
class Player;

namespace KeyInfo
{
	enum Keys
	{
		UP, DOWN, LEFT, RIGHT, Q, E, SPACE, SIZE
	};
}

class GameManager
{
	
public:
	GameManager();
	~GameManager();


	bool Init(int w, int h, int x, int y);

	bool LoadLevel(std::string filename);
	
	bool Update();

	void Close();

	static GameManager* GetInstance();
	SDL_Texture* ConvertStringToText(TTF_Font* font,std::string, SDL_Color color);
	SDL_Texture* LoadMedia(std::string filename);
	

private:

	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	void OnGui();
	

	bool KeyUpdate();
	bool KeyDown(SDL_Keycode key);
	bool KeyUp(SDL_Keycode key);

	bool keys[KeyInfo::SIZE];

	Player* player;

	std::vector<GameObject*> objects;
	std::vector<PathNode*> path;

	static GameManager* instance;

	GameObject* backGround [2];
	
};


#endif