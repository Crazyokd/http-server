#ifndef _DL_H_
#define _DL_H_
void invoke_method( char *lib, char *method, float argument );

class DL
{
    static DL* obj;
    DL() = default;
public:
    static DL* getInstance();
    void method1();
};

#endif