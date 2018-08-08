#include "GameObject.h"
#include "Player.h"
#include "GameManager.h"


GameObject::GameObject()
	:texture(nullptr)
{
	offset.x = 0;
	offset.y = 0;
	position.w = 0;
	position.h = 0;
}


GameObject::~GameObject()
{

}

void GameObject::Update()
{
	position.x = (int)offset.x;
	position.y = (int) offset.y;
}

void GameObject::Render(SDL_Renderer* renderer)
{
	

	SDL_RenderCopy(renderer, texture, NULL, &position);
}

bool GameObject::LoadFile(std::string file)
{
	texture = GameManager::GetInstance()->LoadMedia(file);
	return texture != nullptr;
}

bool GameObject::IsColliding( GameObject* other)
{
	if (layer != other->layer)
	{
		return false;
	}

	if( offset.x < other->offset.x + other->position.w &&
		offset.x + position.w > other->offset.x &&
		offset.y < other->offset.y + other->position.h &&
		offset.y + position.h > other->offset.y)
	{
		printf("Collided!\n");
		// collision detection
		return true;
	}

	return false;
}

void GameObject::HandleCollision(GameObject* other)
{
	
}

