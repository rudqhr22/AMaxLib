#include "CollCheck.h"

bool		CollCheck::AABB_AABB(A_Box& thisBox,A_Box& targetBox)
{
	thisBox.vMin.x;


	if (thisBox.vMin.x <= targetBox.vMax.x && thisBox.vMax.x >= targetBox.vMin.x &&
		thisBox.vMin.y <= targetBox.vMax.y && thisBox.vMax.y >= targetBox.vMin.y &&
		thisBox.vMin.z <= targetBox.vMax.z && thisBox.vMax.z >= targetBox.vMin.z)
	{
		return true;
	}
	return false;
}


//////bool		CheckOBBInPlane(A_Box&  pBox)
//////{
//////	return true;
//////}
//
//bool		CheckOBBInPlaxxxne(A_Box jjj)
//{
//	return true;
//}

CollCheck::CollCheck()
{
}


CollCheck::~CollCheck()
{
}
