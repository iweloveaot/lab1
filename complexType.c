#include <stdio.h>
#include <stdlib.h>
#include "types.h"


void printComplex(const void* elem)
{
    const Complex *c = (const Complex*)elem;
    printf("%.3f + %.3f*i", c->re, c->im);
}


void inputComplex(void* elem)
{
    Complex *c = (Complex*)elem;
    printf("(format: re im) ");
    int code = scanf("%lf %lf", &(c->re), &(c->im));
    while (code != 2) {
        printf("Incorrect input, please repeat\n");
        while (getchar() != '\n');
        code = scanf("%lf %lf", &(c->re), &(c->im));
    }
}


void sumComplex(const void *a, const void *b, void *res)
{
    const Complex *ca = (const Complex*)a;
    const Complex *cb = (const Complex*)b;
    Complex *cres = (Complex*)res;
    cres->re = ca->re + cb->re;
    cres->im = ca->im + cb->im;
}


void multiplyComplex(const void *a, const void *b, void *res)
{
    const Complex *ca = (const Complex*)a;
    const Complex *cb = (const Complex*)b;
    Complex *cres = (Complex*)res;
    cres->re = ca->re * cb->re - ca->im * cb->im;
    cres->im = ca->re * cb->im + cb->re * ca->im;
}


void negativeComplex(void* elem)
{
    Complex *c = (Complex*)elem;
    c->re *= -1;
    c->im *= -1; 
}


void zeroComplex(void* elem)
{
    Complex *c = (Complex*)elem;
    c->re = 0;
    c->im = 0;
}


static TypeInfo complex_type =
{
    .size = sizeof(Complex),
    .print = printComplex,
    .input = inputComplex,
    .add = sumComplex,
    .multiply = multiplyComplex,
    .negative = negativeComplex,
    .zero = zeroComplex
};


TypeInfo* ofComplex()
{
    return &complex_type;
}


