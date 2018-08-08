#include "Boss.h"
#include "Vector2.h"
#include <fstream>


Boss::Boss(Player* t, float s)
	: speed(s), state("idle_l"), target(t)
{
	frame = 0;
	previousTime = 0;
	direction = Vector2(0,0);
	isAlive = true;
	isRight = true;
}


Boss::~Boss()
{
	target = nullptr;
}

void Boss::Update()
{
	if(isAlive)
	{
		float dt = 0.1;

		if(target)
		{
			direction = target->offset - offset;

			float magnitude = direction.magnitude();

			if(magnitude > 0)
			{
				direction /= magnitude;
			}


			float dis = sqrt((target->offset.x - offset.x) * (target->offset.x - offset.x) 
				+ (target->offset.x - offset.y) * (target->offset.x - offset.y));

			if(dis < 500)
			{
				// stalk the player
				offset += speed * direction * dt;
			}
		}

		GameObject::Update();
	}
	else
	{
		state = "death";
	}
}

void Boss::Render(SDL_Renderer* renderer)
{
	SDL_Rect clipArea = animations[state].clip;

	clipArea.x = animations[state].clip.w;

	SDL_RenderCopy(renderer,texture,&clipArea, &position);
}

bool Boss::LoadFile(std::string file)
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

void Boss::HandleCollision(GameObject* obj)
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