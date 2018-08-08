#ifndef _PATHWAY_H_
#define _PATHWAY_H_
#include "Vector2.h"
#include <vector>

struct PathNode
{
	PathNode* prev;
	PathNode* next;

	// vector2 is holding the position in the world 
	//that i want my enemy to follow

	Vector2 pos; // here is my node(that it!)
};

inline void CreatePathNode(std::vector<PathNode*>* path, Vector2 newPosition)
{
	// pass in position into Vector2
	// remember to create the new node

	PathNode* node = new PathNode();
	node->pos = newPosition;
	
	// previous and next need to get set to NULL = 0;

	node->prev = 0;
	node->next = 0;

	path->push_back(node);
}

#endif