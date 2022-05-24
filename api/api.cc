#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
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
typedef vector<int> vec_i;
typedef map<int, double> map_id;
typedef map<string, map_id> map_s_id;

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
    map_id data;
    for(int i = 1; i <= 10 ; i++) {
        data[i] = i * i * 3.14;
    }

    PyObject* ret_dict = PyDict_New();
    for(map_id::iterator it = data.begin(); it != data.end(); it++) {
        PyObject* new_key = PyLong_FromLong(it -> first);
        PyObject* new_val = PyFloat_FromDouble(it -> second);
        int state = PyDict_SetItem(ret_dict, new_key, new_val);
        Py_XDECREF(new_key);
        Py_XDECREF(new_val);

        if(state) {
            printf("cannot insert element '%i': '%f' into Python dict.\n", 
            it->first, it -> second);
        }
    }

    return ret_dict;
}

PyObject* get_list(PyObject* self, PyObject* args) {
    // transform C++ map to the Python dict
    vec_i data;
    for(int i = 1; i <= 10 ; i++) {
        data.push_back(i * i * 3.14);
    }

    Py_ssize_t len = 0;  // initial
    PyObject* ret_list = PyList_New(len);
    for(vec_i::iterator it = data.begin(); it != data.end(); it++) {
        PyObject* new_val = PyFloat_FromDouble(*it);
        int state = PyList_Append(ret_list, new_val);
        Py_XDECREF(new_val);

        if(state) {
            printf("cannot insert element '%i': '%f' into Python list.\n", *it);
        }
    }

    return ret_list;
}

PyObject* get_advmap(PyObject* self, PyObject* args) {
    // transform C++ map to the Python dict
    map_s_id data;
    stringstream ss;    
    for(int v = 0; v <= 20; v += 10) {
        map_id details;
        ss.str("");  // clear
        ss << 's' << v;
        for(int i = 1; i <= 10 ; i++) {
            details[v + i] = (v + i) * (v + i) * 3.14;
        }
        data[ss.str()] = details;
    }

    PyObject* ret_dict = PyDict_New();
    int state = 0;
    for(map_s_id::iterator it = data.begin(); it != data.end(); it++) {
        PyObject* new_key = PyUnicode_FromString((it -> first).c_str());
        PyObject* pairdict = PyDict_New();
        for(map_id::iterator im = (it -> second).begin(); im != (it -> second).end(); im ++) {
            PyObject* new_p_key = PyLong_FromLong(im -> first);
            PyObject* new_p_val = PyFloat_FromDouble(im -> second);
            state = PyDict_SetItem(pairdict, new_p_key, new_p_val);
            Py_XDECREF(new_p_key);
            Py_XDECREF(new_p_val);

            if(state) {
                printf("cannot insert element '%i': '%f' into Python dict.\n", 
                im->first, im -> second);
            }
        }
        state = PyDict_SetItem(ret_dict, new_key, pairdict);
        Py_XDECREF(new_key);
        Py_XDECREF(pairdict);        

        if(state) {
            printf("cannot insert element '%i' into Python higher dict.\n", it->first);
        }        
    }

    return ret_dict;
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
    { "get_advmap", (PyCFunction)get_advmap, METH_VARARGS, nullptr },    
    { "get_list", (PyCFunction)get_list, METH_VARARGS, nullptr }, 

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

