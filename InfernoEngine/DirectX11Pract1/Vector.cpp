#include "Vector.h"
#include <math.h>

#include "Helper.h"

#pragma region Vector3 region

Vector3::Vector3(){};
Vector3::Vector3(float xVal,float yVal,float zVal)
{
	this->x = xVal;
	this->y = yVal;
	this->z = zVal;
}

Vector3::Vector3(const Vector3& otherVector)
{
	this->x = otherVector.x;
	this->y = otherVector.y;
	this->z = otherVector.z;
}

Vector3::~Vector3()
{

}

Vector3 Vector3::One = Vector3(1,1,1);
Vector3 Vector3::Zero = Vector3(0,0,0);
Vector3 Vector3::Right = Vector3(1,0,0);;
Vector3 Vector3:: Left = Vector3(-1,0,0);
Vector3 Vector3:: Up = Vector3(0,1,0);
Vector3 Vector3:: Down = Vector3(0,-1,0);
Vector3 Vector3:: Forward = Vector3(0,0,1);
Vector3 Vector3:: Backward = Vector3(0,0,-1);


float Vector3::Distance(Vector3 origin,Vector3 target)
{
	Vector3 tempPoint;

	tempPoint.x = target.x - origin.x;
	tempPoint.y = target.y - origin.y;
	tempPoint.z = target.z - origin.z;

	return sqrt(((tempPoint.x*tempPoint.x) + (tempPoint.y*tempPoint.y)+(tempPoint.z*tempPoint.z)));
}


Vector3 Vector3::operator+ (Vector3 param) 
{
	Vector3 temp;
	temp.x = x + param.x;
	temp.y = y + param.y;
	temp.z = z + param.z;
	return (temp);
}

Vector3 Vector3::operator- (Vector3 param) 
{
	Vector3 temp;
	temp.x = x - param.x;
	temp.y = y - param.y;
	temp.z = z - param.z;
	return (temp);
}

void Vector3::operator= (Vector3 param) 
{
	x = param.x;
	y = param.y;
	z = param.z;
}

Vector3 Vector3::operator* (float param)
{
	Vector3 temp;
	temp.x = x*param;
	temp.y = y*param;
	temp.z = z*param;
	return temp;
}

Vector3 Vector3::operator* (const Vector3& otherVector)
{
	Vector3 temp;
	temp.x = this->x*otherVector.x;
	temp.y = this->y*otherVector.y;
	temp.z = this->z*otherVector.z;
	return temp;
}

Vector3 Vector3::operator/ (float param)
{
	Vector3 temp;
	temp.x = x/param;
	temp.y = y/param;
	temp.z = z/param;
	return temp;
}

float Vector3::DotProduct(Vector3 v1,Vector3 v2)
{
	float dotProduct = (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);
	return dotProduct;
}


Vector3 Vector3::CrossProduct(Vector3 v1,Vector3 v2)
{
	Vector3 tempVector;

	tempVector.x = (v1.y*v2.z - v1.z*v2.y);
	tempVector.y = (v1.z*v1.x - v1.x*v2.z);
	tempVector.z = (v1.x*v2.y - v1.y*v2.x);

	return tempVector;
}

bool Vector3::IsEqual( Vector3 a, Vector3 b)
{
	bool x,y,z;
	x = Helper::AreEqual(a.x,b.x);
	y = Helper::AreEqual(a.y,b.y);
	z = Helper::AreEqual(a.z,b.z);
	if(x&&y&&z)
		return true;

	return false;

}

float Vector3::Magnitude()
{
	float magnitude = ((this->x*this->x)  + (this->y*this->y) +(this->z*this->z));
	this->magnitude = sqrt(magnitude);
	return this->magnitude;
}

Vector3 Vector3::Normalize(Vector3 givenDirection)
{
	givenDirection.x = givenDirection.x/givenDirection.Magnitude();
	givenDirection.y = givenDirection.y/givenDirection.Magnitude();
	givenDirection.z = givenDirection.z/givenDirection.Magnitude();

	return givenDirection;
}

Vector3 Vector3::GetNormalized()
{
	Vector3 normalized;
	normalized.x = this->x;
	normalized.y = this->y;
	normalized.z = this->z;
	return Normalize(normalized);
}
#pragma endregion Vector3 region


#pragma region Vector2 region


Vector2::Vector2()
{
	x = 0;
	y = 0;
}
Vector2::Vector2(float xVal,float yVal)
{
	this->x = xVal;
	this->y = yVal;
}

Vector2::Vector2(const Vector2& otherVector)
{
	this->x = otherVector.x;
	this->y = otherVector.y;
}

Vector2::~Vector2()
{

}

Vector2 Vector2::One = Vector2(1,1);
Vector2 Vector2::Zero = Vector2(0,0);
Vector2 Vector2::Right = Vector2(1,0);;
Vector2 Vector2:: Left = Vector2(-1,0);
Vector2 Vector2:: Up = Vector2(0,1);
Vector2 Vector2:: Down = Vector2(0,-1);


float Vector2::Distance(Vector2 origin,Vector2 target)
{
	Vector2 tempPoint;

	tempPoint.x = target.x - origin.x;
	tempPoint.y = target.y - origin.y;

	return sqrt(((tempPoint.x*tempPoint.x) + (tempPoint.y*tempPoint.y)));
}

Vector2 Vector2::operator+ (Vector2 param) 
{
	Vector2 temp;
	temp.x = x + param.x;
	temp.y = y + param.y;

	return (temp);
}

Vector2 Vector2::operator- (Vector2 param) 
{
	Vector2 temp;
	temp.x = x - param.x;
	temp.y = y - param.y;
	return (temp);
}

void Vector2::operator= (Vector2 param) 
{
	x = param.x;
	y = param.y;
}

Vector2 Vector2::operator* (float param)
{
	Vector2 temp;
	temp.x = x*param;
	temp.y = y*param;
	return temp;
}

Vector2 Vector2::operator* (const Vector2& otherVector)
{
	Vector2 temp;
	temp.x = this->x*otherVector.x;
	temp.y = this->y*otherVector.y;
	return temp;
}

Vector2 Vector2::operator/ (float param)
{
	Vector2 temp;
	temp.x = x/param;
	temp.y = y/param;
	return temp;
}

float Vector2::DotProduct(Vector2 v1,Vector2 v2)
{
	float dotProduct = (v1.x*v2.x) + (v1.y*v2.y);
	return dotProduct;
}

bool Vector2::IsEqual( Vector2 a, Vector2 b)
{
	bool x,y;
	x = Helper::AreEqual(a.x,b.x);
	y = Helper::AreEqual(a.y,b.y);
	if(x&&y)
		return true;

	return false;

}

float Vector2::Magnitude()
{
	float magnitude = ((this->x*this->x)  + (this->y*this->y));
	this->magnitude = sqrt(magnitude);
	return this->magnitude;
}

Vector2 Vector2::Normalize(Vector2 givenDirection)
{
	givenDirection.x = givenDirection.x/givenDirection.Magnitude();
	givenDirection.y = givenDirection.y/givenDirection.Magnitude();

	return givenDirection;
}

Vector2 Vector2::GetNormalized()
{
	Vector2 normalized;
	normalized.x = this->x;
	normalized.y = this->y;
	return Normalize(normalized);
}



#pragma endregion Vector2 region


#pragma region Vector4 region


Vector4::Vector4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1.0f;
};
Vector4::Vector4(float xVal,float yVal,float zVal,float wVal)
{
	this->x = xVal;
	this->y = yVal;
	this->z = zVal;
	this->w = wVal;
}

Vector4::Vector4(const Vector4& otherVector)
{
	this->x = otherVector.x;
	this->y = otherVector.y;
	this->z = otherVector.z;
	this->w = otherVector.w;
}

Vector4::~Vector4()
{

}

Vector4 Vector4::One = Vector4(1,1,1,1);
Vector4 Vector4::Zero = Vector4(0,0,0,0);
Vector4 Vector4::Right = Vector4(1,0,0,1);;
Vector4 Vector4:: Left = Vector4(-1,0,0,1);
Vector4 Vector4:: Up = Vector4(0,1,0,1);
Vector4 Vector4:: Down = Vector4(0,-1,0,1);
Vector4 Vector4:: Forward = Vector4(0,0,1,1);
Vector4 Vector4:: Backward = Vector4(0,0,-1,1);


float Vector4::Distance(Vector4 origin,Vector4 target)
{
	Vector4 tempPoint;

	tempPoint.x = target.x - origin.x;
	tempPoint.y = target.y - origin.y;
	tempPoint.z = target.z - origin.z;
	tempPoint.w = 1.0f;
	return sqrt(((tempPoint.x*tempPoint.x) + (tempPoint.y*tempPoint.y)+(tempPoint.z*tempPoint.z)));
}


Vector4 Vector4::operator+ (Vector4 param) 
{
	Vector4 temp;
	temp.x = x + param.x;
	temp.y = y + param.y;
	temp.z = z + param.z;
	temp.w = w + param.w;
	return (temp);
}

Vector4 Vector4::operator- (Vector4 param) 
{
	Vector4 temp;
	temp.x = x - param.x;
	temp.y = y - param.y;
	temp.z = z - param.z;
	temp.w = w - param.w;
	return (temp);
}

void Vector4::operator= (Vector4 param) 
{
	x = param.x;
	y = param.y;
	z = param.z;
	w = param.w;
}

Vector4 Vector4::operator* (float param)
{
	Vector4 temp;
	temp.x = x*param;
	temp.y = y*param;
	temp.z = z*param;
	temp.w = w*param;
	return temp;
}

Vector4 Vector4::operator* (const Vector4& otherVector)
{
	Vector4 temp;
	temp.x = this->x*otherVector.x;
	temp.y = this->y*otherVector.y;
	temp.z = this->z*otherVector.z;
	temp.w = this->w*otherVector.w;
	return temp;
}

Vector4 Vector4::operator/ (float param)
{
	Vector4 temp;
	temp.x = x/param;
	temp.y = y/param;
	temp.z = z/param;
	temp.w = w/param;
	return temp;
}

float Vector4::DotProduct(Vector4 v1,Vector4 v2)
{
	float dotProduct = (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z) + (v1.w * v2.w);
	return dotProduct;
}


Vector4 Vector4::CrossProduct(Vector4 v1,Vector4 v2)
{
	Vector4 tempVector;

	tempVector.x = (v1.y*v2.z - v1.z*v2.y);
	tempVector.y = (v1.z*v1.x - v1.x*v2.z);
	tempVector.z = (v1.x*v2.y - v1.y*v2.x);
	return tempVector;
}

bool Vector4::IsEqual( Vector4 a, Vector4 b)
{
	bool x,y,z;
	x = Helper::AreEqual(a.x,b.x);
	y = Helper::AreEqual(a.y,b.y);
	z = Helper::AreEqual(a.z,b.z);
	if(x&&y&&z)
		return true;

	return false;
}

float Vector4::Magnitude()
{
	float magnitude = ((this->x*this->x)  + (this->y*this->y) +(this->z*this->z) + (this->w * this->w));
	this->magnitude = sqrt(magnitude);
	return this->magnitude;
}

Vector4 Vector4::Normalize(Vector4 givenDirection)
{
	givenDirection.x = givenDirection.x/givenDirection.Magnitude();
	givenDirection.y = givenDirection.y/givenDirection.Magnitude();
	givenDirection.z = givenDirection.z/givenDirection.Magnitude();
	givenDirection.w = givenDirection.w/givenDirection.Magnitude();

	return givenDirection;
}

Vector4 Vector4::GetNormalized()
{
	Vector4 normalized;
	normalized.x = this->x;
	normalized.y = this->y;
	normalized.z = this->z;
	normalized.w = 1.0f;
	return Normalize(normalized);
}

#pragma endregion Vector4 region