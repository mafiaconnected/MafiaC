#pragma once
#include "common.h"
template <typename T> 
void *FunctionPointerToVoidP(T func) {
    union {
        T a;
        void *b;
    } c = { func };
    return c.b;
}