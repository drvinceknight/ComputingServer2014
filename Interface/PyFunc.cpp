// Contains the defintion for the python function class

#include "PyFunc.h"


// Constructor automatically pulls function from python if needed
PyFunc::PyFunc(const char* ModuleName, const char* FuncName)
{
    Py_Initialize();

    // Convert the name of the module into something python understands
    pName = PyString_FromString(ModuleName);

    // Import said module
    pModule = PyImport_Import(pName);


    // If import was succesful continue else throw a wobbly
    if (pModule != NULL)
    {
        std::cout << "Loading function ...\n";
        // Now get the function from the file
        pFunc = PyObject_GetAttrString(pModule, FuncName);

        // Check that it worked
        if (pFunc && PyCallable_Check(pFunc))
        {
            std::cout << "[PyFunc][INFO]: Succesfully loaded function " << FuncName 
                << " from " << ModuleName << " module\n";
            isValid = true;
        }
        else
        {
           std::cout << "[PyFunc][ERROR]: " << FuncName << " not a valid function or not in " << ModuleName << std::endl;
           isValid = false;
        }
    }
    else
    {
        std::cout << "[PyFunc][ERROR]: Unable to load module " << ModuleName << std::endl;
        isValid = false;
    }
}

// Destructor,
PyFunc::~PyFunc()
{
    Py_Finalize();
}

// Calls the function it has loaded, note this is only capable of handling 
// functions that take no arguments and returns a list of ints
int* PyFunc::callFunction()
{
    if (validFunc())
    {
        // Call the function and catch the return value
        pValue = PyObject_CallObject(pFunc, NULL);

        // Check that data it received and in proper format
        if (PyList_Check(pValue))
        {
            // Get the length of the list and convert it to C++ data
            int length = PyList_Size(pValue);


            // Reserve space for the array
            int* values = new int[length];

            // Convert the values and add them to the array 
            for (int i = 0; i < length; i++)
            {
                // Get a value from the list
                values[i] = PyInt_AsLong(PyList_GetItem(pValue, i));
            }
            return values;

        }
        else 
        {
            std::cout << "Error no data returned or in wrong format\n";
            return NULL;
        }
    }
    else
    {
        std::cout << "[PyFunc][ERROR]: Attempted call to undefined function, doing nothing\n";
        return NULL;
    }

}

bool PyFunc::validFunc()
{
    return isValid;
}

/* An example program
 *
 *
    #include "PyFunc.h"

    int main ()
    {
        PyFunc func("hello", "hello");

        int* ret = func.callFunction();

        for (int i = 0; i < 10; i++)
        {
            std::cout << ret[i] << std::endl;
        }
        return 0;
    }
    
    And the python file

    hello.py:

    def hello();
        return [1,2,3,4,5,6,7,8,9,10]

*/