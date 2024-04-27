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

    self->current = 1;

    return (PyObject *)self;
}

static int is_prime(int n)
{
    for(int i = 2; i*i <= n ; i++){
        if(n % i == 0){
            return 0;
        }
    }

    return 1;
}

static PyObject *MyIterator_iternext(struct MyIterator *self){
    self->current += 1;
    while(!is_prime(self->current)){
        self->current += 1;
    }
    return PyLong_FromLong(self->current);
}

static PyObject *MyIterator_iter(struct MyIterator *self){
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyTypeObject MyIterator_type = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_basicsize = sizeof(struct MyIterator),
    .tp_name = "iterator.MyIterator",
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_itemsize = 0,
    .tp_new = MyIterator_new,
    .tp_iternext = (iternextfunc) MyIterator_iternext,
    .tp_iter = (getiterfunc) MyIterator_iter,
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

