#include <iostream>
#include <cstdlib>
#include <string>
#include <map>

/* step.1 include the Python.h */
#include <Python.h>
#include "infer.hpp"

/*
https://stackoverflow.com/questions/31018038/python-c-api-passing-two-functions-of-many-parameters-with-special-types-as-mod
https://docs.python.org/3/c-api/structures.html?highlight=pyobject#c.Py_REFCNT
http://web.mit.edu/people/amliu/vrut/python/ext/parseTuple.html
*/

using namespace std;

/* step.2 define the C++ functions */
PyObject* get_long(PyObject* self, PyObject* o) {
    long v = PyLong_AsLong(o);
    return PyLong_FromLong(v);
}

int* get_int(PyObject* self, int* v) {
    return v;
}

PyObject* parse_args(PyObject* module, PyObject* args) {
    int k, l;
    char *s[50];

    if (! PyArg_ParseTuple(args, "iis", &k, &l, &s)) {
        cout << "Error!" << endl;
    }

    cout << "String: " << *s << endl;
    return PyLong_FromLong(static_cast<long>(k + l));
}

PyObject* add_and_mul(PyObject* self, PyObject* args) {
    int v1, v2;

    if (! PyArg_ParseTuple(args, "ii", &v1, &v2)) {
        cout << "Error!" << endl;
        return NULL;
    }    

    int result = add_mul(v1, v2);
    return PyLong_FromLong(static_cast<long>(result));
}

PyObject* get_map(PyObject* self, PyObject* args) {
    // transform C++ map to the Python dict
    map<int, double> data;
    for(int i = 1; i <= 10 ; i++) {
        data[i] = i * i * 3.14;
    }

    PyObject* retdict = PyDict_New();
    for(map<int, double>::iterator it = data.begin(); it != data.end(); it++) {
        PyObject* new_key = PyLong_FromLong(it -> first);
        PyObject* new_val = PyFloat_FromDouble(it -> second);
        int state = PyDict_SetItem(retdict, new_key, new_val);
        Py_XDECREF(new_key);
        Py_XDECREF(new_val);

        if(state) {
            printf("cannot insert element '%i': '%f' into Python dict.", 
            it->first, it -> second);
        }
    }

    return retdict;
}

/* step.3 Define the structure for how to express C++ method to Python */
static PyMethodDef apis_methods[] = {
    // The first property is the name exposed to Python, int_add_mul
    // The second is the C++ function with the implementation
    // METH_O means it takes a single PyObject argument
    // METH_VARARGS means it takes multiple variables
    { "get_long", (PyCFunction)get_long, METH_O, nullptr },    
    { "get_int", (PyCFunction)get_int, METH_O, nullptr },
    { "parse_args", (PyCFunction)parse_args, METH_VARARGS, nullptr },
    { "int_add_mul", (PyCFunction)add_and_mul, METH_VARARGS, nullptr },
    { "get_map", (PyCFunction)get_map, METH_VARARGS, nullptr },    

    // Terminate the array with an object containing nulls.
    { nullptr, nullptr, 0, nullptr }
};

/* step.4 Define another structure for how to import module in Python, 
   like `from ... import ...`*/
static PyModuleDef apis_module = {
    PyModuleDef_HEAD_INIT,
    "mapi",                                 // Module name to use with Python import statements
    "access multiple shared libraries",     // Module description
    0,
    apis_methods                            // Structure that defines the methods of the module
};

/* step.5 The way of loading modules in Python.
   Must name as `PyInit_<module-name>`.*/
PyMODINIT_FUNC PyInit_mapi() {
    return PyModule_Create(&apis_module);
}

