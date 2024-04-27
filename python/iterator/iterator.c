#include <Python.h>

struct MyIterator {
    PyObject_HEAD
    int current;
};

static PyObject *MyIterator_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    struct MyIterator *self = (struct MyIterator*) type->tp_alloc(type, 0);

    if(self == NULL){
        return NULL;
    }

    return (PyObject *)self;
}

static PyTypeObject MyIterator_type = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_basicsize = sizeof(struct MyIterator),
    .tp_name = "iterator.MyIterator",
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_itemsize = 0,
    .tp_new = MyIterator_new,
};

static PyModuleDef iterator_module_def = {
    PyModuleDef_HEAD_INIT,
    .m_name = "iterator",
    .m_doc = "Module providing an iterator",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit__iterator(void)
{
    PyObject *m = PyModule_Create(&iterator_module_def);

    if(m == NULL){
        return NULL;
    }

    if(PyType_Ready(&MyIterator_type) < 0){
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&MyIterator_type);
    if(PyModule_AddObject(m, "MyIterator", (PyObject *) &MyIterator_type) < 0){
        Py_DECREF(m);
        Py_DECREF(&MyIterator_type);
        return NULL;
    }

    return m;
}

