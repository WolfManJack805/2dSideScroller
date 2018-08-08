#include "GameManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

#include <SDL_image.h>
#include <fstream>
#include <queue>



struct RenderObject
{
	GameObject * obj;
	bool operator <(const RenderObject& other) const
	{
		return obj->layer > other.obj->layer;
	}
};

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
	:window(nullptr), renderer (nullptr)
{
	if(instance != nullptr)
	{
		delete instance;
	}

	instance = this;

	for(int i = 0; i < KeyInfo::SIZE; ++i)
	{
		keys[i] = false;
	}

	for(int i = 0; i < 2; ++i)
	{
		backGround[i] = 0;
		
	}
	
	font = 0;
}


GameManager::~GameManager()
{
}

bool GameManager::Init(int w, int h, int x, int y)
{
	if(SDL_Init (SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	if(IMG_Init (IMG_INIT_PNG== 0))
	{
		return false;
	}
	if(TTF_Init() == -1)
	{
		return false;
	}
	window = SDL_CreateWindow("SDL STARTUP", x, y, w, h
		,SDL_WINDOW_SHOWN);

	if(window == NULL)
	{
		return false;
	}

	if(!(font = TTF_OpenFont("alphaslab.ttf", 24)))
	{
		return false;
	}

	renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if(renderer == NULL)
	{
		return false;
	}

	return true;
}

bool GameManager::LoadLevel(std::string filename)
{
	//open file

	std::ifstream in;

	in.open(filename);

	if (in.fail())
	{
		return false;
	}

	//read file

	std::string name;
	std::string type;
	int x, y, w, h, layer;
	std::string imgFile;
	std::string dummyLine;
	int backgroundIndex = 0;

	while(in >> name)
	{
		if(name[0] == '#')
		{
			getline(in, dummyLine);
			continue;
		}

		in >> type >> x >> y >> w >> h >> layer >> imgFile;

		GameObject* obj = NULL;

		if(type == "player")
		{
			obj = new Player();
			player = (Player*) obj; // type cast
			
		}
		else if (type == "enemy")
		{
			obj = new Enemy(0, 13);
		}
		else if (type == "boss")
		{
			obj = new Boss(player, 20);
		}
		else if (type == "bg")
		{
			if(backgroundIndex < 2)
			{
				obj = new GameObject();
				backGround[backgroundIndex] = obj;
				backgroundIndex++;
			}

		}
		else if(strcmp(type.c_str(),"object") == 0)
		{
			CreatePathNode(&path, Vector2(x + (w * 0.5f) ,y - (h * 0.5f)));
			obj = new GameObject();
		}
		else
		{
			obj = new GameObject();
		}

		obj->name = name;
		obj->type = type;
		obj->layer = layer;
		obj->LoadFile(imgFile);

		if(obj->texture == NULL)
		{
			return false;
		}

		obj->offset.x = x;
		obj->offset.y = y;
		obj->position.w = w;
		obj->position.h= h;

		objects.push_back(obj);


	}
	//close file

	in.close();

	//loop through the nodes paths and link them together so we can work with them

	for(int i = 0; i < path.size(); i++)
	{
		PathNode* node1 = path[i];

		for(int j = i+1; j < path.size(); j++)
		{
			PathNode* node2 = path[j];

			// pothgarm and therom

			float dist = sqrt(((node2->pos.x - node1->pos.x) * (node2->pos.x - node1->pos.x)) 
				+ ((node2->pos.y - node1->pos.y) * (node2->pos.y - node1->pos.y)));

			if(dist <= 100)
			{
				if(node2->pos.y < node1->pos.y || node1->pos.y < node2->pos.y)
				{
					continue;
				}
				node1->next = node2;
				node2->prev = node1;
			}
		}
	}


	for(int i = 0; i < path.size(); ++i)
	{
		// loop through the pathnodes and also the game objects to find
		// each enemy

		PathNode* node = path[i];

		for(int j = 0; j < objects.size(); ++j )
		{
			if(strcmp(objects[j]->type.c_str(), "enemy") == 0)
			{
				Enemy* e = (Enemy*) objects[j];
				
				// get the distance between enemy and node path

				float distance = sqrt(((node->pos.x - e->offset.x) * (node->pos.x - e->offset.x)) 
				+ ((node->pos.y - e->offset.y) * (node->pos.y - e->offset.y)));
				// if distance <= 100 then set target of enemy to node

				if(distance <= 100)
				{
					e->target = path[i];
				}

				
			}

		}

	}
	return true;
}

bool isJumping = false;

float gravity = -4.81f;

// create a bool for the left margin and the right margin

bool leftMargin;
bool rightMargin;

bool GameManager::Update()
{
	if(KeyUpdate() == false)
	{
		return false;
	}

	// here is my game logic
	// also included marigin movment info of the screen right or left

	if(player != nullptr)
	{
		player->dir = Vector2(0,0);

		
		if(player->offset.x > 0)
		{
			if(keys[KeyInfo::LEFT])
			{
				player->dir.x = -1;
				player->state = "walk_l";
				
			}

			leftMargin = false;
		}
		else
		{
			for(int i = 0; i < 2; ++i)
			{
				backGround[i]->offset.x ++;
				
			}

			leftMargin = true;
		}
		if(player->offset.x + player->position.w  < SCREEN_WIDTH -110)
		{
			if (keys[KeyInfo::RIGHT])
			{
				player->dir.x = 1;
				player->state = "walk_r";
				
			}

			rightMargin = false;
		}
		else
		{
			for(int i = 0; i < 2; ++i)
			{
				backGround[i]->offset.x--;
				
			}
			
			rightMargin = true;
			
			
		}

		// here is where the screens background image gets moves to the right or the left

		for(int i = 0; i < 2; ++i)
		{
			if(backGround[i]->offset.x + backGround[i]->position.w < 0)
			{
				backGround[i]->offset.x += backGround[i]->position.w *2; 
			}
			if(backGround[i]->offset.x > SCREEN_WIDTH)
			{
				backGround[i]->offset.x -= backGround[i]->position.w *2;
			}
		}

	

		

		// first part of my jumping function

		if(keys[KeyInfo::SPACE] && isJumping == false && player->isGrounded)
		{
			isJumping = true;
		}

		// here is the additional part to my jump function

		if(isJumping)
		{
			player->offset.y -= 102;
			isJumping = false;
			player->isGrounded = false;
		}

		// player affected by gravity

		player->offset.y -= gravity;

		// update objects by key press

		for(int i = 0; i < objects.size(); ++i)
		{
			if(strcmp(objects[i]->type.c_str(), "enemy") == 0)
			{
				objects[i]->offset.y -= gravity;


			}

			// HOW TO MAKE THE BRIDGE BACKGROUND 


			if(rightMargin)
			{
				// move the path to see all the paths in the level. check if [i] is < the path size

				if(i < path.size())
				{
					path[i]->pos.x -= player->speed;
				}
				// object is moving to left
				
				objects[i]->offset.x -= player->speed;
			}
			else if (leftMargin)
			{
				// object move to right (WILL USE ONLY IF NEEDED)
				//objects[i]->offset.x += player->speed;

			}
			
			objects[i]->Update();

			// added collision detection for players

			for(int i = 0; i < objects.size(); ++i)
			{
				for( int j = i + 1; j < objects.size(); ++ j )
				{
					//if(strcmp(objects[j]->type.c_str(), "object") == 0)
					{
						if (objects[i] -> IsColliding(objects[j]))
						{
							objects[i] -> HandleCollision(objects[j]);
						}
					}
				}
			}

		
		}


	}

	// will clear the screen

	SDL_RenderClear(renderer);

	// how to prioritize que

	std::priority_queue<RenderObject> queue;

	for(unsigned int i = 0; i < objects.size(); ++i)
	{
		RenderObject renderObj;
		renderObj.obj = objects[i];
		queue.push(renderObj);
	}

	while(!queue.empty())
	{
		RenderObject renderObj = queue.top();
		queue.pop();

		renderObj.obj->Render(renderer);
	}

	for(int i = 0; i < path.size(); ++i)
	{
		PathNode* node = path[i];
		SDL_Rect rect;
		rect.x = node->pos.x;
		rect.y = node->pos.y;
		rect.w = 10;
		rect.h = 10;

		SDL_RenderDrawRect(renderer, &rect);

		if(node->next)
		{
			PathNode* node2 = node->next;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

			SDL_RenderDrawLine(renderer, node->pos.x, node->pos.y, node2->pos.x, node2->pos.y);
		}
		
	}

	OnGui();

	// Update screen
	SDL_RenderPresent(renderer);

	//60 FPS

	SDL_Delay(17);

	return true;
}

void GameManager::OnGui()
{
	std::string Life;
	std:: string gameOver;
	char buffer[256] = {0};
	Life = "Lives: ";
	gameOver = "Game Over!";
	Life += itoa(player->lives, buffer, 10);
	SDL_Color color;
	color.a = 255;
	color.b = 255;
	color.g = 255;
	color.r = 255;
	SDL_Texture* text = ConvertStringToText(font, Life, color);
	
	if(player->lives <= 0)
	{
		text = ConvertStringToText(font, gameOver, color);
	}

	
	// part of rendering
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 2;

	SDL_QueryTexture(text, 0 , 0 , &rect.w, &rect.h);
	SDL_RenderCopy(renderer, text, 0, &rect);

	SDL_DestroyTexture(text);

}

SDL_Texture* GameManager::ConvertStringToText(TTF_Font* font,std::string text, SDL_Color color)
{
	
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if(surface == 0)
	{
		return 0;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(texture == 0)
	{
		return 0;
	}

	SDL_FreeSurface(surface);

	return texture;
}

SDL_Texture* GameManager::LoadMedia(std::string filename)
{
	SDL_Surface* surf = IMG_Load(filename.c_str());

	if(surf == NULL)
	{
		exit (404);
		return NULL;
	}

	if(renderer == NULL)
	{
		exit(101);
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_FreeSurface(surf);
	
	return texture;
}

void GameManager::Close()
{
	for(int i = 0; i < objects.size(); ++i)
	{
		SDL_DestroyTexture(objects[i]->texture);
		objects[i]->texture = nullptr;
		delete objects[i];
	}

	for(int i = 0; i < path.size(); ++i)
	{
		delete path[i];
	}

	// Destroy our renderer

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	// Destroy our window

	SDL_DestroyWindow(window);
	window = NULL;

	IMG_Quit();
	SDL_Quit();
}

GameManager* GameManager::GetInstance()
{
	return instance;
}

bool GameManager::KeyUpdate()
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			return false;

		case SDL_KEYDOWN:
			if(!KeyDown(e.key.keysym.sym))
				return false;
				break;

		case SDL_KEYUP:
			if(!KeyUp(e.key.keysym.sym))
				return false;
				break;
		}
	}

	return true;
}

bool GameManager::KeyDown(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_UP:
	case 'w':
		keys[KeyInfo::UP] = true;
		break;

	case SDLK_DOWN:
	case 's':
		keys[KeyInfo::DOWN] = true;
		break;

	case SDLK_LEFT:
	case 'a':
		keys[KeyInfo::LEFT] = true;
		break;

	case SDLK_RIGHT:
	case 'd':
		keys[KeyInfo::RIGHT] = true;
		break;

	case SDLK_q:
		keys[KeyInfo::Q] = true;
		break;

	case SDLK_e:
		keys[KeyInfo::E] = true;
		break;

	case SDLK_SPACE:
		keys[KeyInfo::SPACE] = true;
		break;

	case SDLK_ESCAPE:
		return false;

	}

	return true;
}

bool GameManager::KeyUp(SDL_Keycode key)
{
	switch(key)
	{
	case SDLK_UP:
	case 'w':
		keys[KeyInfo::UP] = false;
		break;

	case SDLK_DOWN:
	case 's':
		keys[KeyInfo::DOWN] = false;
		break;

	case SDLK_LEFT:
	case 'a':
		keys[KeyInfo::LEFT] = false;
		break;

	case SDLK_RIGHT:
	case 'd':
		keys[KeyInfo::RIGHT] = false;
		break;

	case SDLK_q:
		keys[KeyInfo::Q] = false;
		break;

	case SDLK_e:
		keys[KeyInfo::E] = false;
		break;

	case SDLK_SPACE:
		keys[KeyInfo::SPACE] = false;
		break;

	case SDLK_ESCAPE:
		return false;
	}

	return true;

}
