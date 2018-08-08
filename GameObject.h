#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <SDL.h>
#include <string>
#include "Vector2.h"
#include "CommonVariables.h"

class GameManager;

class GameObject
{
	friend class GameManager;
	friend class Player;
	friend class Enemy;
	friend class Boss;
	
	friend struct RenderObject;

public:
	GameObject();
	virtual ~GameObject();

	virtual void Update();
	virtual void Render(SDL_Renderer* renderer);
	virtual bool LoadFile(std::string file);

	virtual bool IsColliding( GameObject* other);

	virtual void HandleCollision( GameObject* other);

	Vector2 offset;

protected:
	std::string name;
	std::string type;
	SDL_Texture* texture;
	SDL_Rect position;
	int layer;
};

#endif

