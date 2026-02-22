#ifndef _TYPEINFO_H_
#define _TYPEINFO_H_

typedef struct TypeInfo
{
    size_t size;
    void (*print)(const void*);
    void (*input)(void*);
    void (*add)(const void*, const void*, void*);
    void (*multiply)(const void*, const void*, void*);
    void (*negative)(void*);
    void (*zero)(void*);
} TypeInfo;


typedef struct Double
{
    double value;
} Double;


typedef struct Complex
{
    double re;
    double im;
} Complex;

TypeInfo* ofDouble();
TypeInfo* ofComplex();


#endif // _TYPEINFO_H_
