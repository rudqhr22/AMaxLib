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
//   ������ ������ �� ��Ģ
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//������ ������ �ÿ� ���׿����� ��ġ�� ��ġ ������ ���
a.operator++() (->++a) / a.operator++(int) (->a++)
//������ ������ �ÿ� ���׿����ڸ� ��ġ�� �����(++a),
���۷����� ��ü�� �����(call by reference), *this�� �����Ѵ�.
//������ ������ �ÿ� ���׿����ڸ� ��ġ�� �����(a++), call by value�̵� const�� �����ؾ��Ѵ�.
�������� �����ϰ� �ڱ��ڽ� ���� ������Ų��.
//�������� ����Ű�� ���ο� ī���� ������ �ذ��ϱ� ���Ͽ�
���۷����� ���� �Լ��� ������ �ؾ��Ѵ�.
//�������ڸ� ������ٸ�
Int32 & operator=(const Int32 &r)�� �Բ� �������Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////////*/
// ��ġ ������ ������ 
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
// ��ġ ������ ������ 
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




// ����
float Vector2::LengthSquared()
{
	return (x*x + y * y);
}

// �������� ������ �Ÿ�
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
	//// Y���� �������� �Ʒ��� �����ϵ��� Ŭ���̾�Ʈ ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
	//pTarget.y *= -1.0f;
	//pPos.y *= -1.0f;
	//// ���� ���� �������� ���Ϳ� ������ ������ ���� ����Ѵ�.
	//Vector2 vDir;
	//vDir.x = pTarget.x - pPos.x;
	//vDir.y = pTarget.y - pPos.y;
	//vDir = vDir.Normal();
	//float fAngle = vDir.Angle(vLook);

	// �ð� �ݴ�������� �����̸� ������ ����.
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

// ������ ������ 
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

// ����
float Vector3::LengthSquared()
{
	return (x*x + y * y + z * z);
}

// �������� ������ �Ÿ�
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
// ������ ������ 
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
// ����
float Vector4::LengthSquared()
{
	return (x*x + y * y + z * z + w * w);
}

// �������� ������ �Ÿ�
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
