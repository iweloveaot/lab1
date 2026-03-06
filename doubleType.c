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
    int scaned = scanf("%lf", &(d->value));
    while (scaned != 1) {
        printf("Incorrect input, please repeat\n");
        while (getchar() != '\n');
        scaned = scanf("%lf", &(d->value));
    }
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


void multiplyCoefDouble(const void *elem, const double coef, void *res)
{
    const Double *d = (Double*)elem;
    Double *dres = (Double*)res;
    dres->value = d->value * coef;
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


void unitDouble(void* elem)
{
    Double *d = (Double*)elem;
    d->value = 1;
}


static TypeInfo double_type =
{
    .size = sizeof(Double),
    .print = printDouble,
    .input = inputDouble,
    .add = sumDouble,
    .multiply = multiplyDouble,
    .multiply_coef = multiplyCoefDouble,
    .negative = negativeDouble,
    .zero = zeroDouble,
    .unit = unitDouble
};


TypeInfo* ofDouble()
{
    return &double_type;
}







