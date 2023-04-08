#pragma once
#include <iostream>

class Vector2
{
public:
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);

	Vector2& Add			(const Vector2& vec);
	Vector2& Subtract		(const Vector2& vec);
	Vector2& Multiply		(const Vector2& vec);
	Vector2& Divide 		(const Vector2& vec);
	static float&   Distance		(Vector2 B1, Vector2 B2);

	friend Vector2& operator+ (Vector2& v1, const Vector2& v2);
	friend Vector2& operator- (Vector2& v1, const Vector2& v2);
	friend Vector2& operator* (Vector2& v1, const Vector2& v2);
	friend Vector2& operator/ (Vector2& v1, const Vector2& v2);
	//friend Vector2& operatorF (const double g);


	Vector2& operator+= (const Vector2& vec);
	Vector2& operator-= (const Vector2& vec);
	Vector2& operator*= (const Vector2& vec);
	Vector2& operator/= (const Vector2& vec);


	Vector2& operator*	(const int& i);
	//float& operatorF	(const double& i);
	Vector2& Zero ();









	friend std::ostream& operator<<(std::ostream& stream, const Vector2& vec);
};