#include "Enemy.h"
#include "Vector2.h"
#include <fstream>


Enemy::Enemy(PathNode* t, float s)
	: target(t)
	, speed(s), state("idle_d")
{
	frame = 0;
	previousTime = 0;
	direction = Vector2(0,0);
	isAlive = true;
	isRight = true;
	
	
}

Enemy::~Enemy()
{
	target = nullptr;
}

void Enemy::Update()
{
	if(isAlive)
	{
		float dt = 0.1;
		if(target)
		{
	
			direction = target->pos - offset;

			// make sure that direction is normalized

			float magnitude = direction.magnitude();

			// normalization of the direction
			// first get mag(length) then divide by the mag
			if(magnitude > 0)
				direction /= magnitude;

			float dist = sqrt((target->pos.x - offset.x) * (target->pos.x - offset.x) 
				+ (target->pos.y - offset.y) * (target->pos.y - offset.y));

			if(isRight)
			{
				if(dist <= position.w/2)
				{
					if(target->next)
					{
						target = target->next;
					}
					else
					{
						isRight = false;
					}
				}
				
			}
			else
			{
				if(dist <= position.w/2)
				{
					// if target next is avail. then target is set to next
					// if not avail. then goin in opp. dir which is prev

					if(target->prev)
					{
						target = target->prev;
					}
					else
					{
						isRight = true;
						
					}
				}

			}

			
		}
		
		// making the enemy move in a direction
		
		offset += speed * direction * dt;

		GameObject::Update();
	}
	else
	{
		
			state = "death";
		
	}
}

void Enemy::Render(SDL_Renderer* renderer)
{
	SDL_Rect clipArea = animations[state].clip;
	clipArea.x += frame * animations[state].clip.w;


	SDL_RenderCopy(renderer, texture, &clipArea, &position);

}

bool Enemy::LoadFile(std::string file)
{
	// open file
	std::ifstream in;
	in.open(file);

	if(in.fail())
	{
		return false;
	}

	//read file

	std::string dummy;
	std::string imgFile;
	std::string name;
	Animation animation;

	getline(in, dummy);
	in >> imgFile;

	if(!GameObject::LoadFile(imgFile))
		return false;

	while(in >> name)
	{
		in >> animation.numFrames >> animation.frameRate >> animation.clip.x
			>> animation.clip.y >> animation.clip.w >> animation.clip.h;

		animations[name] = animation;
	}

	

	// close file

	in.close();

	return true;
}

void Enemy::HandleCollision(GameObject* obj)
{
		if(strcmp(obj->type.c_str(),"object") == 0 )
		{
			if(offset.y < obj->offset.y && position.h + offset.y < obj->offset.y + obj->position.h)
			{
				float colOffset;

				colOffset = (offset.y + position.h) - obj->offset.y;

				offset.y -= colOffset;

			}
		}
}