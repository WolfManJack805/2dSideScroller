#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include <fstream>

Player::Player()
	:state("walk_l"), frame(0)
	, previousTime(0)
{
	animate = false;
	isGrounded = false;
	dir = Vector2(0,0);
	speed = 5;
	isAlive = true;
	lives = 3;
}

Player::~Player()
{

}

void Player::Update()
{
	if(isAlive)
	{
		if(lives <= 0)
		{
			isAlive = false;
			frame = 0;
		}

		if(dir.x == 1 || dir.x == -1)
		{
			offset += dir * speed;
		}

		// players movement state

		if(dir.x == -1)
		{
			state = "walk_l";
		}
		else if (dir.x == 1)
		{
			state = "walk_r";
		}
		else if (dir.x == 0)
		{
			if(strcmp(state.c_str(), "walk_l") == 0 )
			{
				state = "idle_l";
			}
			else if (strcmp(state.c_str(), "walk_r") == 0)
			{
				state = "idle_r";
			}
		}
		
		// if the player is below this screen lenght then player is dead

		if(offset.y+ (position.h-40) > SCREEN_HEIGHT)
		{
			
			lives = 0;
			
		}
		// get ticks in the start of the game in milsecs

		Uint32 currentTime = SDL_GetTicks();

		if(currentTime - previousTime > 1000/animations[state].frameRate)
		{
			frame ++;
			frame %= animations[state].numFrames;
			previousTime = currentTime;
		}

		GameObject::Update();
	}
	else
	{
		// here i would animate the players death
		state = "death";
		

		Uint32 currentTime = SDL_GetTicks();

		if(currentTime - previousTime > 1000/animations[state].frameRate)
		{
			if(frame < animations[state].numFrames)
			{
				frame++;
				previousTime = currentTime;
			}
		}
	}
}

void Player::Render(SDL_Renderer* renderer)
{
	

	SDL_Rect clipArea = animations[state].clip;
	clipArea.x += frame * animations[state].clip.w;


	SDL_RenderCopy(renderer, texture, &clipArea, &position);
}

bool Player::LoadFile(std::string file)
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

void Player::HandleCollision(GameObject* other)
{
	// handle collision detail

	if( strcmp(other->type.c_str(), "object") == 0)
	{
		//players position in space
		//check what side you are touching and push player back up
		// calculate how much you are penetrating on that side
		// have to move player by the amount calculated in opp. dir he is coming from

		
		if(offset.y < other->offset.y && (position.h - 40) + offset.y < other->offset.y + other->position.h)
		{
			float colOffset;

			colOffset = (offset.y + (position.h - 40)) - other->offset.y;

			offset.y -= colOffset;

			

			isGrounded = true;
		}
			
		else if(offset.x < other->offset.x && position.w + offset.x < other->offset.x + other->position.w)
		{
			float colOffset;

			colOffset = (offset.x + (position.w)) - other->offset.x;

			offset.x -= colOffset;

			
		}
		else if(offset.x > other->offset.x && offset.x + position.w > other->offset.x + other->position.w)
		{
			float colOffset;

			colOffset = (other->offset.x + other->position.w) - (offset.x);

			offset.x += colOffset;
		}

	}
	else if (strcmp(other->type.c_str(), "enemy") == 0)
	{
		// type cast to enemy so that this class can recognize its the enemy class
		// also distinguishes the action of the player with enemy ex. if i jump on enemy then enemy dies..ect.

		Enemy *e = (Enemy*) other;
		if(e->isAlive)
		{
		
			if(offset.y < other->offset.y && (position.h-40) + offset.y < (other->position.h / 2) + other->offset.y)
			{
				// do something (destroy enemy)
			
				e->isAlive = false;
				e->position.x = -100;

			}
			else if (offset.x < other->offset.x && position.w + offset.x < other->offset.x + position.w)
			{
				// player take damage
				if(isAlive)
				{
					lives -= 1;
					offset.x -= 23;
					
				}
			}
			else if(offset.x > other->offset.x && position.w + offset.x > other->offset.x + position.w)
			{
				// player take damage
				if(isAlive)
				{
					lives -= 1;
					offset.x += 23;
					
				}
			}
		}
	}
	else if(strcmp(other->type.c_str(), "boss") == 0)
	{
		Boss *b = (Boss*) other;

		if(b->isAlive)
		{
			if(offset.y < other->offset.y && offset.y + (position.h - 40) < (other->position.h/2) + other->offset.y)
			{
				// destroy enemy
				b->isAlive = false;
				b->position.x = -100;
			}
			else if (offset.x < other->offset.x && offset.x + position.w < other->offset.x + other->position.w)
			{
				if(isAlive)
				{
					// player dies
					lives -= 1;
					offset.x -= 23;
				}
				
			}
			else if (offset.x > other->offset.x && offset.x + position.w > other->offset.x + other->position.w)
			{
				if(isAlive)
				{
					// player dies
					lives -= 1;
					offset.x += 23;
				}
				
			}
		}
	}
}