#include "Vector.h"



Vector2::Vector2()
{
	x = y = 0.0f;
}

Vector2::Vector2(const Vector2& v0)
{
	x = v0.x;
	y = v0.y;
}

Vector2::Vector2(float fX, float fY)
{
	x = fX;
	y = fY;
}
/*////////////////////////////////////////////////////////////////////////////////////////////////////////
//   연산자 재정의 시 규칙
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//연산자 재정의 시에 단항연산자 전치와 후치 구분할 방법
a.operator++() (->++a) / a.operator++(int) (->a++)
//연산자 재정의 시에 단항연산자를 전치로 쓸경우(++a),
레퍼런스로 객체를 만들고(call by reference), *this를 리턴한다.
//연산자 재정의 시에 단항연산자를 후치로 쓸경우(a++), call by value이되 const로 리턴해야한다.
이전값을 리턴하고 자기자신 값은 증가시킨다.
//같은것을 가리키는 쉘로우 카피의 문제를 해결하기 위하여
오퍼레이터 이퀄 함수를 재정의 해야한다.
//복사형자를 만들었다면
Int32 & operator=(const Int32 &r)도 함께 만들어야한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////////*/
// 전치 연산자 재정의 
Vector2  Vector2::operator-()
{
	Vector2 temp = *this;
	temp = temp * -1.0f;
	return temp;
}

Vector2 & Vector2::operator++()
{
	*this = *this + *this;
	return *this;
}
// 후치 연산자 재정의 
const Vector2  Vector2::operator++(int)
{
	Vector2 temp = *this;
	++(*this);
	return temp;
}

Vector2 Vector2::operator + (Vector2 const &v0)
{
	return Vector2(v0.x + x, v0.y + y);
}

Vector2 Vector2::operator - (Vector2 const &v0)
{
	return Vector2(x - v0.x, y - v0.y);
}

Vector2 Vector2::operator + (float const &fScala)
{
	return Vector2(fScala + x, fScala + y);
}
Vector2 Vector2::operator * (float const &fScala)
{
	return Vector2(fScala*x, fScala*y);
}
Vector2 Vector2::operator / (float const &fScala)
{
	float fInvert = 1.0f / fScala;
	return Vector2(x*fInvert, y*fInvert);
}

// Dot Product
float Vector2::operator | (Vector2 const &v0)
{
	return x * v0.x + y * v0.y;
}

bool	Vector2::operator == (Vector2 const &v0)
{
	if (fabs(x - v0.x) < TBASIS_EPSILON)
	{
		if (fabs(y - v0.y) < TBASIS_EPSILON)
		{
			return true;
		}
	}
	return false;
}


bool Vector2::operator > (int const &v0)
{
	if (fabs(x - v0) )
	{
		if (fabs(y - v0))
		{
			return true;
		}
	}
	return false;
}

bool Vector2::operator > (float const &v0)
{
	if (fabs(x - v0))
	{
		if (fabs(y - v0))
		{
			return true;
		}
	}
	return false;
}




// 제곱
float Vector2::LengthSquared()
{
	return (x*x + y * y);
}

// 원점으로 부터의 거리
float Vector2::Length()
{
	return (float)sqrt(LengthSquared());
}

Vector2 Vector2::Normal()
{
	float invertLength = 1.0f / Length();
	return Vector2(x*invertLength, y*invertLength);
}

float Vector2::Angle(Vector2& v0)
{
	Vector2 vA = Normal();
	Vector2 vB = v0.Normal();
	float fDot = (vA | vB);
	if (fDot > 1.0f) fDot = 1.0f;
	float fRadian = acos(fDot);
	float fAngle = (float)RadianToDegree(fRadian);
	if (fDot < 0.0f)
	{
		fAngle = 180.0f + (180.0f - fAngle);
	}
	else
	{
		fAngle = fAngle;
	}
	return fAngle;
}

// Cross Product
Vector2 Vector2::operator ^ (Vector2 const &v0)
{
	if (*this == v0)
	{
		return v0;
	}
	return Vector2(0, (x*v0.y - y * v0.x));
}

bool   Vector2::CCW(Vector2 vLook, Vector2 vDir)
{
	//Vector2 pTarget, pPos;
	//// Y값은 원점에서 아래로 감소하도록 클라이언트 좌표를 직각 좌표로 변환한다.
	//pTarget.y *= -1.0f;
	//pPos.y *= -1.0f;
	//// 이후 이전 프레임의 벡터와 현재의 벡터의 각을 계산한다.
	//Vector2 vDir;
	//vDir.x = pTarget.x - pPos.x;
	//vDir.y = pTarget.y - pPos.y;
	//vDir = vDir.Normal();
	//float fAngle = vDir.Angle(vLook);

	// 시계 반대방향으로 움직이면 각도를 뺀다.
	Vector2 vA, vB, vC, vD;
	vA.x = vLook.x;	vA.y = vLook.y;
	vB.x = vDir.x;	vB.y = vDir.y;
	vC = (vA ^ vB).Normal();
	float fDot = vC | Vector2(0, -1);
	if (fDot > 0.0f)
	{
		return false;
	}
	return true;
}


/*Vector3 vA, vB, vC, vD;
vA.x = m_pOwner->m_vLook.x;	vA.y = m_pOwner->m_vLook.y;	vA.z = 0.0f;
vB.x = vDir.x;	vB.y = vDir.y;	vB.z = 0.0f;
vC = (vA ^ vB).Normal();
float fDot = vC | Vector3(0,0,-1);*/



Vector3::Vector3()
{
	x = y = z = 0.0f;
}

Vector3::Vector3(const Vector3& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
}

Vector3::Vector3(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

// 연산자 재정의 
Vector3 Vector3::operator + (Vector3 const &v0)
{
	return Vector3(v0.x + x, v0.y + y, v0.z + z);
}

Vector3 Vector3::operator - (Vector3 const &v0)
{
	return Vector3(x - v0.x, y - v0.y, z - v0.z);
}

Vector3 Vector3::operator + (float const &fScala)
{
	return Vector3(fScala + x, fScala + y, fScala + z);
}
Vector3 Vector3::operator * (float const &fScala)
{
	return Vector3(fScala*x, fScala*y, fScala*z);
}
Vector3 Vector3::operator / (float const &fScala)
{
	float fInvert = 1.0f / fScala;
	return Vector3(x*fInvert, y*fInvert, z*fInvert);
}

// Dot Product
float Vector3::operator | (Vector3 const &v0)
{
	return x * v0.x + y * v0.y + z * v0.z;
}

// Cross Product
Vector3 Vector3::operator ^ (Vector3 const &v0)
{
	return Vector3((y*v0.z - z * v0.y), (z*v0.x - x * v0.z), (x*v0.y - y * v0.x));
}

bool	Vector3::operator == (Vector3 const &v0)
{
	if (fabs(x - v0.x) < TBASIS_EPSILON)
	{
		if (fabs(y - v0.y) < TBASIS_EPSILON)
		{
			if (fabs(z - v0.z)< TBASIS_EPSILON)
			{
				return true;
			}
		}
	}
	return false;
}

// 제곱
float Vector3::LengthSquared()
{
	return (x*x + y * y + z * z);
}

// 원점으로 부터의 거리
float Vector3::Length()
{
	return (float)sqrt(LengthSquared());
}

Vector3 Vector3::Normal()
{
	float invertLength = 1.0f / Length();
	x = x * invertLength;
	y = y * invertLength;
	z = z * invertLength;
	return Vector3(x, y, z);
}

float Vector3::Angle(Vector3& v0)
{
	float fLength1 = Length();
	float fLength2 = v0.Length();
	float fCosAngle = ((*this) | v0) / (fLength1*fLength2);
	return (float)RadianToDegree(acos(fCosAngle));
}


Vector4::Vector4()
{
	x = y = z = w = 0.0f;
}

Vector4::Vector4(const Vector4& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
	w = v0.w;
}

Vector4::Vector4(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}
// 연산자 재정의 
Vector4 Vector4::operator + (Vector4 const &v0)
{
	return Vector4(v0.x + x, v0.y + y, v0.z + z, v0.w + w);
}

Vector4 Vector4::operator - (Vector4 const &v0)
{
	return Vector4(x - v0.x, y - v0.y, z - v0.z, w - v0.w);
}

Vector4 Vector4::operator + (float const &fScala)
{
	return Vector4(fScala + x, fScala + y, fScala + z, fScala + w);
}
Vector4 Vector4::operator * (float const &fScala)
{
	return Vector4(fScala*x, fScala*y, fScala*z, fScala*w);
}
// Dot Product
float Vector4::operator | (Vector4 const &v0)
{
	return x * v0.x + y * v0.y + z * v0.z + w * v0.w;
}

bool	Vector4::operator == (Vector4 const &v0)
{
	if (fabs(x - v0.x) < TBASIS_EPSILON)
	{
		if (fabs(y - v0.y) < TBASIS_EPSILON)
		{
			if (fabs(z - v0.z)< TBASIS_EPSILON)
			{
				if (fabs(w - v0.w)< TBASIS_EPSILON)
				{
					return true;
				}
			}
		}
	}
	return false;
}
// 제곱
float Vector4::LengthSquared()
{
	return (x*x + y * y + z * z + w * w);
}

// 원점으로 부터의 거리
float Vector4::Length()
{
	return (float)sqrt(LengthSquared());
}

Vector4 Vector4::Normal()
{
	float invertLength = 1.0f / Length();
	x = x * invertLength;
	y = y * invertLength;
	z = z * invertLength;
	w = w * invertLength;
	return Vector4(x, y, z, w);
}
