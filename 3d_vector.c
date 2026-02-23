#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3d_vector.h"


Vector3D* createVector(TypeInfo* info)
{
    if (!info) return NULL;

    Vector3D* v = (Vector3D*)malloc(sizeof(Vector3D));
    if (!v) return NULL;

    v->x = malloc(info->size);
    v->y = malloc(info->size);
    v->z = malloc(info->size);
    v->type_info = info;

    if (!v->x || !v->y || !v->z) 
    {
        free(v->x);
        free(v->y);
        free(v->z);
        free(v);
        return NULL;
    }

    info->zero(v->x);
    info->zero(v->y);
    info->zero(v->z);

    return v;
};


void setCoords(Vector3D* v, const void* x, const void* y, const void* z)
{
    memcpy(v->x, x, v->type_info->size);
    memcpy(v->y, y, v->type_info->size);
    memcpy(v->z, z, v->type_info->size);
}


Vector3D* createVectorWithElems(TypeInfo* info, const void* x, const void* y, const void* z)
{
    Vector3D* v = createVector(info);
    if (!v) return NULL;

    setCoords(v, x, y, z);
    return v;
}


void deleteVector(Vector3D* v)
{
    if (v)
    {
        free(v->x);
        free(v->y);
        free(v->z);
        free(v);
    }
};


int isCompatible(const Vector3D* v1, const Vector3D* v2) 
{
    return v1 && v2 && v1->type_info == v2->type_info;
}


Vector3D* vectorAdd(const Vector3D* v1, const Vector3D* v2) 
{
    if (!isCompatible(v1, v2)) 
    {
        fprintf(stderr, "Error: vectors are incompatible (different data types) or not initialized\n");
        return NULL;
    }

    Vector3D* res = createVector(v1->type_info);
    if (!res) return NULL;

    v1->type_info->add(v1->x, v2->x, res->x);
    v1->type_info->add(v1->y, v2->y, res->y);
    v1->type_info->add(v1->z, v2->z, res->z);

    return res;
}


int scalarProduct(const Vector3D* v1, const Vector3D* v2, void* product) 
{
    if (!isCompatible(v1, v2)) 
    {
        fprintf(stderr, "Error: vectors are incompatible (different data types) or not initialized\n");
        return -1;
    }


    void* temp1 = malloc(v1->type_info->size);
    void* temp2 = malloc(v1->type_info->size);
    void* temp3 = malloc(v1->type_info->size);

    if (!temp1 || !temp2 || !temp3) 
    {
        free(temp1);
        free(temp2);
        free(temp3);
        return -1;
    }


    // x1*x2
    v1->type_info->multiply(v1->x, v2->x, temp1);
    // y1*y2
    v1->type_info->multiply(v1->y, v2->y, temp2);
    // x1*x2 + y1*y2
    v1->type_info->add(temp1, temp2, temp3);
    // z1*z2
    v1->type_info->multiply(v1->z, v2->z, temp1);
    // (x1*x2 + y1*y2) + z1*z2
    v1->type_info->add(temp3, temp1, product);

    free(temp1);
    free(temp2);
    free(temp3);
    return 0;
}


Vector3D* crossProduct(const Vector3D* v1, const Vector3D* v2)
{
    if (!isCompatible(v1, v2)) 
    {
        fprintf(stderr, "Error: vectors are incompatible (different data types) or not initialized\n");
        return NULL;
    }

    Vector3D* res = createVector(v1->type_info);
    if (!res) return NULL;

    void *temp1 = malloc(v1->type_info->size);
    void *temp2 = malloc(v1->type_info->size);

    if (!temp1 || !temp2) 
    {
        free(temp1);
        free(temp2);
        deleteVector(res);
        return NULL;
    }

    // x = y1*z2 - z1*y2
    v1->type_info->multiply(v1->y, v2->z, temp1);  // y1*z2
    v1->type_info->multiply(v1->z, v2->y, temp2);  // z1*y2
    v1->type_info->negative(temp2);  // -z1*y2
    v1->type_info->add(temp1, temp2, res->x);  // y1*z2 - z1*y2

    // y = - (x1*z2 - z1*x2)
    v1->type_info->multiply(v1->x, v2->z, temp1);  // x1*z2
    v1->type_info->multiply(v1->z, v2->x, temp2);  // z1*x2
    v1->type_info->negative(temp2);  // -z1*x2
    v1->type_info->add(temp1, temp2, res->y);  // x1*z2 - z1*x2
    v1->type_info->negative(res->y);  // - (x1*z2 - z1*x2)

    // z = x1*y2 - y1*x2
    v1->type_info->multiply(v1->x, v2->y, temp1);  // x1*y2
    v1->type_info->multiply(v1->y, v2->x, temp2);  // y1*x2
    v1->type_info->negative(temp2);  // -y1*x2
    v1->type_info->add(temp1, temp2, res->z); // x1*y2 - y1*x2

    free(temp1);
    free(temp2);

    return res;
}


int printVector(const Vector3D* v) 
{
    if (!v) 
    {
        printf("NULL\n");
        return -1;
    }

    printf("(");
    v->type_info->print(v->x);
    printf(", ");
    v->type_info->print(v->y);
    printf(", ");
    v->type_info->print(v->z);
    printf(")\n");
    return 0;
}


Vector3D* inputVector(TypeInfo* info) 
{
    if (!info) return NULL;

    void* x = malloc(info->size);
    void* y = malloc(info->size);
    void* z = malloc(info->size);

    if (!x || !y || !z) 
    {
        free(x);
        free(y);
        free(z);
        return NULL;
    }

    printf("Input vector coordinates:\n");

    printf("x: ");
    info->input(x);
    printf("y: ");
    info->input(y);
    printf("z: ");
    info->input(z); 

    Vector3D* v = createVectorWithElems(info, x, y, z);

    free(x);
    free(y);
    free(z);

    return v;
}

