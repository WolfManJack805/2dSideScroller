#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.h"
#include <map>
#include "Animations.h"


class Player: public GameObject
{
	friend class GameManager;
	friend class GameObject;
	

public:

	Player();
	~Player();

	void Update() override;
	void Render(SDL_Renderer* renderer) override;
	bool LoadFile(std::string file) override;

	void HandleCollision(GameObject* other);


protected:

	std::string state;
	int frame;
	Uint32 previousTime;
	std::map<std::string, Animation> animations;
	bool animate;
	bool isGrounded;
	float speed;

	Vector2 dir;

	bool isAlive;
	int lives;
};

#endif