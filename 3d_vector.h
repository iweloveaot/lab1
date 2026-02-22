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


Vector3D* createVector(TypeInfo*);
void setCoords(Vector3D*, const void*, const void*, const void*);
Vector3D* createVectorWithElems(TypeInfo*, const void*, const void*, const void*);
void deleteVector(Vector3D*);

Vector3D* vectorAdd(const Vector3D*, const Vector3D*);
int scalarProduct(const Vector3D*, const Vector3D*, void*);
Vector3D* crossProduct(const Vector3D*, const Vector3D*);

int isCompatible(const Vector3D*, const Vector3D*);
int printVector(const Vector3D*);
Vector3D* inputVector(TypeInfo* info);


#endif // _VECTOR3D_H_



