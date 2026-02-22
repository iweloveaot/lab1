#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "3d_vector.h"

void test_double_operations() 
{
    printf("\n=== Testing double ===\n");
    
    Double x1 = {1.0}, y1 = {2.0}, z1 = {3.0};
    Double x2 = {4.0}, y2 = {5.0}, z2 = {6.0};
    
    Vector3D* v1 = createVectorWithElems(ofDouble(), &x1, &y1, &z1);
    Vector3D* v2 = createVectorWithElems(ofDouble(), &x2, &y2, &z2);
    
    printf("v1 = ");
    printVector(v1);
    printf("\nv2 = ");
    printVector(v2);
    printf("\n");
    
    // Тест сложения
    Vector3D* sum = vectorAdd(v1, v2);
    assert(sum != NULL);
    printf("v1 + v2 = ");
    printVector(sum);
    printf("\n");
    
    Double* sx = (Double*)sum->x;
    Double* sy = (Double*)sum->y;
    Double* sz = (Double*)sum->z;
    assert(fabs(sx->value - 5.0) < 1e-10);
    assert(fabs(sy->value - 7.0) < 1e-10);
    assert(fabs(sz->value - 9.0) < 1e-10);
    
    // Тест скалярного произведения
    Double sp = {0};
    scalarProduct(v1, v2, &sp);
    printf("scalar(v1, v2) = ");
    ofDouble()->print(&sp);
    printf("\n");
    assert(fabs((&sp)->value - (1.0*4.0 + 2.0*5.0 + 3.0*6.0)) < 1e-10);
    
    // Тест векторного произведения
    Vector3D* cross = crossProduct(v1, v2);
    assert(cross != NULL);
    printf("v1 x v2 = ");
    printVector(cross);
    printf("\n");
    
    Double* cx = (Double*)cross->x;
    Double* cy = (Double*)cross->y;
    Double* cz = (Double*)cross->z;
    assert(fabs(cx->value - (2.0*6.0 - 3.0*5.0)) < 1e-10);
    assert(fabs(cy->value - (3.0*4.0 - 1.0*6.0)) < 1e-10);
    assert(fabs(cz->value - (1.0*5.0 - 2.0*4.0)) < 1e-10);
    
    deleteVector(v1);
    deleteVector(v2);
    deleteVector(sum);
    deleteVector(cross);
    
    printf("✓ All double numbers tests passed\n");
}

void test_complex_operations() 
{
    printf("\n=== Testing complex numbers ===\n");
    
    Complex x1 = {1.0, 1.0}, y1 = {2.0, 2.0}, z1 = {3.0, 3.0};
    Complex x2 = {1.0, -1.0}, y2 = {2.0, -2.0}, z2 = {3.0, -3.0};
    
    Vector3D* v1 = createVectorWithElems(ofComplex(), &x1, &y1, &z1);
    Vector3D* v2 = createVectorWithElems(ofComplex(), &x2, &y2, &z2);
    
    printf("v1 = ");
    printVector(v1);
    printf("\nv2 = ");
    printVector(v2);
    printf("\n");
    
    // Тест сложения
    Vector3D* sum = vectorAdd(v1, v2);
    assert(sum != NULL);
    printf("v1 + v2 = ");
    printVector(sum);
    printf("\n");
    
    Complex* sx = (Complex*)sum->x;
    Complex* sy = (Complex*)sum->y;
    Complex* sz = (Complex*)sum->z;
    assert(fabs(sx->re - 2.0) < 1e-10 && fabs(sx->im) < 1e-10);
    assert(fabs(sy->re - 4.0) < 1e-10 && fabs(sy->im) < 1e-10);
    assert(fabs(sz->re - 6.0) < 1e-10 && fabs(sz->im) < 1e-10);
    
    // Тест скалярного произведения
    Complex sp;
    scalarProduct(v1, v2, &sp);
    printf("scalar(v1, v2) = ");
    ofComplex()->print(&sp);
    printf("\n");
    assert(fabs((&sp)->re - 28.0) < 1e-10 && fabs((&sp)->im) < 1e-10);

    // Тест векторного произведения
    Vector3D* cross = crossProduct(v1, v2);
    assert(cross != NULL);
    printf("v1 x v2 = ");
    printVector(cross);
    printf("\n");

    Complex* cx = (Complex*)cross->x;
    Complex* cy = (Complex*)cross->y;
    Complex* cz = (Complex*)cross->z;
    assert(fabs(cx->re) < 1e-10 && fabs(cx->im) < 1e-10);
    assert(fabs(cy->re) < 1e-10 && fabs(cy->im) < 1e-10);
    assert(fabs(cz->re) < 1e-10 && fabs(cz->im) < 1e-10);
    
    deleteVector(v1);
    deleteVector(v2);
    deleteVector(sum);
    deleteVector(cross);

    
    printf("✓ All complex numbers tests passed\n");
}

void test_error_cases() 
{
    printf("\n=== Testing error cases ===\n");
    
    Double x1 = {1.0}, y1 = {2.0}, z1 = {3.0};
    Complex x2 = {1.0, 1.0}, y2 = {2.0, 2.0}, z2 = {3.0, 3.0};
    
    Vector3D* v_double = createVectorWithElems(ofDouble(), &x1, &y1, &z1);
    Vector3D* v_complex = createVectorWithElems(ofComplex(), &x2, &y2, &z2);
    
    printf("Attempt to add vectors of different types:\n");
    Vector3D* bad_sum = vectorAdd(v_double, v_complex);
    assert(bad_sum == NULL);
    printf("\n");
    
    printf("Attempt to do a scalar product of vectors of different types:\n");
    Double bad_sp;
    int bad_sp_res = scalarProduct(v_double, v_complex, &bad_sp);
    assert(bad_sp_res == -1);
    printf("\n");
    
    printf("Attempt to do a cross product of vectors of different types:\n");
    Vector3D* bad_cross = crossProduct(v_double, v_complex);
    assert(bad_cross == NULL);
    printf("\n");
    
    printf("Attempt to operate on NULL pointers:\n");
    Double sp;
    assert(vectorAdd(NULL, v_double) == NULL);
    assert(scalarProduct(v_double, NULL, &sp) == -1);
    assert(crossProduct(NULL, NULL) == NULL);
    printf("\n");
    
    deleteVector(v_double);
    deleteVector(v_complex);
    
    printf("✓ All error cases tests passed\n");
}

void run_all_tests() 
{
    printf("========================================\n");
    printf("RUNNING TESTS\n");
    printf("========================================\n");
    
    test_double_operations();
    test_complex_operations();
    test_error_cases();
    
    printf("\n========================================\n");
    printf("ALL TESTS PASSED SUCCESFULLY\n");
    printf("========================================\n");
}