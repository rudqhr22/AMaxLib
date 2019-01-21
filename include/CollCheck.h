#pragma once
#include "AModel.h"



class CollCheck : public ASingleton< CollCheck >
{
private:
	friend class ASingleton<CollCheck>;
public:
	bool	AABB_AABB(A_Box& thisBox, A_Box& targetBox);


public:
	CollCheck();
	virtual~CollCheck();
};
#define I_CHECK CollCheck::GetInstance()
