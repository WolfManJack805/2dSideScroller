#include "GameObject.h"
#include "Animations.h"
#include "Pathway.h"

#include <map>

#ifndef _ENEMY_H_
#define _ENEMY_H_

class Enemy : public GameObject
{
	friend class GameObject;
	friend class Player;
	friend class GameManager;
	
	
public:

	Enemy(PathNode * t, float s);
	~Enemy();


	void Update() override;

	void Render(SDL_Renderer* renderer) override;
	bool LoadFile(std::string file) override;

	void HandleCollision(GameObject* obj);

protected:


	PathNode* target;
	float speed;

	Vector2 direction;

	std::string state;
	int frame;
	Uint32 previousTime;
	std::map<std::string, Animation> animations;
	bool animate;

	bool isAlive;
	bool isRight;
};



#endif