#pragma once
#include "AStd.h"
#include "Max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

//SIMPLE TYPE
#include "utilapi.h"
#include "stdmat.h"
#include "decomp.h"
#include "shape.h"
#include "interpik.h"
#include "cs/bipexp.h"
#include "cs/phyexp.h"
#include "iskin.h"

#include "resource.h"



//extern TCHAR *GetString(int id);

extern HINSTANCE hInstance;



const enum OBJECTCLASSTYPE
{
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};

typedef struct _D3D_MATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
	};
}D3D_MATRIX, *LPD3D_MATRIX;

struct P3NCVERTEX
{
	float		Px, Py, Pz;
	float		Nx, Ny, Nz;
	Point4		c;
};

struct PNCT_VERTEX
{
	Point3		p;
	Point3		n;
	Point4		c;
	Point2	    t;
};



#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define TODO(x) __FILE__ "(" STRING(__LINE__) "): TODO: "x

#define CTL_CHARS		31
#define SINGLE_QUOTE	39 // ( ' )
#define ALMOST_ZERO		1.0e-3f