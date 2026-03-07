#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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


int rotateAroundAxis(const double angle, Vector3D* v, const char axis)
{
    if (!v) return -1;

    Vector3D* unit_vector = (Vector3D*)malloc(sizeof(Vector3D));
    int init_unit_err = initVector(v->type_info, unit_vector);
    if (init_unit_err) 
    {
        deleteVector(unit_vector);
        return -1;
    }

    void* ux = malloc(v->type_info->size);
    void* uy = malloc(v->type_info->size);
    void* uz = malloc(v->type_info->size);
    if (!ux || !uy || !uz) 
    {  
        free(ux);
        free(uy);
        free(uz);
        deleteVector(unit_vector);
        return -1;
    }

    switch (axis)
    {
        case 'x':
            v->type_info->unit(ux);
            v->type_info->zero(uy);
            v->type_info->zero(uz);
            setCoords(unit_vector, ux, uy, uz);
            break;
        
        case 'y':
            v->type_info->zero(ux);
            v->type_info->unit(uy);
            v->type_info->zero(uz);
            setCoords(unit_vector, ux, uy, uz);
            break;

        case 'z':
            v->type_info->zero(ux);
            v->type_info->zero(uy);
            v->type_info->unit(uz);
            setCoords(unit_vector, ux, uy, uz);
            break;
        
        default:
            fprintf(stderr, "Error: indefinite axis %c", axis);
            return -1;    
    }
    
    void* scal_pr = malloc(v->type_info->size);
    Vector3D* cr_pr = (Vector3D*)malloc(sizeof(Vector3D));
    Vector3D* term1_1 = (Vector3D*)malloc(sizeof(Vector3D));
    Vector3D* term1_2 = (Vector3D*)malloc(sizeof(Vector3D));
    Vector3D* term2 = (Vector3D*)malloc(sizeof(Vector3D));
    Vector3D* term3 = (Vector3D*)malloc(sizeof(Vector3D));
    if (!scal_pr || !cr_pr || !term1_1 || !term1_2 || !term2 || !term3)
    {
        free(ux);
        free(uy);
        free(uz);
        deleteVector(unit_vector);
        free(scal_pr);
        free(cr_pr);
        free(term1_1);
        free(term1_2);
        free(term2);
        free(term3);
        return -1;
    } 

    int term1_1_init_err = initVector(v->type_info, term1_1);
    int term2_init_err = initVector(v->type_info, term2);
    int term3_init_err = initVector(v->type_info, term3);
    if (term1_1_init_err || term2_init_err || term3_init_err)
    {
        free(ux);
        free(uy);
        free(uz);
        deleteVector(unit_vector);
        free(scal_pr);
        free(cr_pr);
        deleteVector(term1_1);
        free(term1_2);
        deleteVector(term2);
        deleteVector(term3);
        return -1;
    }

    int sc_pr_err = scalarProduct(v, unit_vector, scal_pr);
    int cr_pr_err = crossProduct(unit_vector, v, cr_pr);
    if (sc_pr_err || cr_pr_err) 
    {
        free(ux);
        free(uy);
        free(uz);
        deleteVector(unit_vector);
        free(scal_pr);
        if (!cr_pr_err) deleteVector(cr_pr);
        else free(cr_pr);
        deleteVector(term1_1);
        deleteVector(term1_2);
        deleteVector(term2);
        deleteVector(term3);
        return -1;
    }
    printVector(cr_pr);
    v->type_info->print(scal_pr);

    v->type_info->multiply(unit_vector->x, scal_pr, term3->x);
    v->type_info->multiply(unit_vector->y, scal_pr, term3->y);
    v->type_info->multiply(unit_vector->z, scal_pr, term3->z);

    double s = sin(angle), c = cos(angle);
    v->type_info->multiply_coef(cr_pr->x, s, term2->x);
    v->type_info->multiply_coef(cr_pr->y, s, term2->y);
    v->type_info->multiply_coef(cr_pr->z, s, term2->z);

    v->type_info->multiply(unit_vector->x, scal_pr, term1_1->x);
    v->type_info->multiply(unit_vector->y, scal_pr, term1_1->y);
    v->type_info->multiply(unit_vector->z, scal_pr, term1_1->z);
    v->type_info->negative(term1_1->x);
    v->type_info->negative(term1_1->y);
    v->type_info->negative(term1_1->z);

    int add_err = vectorAdd(v, term1_1, term1_2);
    if (add_err) 
    {
        free(ux);
        free(uy);
        free(uz);
        deleteVector(unit_vector);
        free(scal_pr);
        deleteVector(cr_pr);
        deleteVector(term1_1);
        free(term1_2);
        deleteVector(term2);
        deleteVector(term3);
        return -1;
    }
    v->type_info->multiply_coef(term1_2->x, c, term1_2->x);
    v->type_info->multiply_coef(term1_2->y, c, term1_2->y);
    v->type_info->multiply_coef(term1_2->z, c, term1_2->z);
    
    Vector3D* result = (Vector3D*)malloc(sizeof(Vector3D));
    Vector3D* mid_result = (Vector3D*)malloc(sizeof(Vector3D));
    if (!result || !mid_result)
    {
        free(ux);
        free(uy);
        free(uz);
        deleteVector(unit_vector);
        free(scal_pr);
        deleteVector(cr_pr);
        deleteVector(term1_1);
        deleteVector(term1_2);
        deleteVector(term2);
        deleteVector(term3);
        free(result);
        free(mid_result);
        return -1;
    }
    
    int mid_sum_err = vectorAdd(term1_2, term2, mid_result);
    if (mid_sum_err)
    {
        free(ux);
        free(uy);
        free(uz);
        deleteVector(unit_vector);
        free(scal_pr);
        deleteVector(cr_pr);
        deleteVector(term1_1);
        deleteVector(term1_2);
        deleteVector(term2);
        deleteVector(term3);
        free(result);
        free(mid_result);
        return -1;
    }
    int res_sum_err = vectorAdd(mid_result, term3, result);
    if (res_sum_err)
    {
        free(ux);
        free(uy);
        free(uz);
        deleteVector(unit_vector);
        free(scal_pr);
        deleteVector(cr_pr);
        deleteVector(term1_1);
        deleteVector(term1_2);
        deleteVector(term2);
        deleteVector(term3);
        free(result);
        deleteVector(mid_result);
        return -1;
    } 

    const double one = 1.0;
    v->type_info->multiply_coef(result->x, one, v->x);\
    v->type_info->multiply_coef(result->y, one, v->y);
    v->type_info->multiply_coef(result->z, one, v->z);

    free(ux);
    free(uy);
    free(uz);
    deleteVector(unit_vector);
    free(scal_pr);
    deleteVector(cr_pr);
    deleteVector(term1_1);
    deleteVector(term1_2);
    deleteVector(term2);
    deleteVector(term3);
    deleteVector(result);
    deleteVector(mid_result);

    return 0;
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
    free(x);
    free(y);
    free(z);

    return init_err;
}

