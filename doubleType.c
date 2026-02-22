#include <stdio.h>
#include <stdlib.h>
#include "types.h"


void printDouble(const void* a)
{
    const Double *da = (const Double*)a;
    printf("%.3f", da->value);
}


void inputDouble(void* elem)
{
    Double *d = (Double*)elem;
    scanf("%lf", &(d->value));
}

void sumDouble(const void *a, const void *b, void *res)
{
    const Double *da = (const Double*)a;
    const Double *db = (const Double*)b;
    Double *dres = (Double*)res;
    dres->value = da->value + db->value;
}


void multiplyDouble(const void *a, const void *b, void *res)
{
    const Double *da = (const Double*)a;
    const Double *db = (const Double*)b;
    Double *dres = (Double*)res;
    dres->value = da->value * db->value;
}


void negativeDouble(void* elem)
{
    Double *d = (Double*)elem;
    d->value *= -1; 
}


void zeroDouble(void* elem)
{
    Double *d = (Double*)elem;
    d->value = 0;
}


static TypeInfo double_type =
{
    .size = sizeof(Double),
    .print = printDouble,
    .input = inputDouble,
    .add = sumDouble,
    .multiply = multiplyDouble,
    .negative = negativeDouble,
    .zero = zeroDouble
};


TypeInfo* ofDouble()
{
    return &double_type;
}







