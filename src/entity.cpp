#include "entity.h"

Entity::Entity()
{
	x = 0.0;
	y = 0.0;
	z = 1.0; //Minimum Z to appear in the frustum.
	height = 0.0;
	width = 0.0;
	size = 0.0;
	velocityX = 0.0; velocityY = 0.0;
}

bool Entity::collide(Entity* e)
{
	if(e==NULL) return false;

	bool hitX;
	bool hitY;
	bool hit;	

	Box* c1 = this->collisionBox();
	Box* c2 = e->collisionBox();
	
	//Verify if the max or the min is in the limits of the other, and vice-versa.
	hitX = (c2->minX > c1->minX && c2->minX < c1->maxX) || (c2->maxX > c1->minX && c2->maxX < c1->maxX)
	|| (c1->minX > c2->minX && c1->minX < c2->maxX) || (c1->maxX > c2->minX && c1->maxX < c2->maxX);
	hitY = (c2->minY > c1->minY && c2->minY < c1->maxY) || (c2->maxY > c1->minY && c2->maxY < c1->maxY)
	|| (c1->minY > c2->minY && c1->minY < c2->maxY) || (c1->maxY > c2->minY && c1->maxY < c2->maxY);
	hit = hitX && hitY;

	delete c1; delete c2;

	return hit;
}
void Entity::print()
{
	cout << "\n		Entidade: Pos -> {" << x << " , " << y << " , " << z << "};";
	cout << "\n		Entidade: Largura -> " << width << " | Altura -> " << height << " | Tamanho -> " << size;
}


Entity::~Entity()
{
}
