#include "dl.h"
#include <iostream>

DL* DL::obj = nullptr;
DL* DL::getInstance() {
    if (obj == nullptr) {
        obj = new DL();
    }
    return obj;
}

void DL::method1() {
    std::cout << "hello dl2 method1" << std::endl;
}

extern "C" DL* getInstance() {
    return DL::getInstance();
}

extern "C" void method1(DL* instance) {
    instance->method1();
}
