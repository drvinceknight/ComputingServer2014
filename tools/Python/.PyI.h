#ifndef PYTHON_INTERPRETER_CPP
#define PYTHON_INTERPRETER_CPP

#include <python2.7/Python.h>

#include "PyFunc.h"
#include "../tools/LinkedList.h"

class PyI
{
    private:
        LinkedList<PyFunc>* funcList;

    public:

        PyI();
        ~PyI();

        void addFunction(PyFunc* pFunc);


};

#endif
