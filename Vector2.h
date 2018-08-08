#include <cmath>

#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_


struct Vector2
{
	Vector2()
	{}

	Vector2(float xx, float yy)
		: x(xx), y(yy)
	{
	}

	Vector2(const Vector2& copy)
		: x(copy.x), y(copy.y)
	{
	}


	float magnitude()
	{
		return sqrt(x*x + y*y);
	}

	Vector2 operator/(float c)
	{
		return Vector2(x/c, y/c);
	}

	Vector2 operator/=(float c)
	{
		x /=c;
		y /=c;
		return *this;
	}

	Vector2 operator*(float c)
	{
		return Vector2 (x*c, y*c);
	}

	Vector2 operator- (Vector2 v)
	{
		return Vector2( x-v.x, y-v.y );
	}

	Vector2 operator+ (Vector2 v)
	{
		return Vector2( x+v.x, y+v.y );
	}

	Vector2 operator+=(Vector2 v)
	{
		x += v.x;
		y += v.y;
		return * this;
	}


	float x, y;
};

static Vector2 operator*(float c, Vector2& v)
	{
		return Vector2 (v.x*c, v.y*c);
	}

#endif