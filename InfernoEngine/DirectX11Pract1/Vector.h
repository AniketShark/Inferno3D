#ifndef Vector_H_
#define Vector_H_


#include <math.h>

class Vector3
{
public:

	Vector3(); 
	Vector3(const Vector3&);
	Vector3(float xVal, float yVal, float zVal);
	~Vector3();
	//
	float x;
	float y;
	float z;

	static float Distance(Vector3 origin, Vector3 target);
	static bool IsEqual( Vector3, Vector3);
	static Vector3 Normalize(Vector3 givenDirection);
	static float DotProduct(Vector3,Vector3);
	static Vector3 CrossProduct(Vector3,Vector3);

	static Vector3 One;
	static Vector3 Zero;
	static Vector3 Right;
	static Vector3 Left;
	static Vector3 Up;
	static Vector3 Down;
	static Vector3 Forward;
	static Vector3 Backward;


	Vector3 operator + (Vector3);
	Vector3 operator - (Vector3);
	void operator= (Vector3);

	Vector3 operator* (float);
	Vector3 operator* (const Vector3&);

	Vector3 operator/ (float);

	Vector3 GetNormalized();
	float Magnitude();
private: 
	float magnitude;
};

class Vector2
{
public:
	Vector2(); 
	Vector2(const Vector2&);
	Vector2(float xVal, float yVal);
	~Vector2();

	float x;
	float y;

	static float Distance(Vector2 origin, Vector2 target);
	static bool IsEqual( Vector2, Vector2);
	static Vector2 Normalize(Vector2 givenDirection);
	static float DotProduct(Vector2,Vector2);

	static Vector2 One;
	static Vector2 Zero;
	static Vector2 Right;
	static Vector2 Left;
	static Vector2 Up;
	static Vector2 Down;

	Vector2 operator + (Vector2);
	Vector2 operator - (Vector2);
	void operator= (Vector2);

	Vector2 operator* (float);
	Vector2 operator* (const Vector2&);

	Vector2 operator/ (float);

	Vector2 GetNormalized();
	float Magnitude();
private: 
	float magnitude;
};

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

	Vector4(); 
	Vector4(const Vector4&);
	Vector4(float xVal, float yVal, float zVal,float wVal);
	~Vector4();

	static float Distance(Vector4 origin, Vector4 target);
	static bool IsEqual( Vector4, Vector4);
	static Vector4 Normalize(Vector4 givenDirection);
	static float DotProduct(Vector4,Vector4);
	static Vector4 CrossProduct(Vector4,Vector4);

	static Vector4 One;
	static Vector4 Zero;
	static Vector4 Right;
	static Vector4 Left;
	static Vector4 Up;
	static Vector4 Down;
	static Vector4 Forward;
	static Vector4 Backward;


	Vector4 operator + (Vector4);
	Vector4 operator - (Vector4);
	void operator= (Vector4);

	Vector4 operator* (float);
	Vector4 operator* (const Vector4&);

	Vector4 operator/ (float);

	Vector4 GetNormalized();
	float Magnitude();
private: 
	float magnitude;
};





#endif