#ifndef _BOSS_H_
#define _BOSS_H_
#include "GameObject.h"
#include "Player.h"
#include "Animations.h"
#include "Pathway.h"

#include <map>

class Boss: public GameObject
{
	friend class GameObject;
	friend class Player;
	friend class GameManager;

public:
	Boss(Player* t, float s);
	~Boss();

	void Update() override;

	void Render(SDL_Renderer* renderer) override;
	bool LoadFile(std::string file) override;

	void HandleCollision(GameObject* obj);

protected:

	Player* target;
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

