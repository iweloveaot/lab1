#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "3d_vector.h"


int initVector(TypeInfo* info, Vector3D* v)
{
    if (!info || !v) return -1;

    v->x = malloc(info->size);
    v->y = malloc(info->size);
    v->z = malloc(info->size);
    v->type_info = info;

    if (!v->x || !v->y || !v->z) 
    {
        free(v->x);
        free(v->y);
        free(v->z);
        return -1; 
    }

    info->zero(v->x);
    info->zero(v->y);
    info->zero(v->z);
    return 0;
};


void setCoords(Vector3D* v, const void* x, const void* y, const void* z)
{
    memcpy(v->x, x, v->type_info->size);
    memcpy(v->y, y, v->type_info->size);
    memcpy(v->z, z, v->type_info->size);
}


int initVectorWithElems(TypeInfo* info, const void* x, const void* y, const void* z, Vector3D* v)
{
    int init_err = initVector(info, v);
    if (!init_err) setCoords(v, x, y, z);
    return init_err;
    
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


int vectorAdd(const Vector3D* v1, const Vector3D* v2, Vector3D* res) 
{
    if (!isCompatible(v1, v2)) 
    {
        fprintf(stderr, "Error: vectors are incompatible (different data types) or not initialized\n");
        return -1;
    }

    int init_err = initVector(v1->type_info, res);
    if (!init_err)
    {
        v1->type_info->add(v1->x, v2->x, res->x);
        v1->type_info->add(v1->y, v2->y, res->y);
        v1->type_info->add(v1->z, v2->z, res->z);
    }
    return init_err;
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


int crossProduct(const Vector3D* v1, const Vector3D* v2, Vector3D* res)
{
    if (!isCompatible(v1, v2)) 
    {
        fprintf(stderr, "Error: vectors are incompatible (different data types) or not initialized\n");
        return -1;
    }

    int init_err = initVector(v1->type_info, res);
    if (!init_err)
    {
        void *temp1 = malloc(v1->type_info->size);
        void *temp2 = malloc(v1->type_info->size);

        if (!temp1 || !temp2) 
        {
            free(temp1);
            free(temp2);
            deleteVector(res);
            return -1;
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
    }
  
    return init_err;
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


int inputVector(TypeInfo* info, Vector3D* v) 
{
    if (!info || !v) return -1;

    void* x = malloc(info->size);
    void* y = malloc(info->size);
    void* z = malloc(info->size);

    if (!x || !y || !z) 
    {
        free(x);
        free(y);
        free(z);
        return -1;
    }

    printf("Input vector coordinates:\n");

    printf("x: ");
    info->input(x);
    printf("y: ");
    info->input(y);
    printf("z: ");
    info->input(z); 

    int init_err = initVectorWithElems(info, x, y, z, v);
    if (!init_err)
    {
        free(x);
        free(y);
        free(z);
    }

    return init_err;
}

