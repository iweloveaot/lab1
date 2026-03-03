#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#include "types.h"

typedef struct Vector3D
{
    void* x;
    void* y;
    void* z;
    TypeInfo* type_info;
} Vector3D;


int initVector(TypeInfo*, Vector3D*);
void setCoords(Vector3D*, const void*, const void*, const void*);
int initVectorWithElems(TypeInfo*, const void*, const void*, const void*, Vector3D*);
void deleteVector(Vector3D*);

int vectorAdd(const Vector3D*, const Vector3D*, Vector3D*);
int scalarProduct(const Vector3D*, const Vector3D*, void*);
int crossProduct(const Vector3D*, const Vector3D*, Vector3D*);

int isCompatible(const Vector3D*, const Vector3D*);
int printVector(const Vector3D*);
int inputVector(TypeInfo* info, Vector3D*);


#endif // _VECTOR3D_H_



