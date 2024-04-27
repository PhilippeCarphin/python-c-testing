#include <Python.h>

struct MyIterator {
    PyObject_HEAD
    int current;
    int *previous_primes;
    size_t previous_primes_len;
    size_t previous_primes_capacity;
};

static int MyIterator_AddPreviousPrime(struct MyIterator *self, int n)
{
    if(self->previous_primes_len == self->previous_primes_capacity)
    {
        int new_capacity = self->previous_primes_capacity * 2;
        int *new_array = realloc(self->previous_primes, sizeof(*self->previous_primes) * new_capacity);
        if(new_array == NULL){
            return 1;
        }
        self->previous_primes = new_array;
        self->previous_primes_capacity = new_capacity;
    }

    self->previous_primes[self->previous_primes_len++] = n;

    return 0;
}

static PyObject *MyIterator_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    struct MyIterator *self = (struct MyIterator*) type->tp_alloc(type, 0);

    if(self == NULL){
        return NULL;
    }

    self->previous_primes_capacity = 10;
    self->previous_primes = malloc( sizeof(*self->previous_primes) * self->previous_primes_capacity);
    if(self->previous_primes == NULL){
        type->tp_free(self);
        return NULL;
    }
    self->previous_primes_len = 0;
    self->current = 1;

    return (PyObject *)self;
}

static void MyIterator_dealloc(struct MyIterator *self)
{
    free(self->previous_primes);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static int MyIterator_IsPrime(struct MyIterator *self, int n)
{
    for(int i = 0; i < self->previous_primes_len; i++){
        int pp = self->previous_primes[i];
        if(pp*pp > n){
            break;
        }

        if(n % pp == 0){
            return 0;
        }
    }

    return 1;
}

static PyObject *MyIterator_iternext(struct MyIterator *self){
    self->current += 1;
    while(!MyIterator_IsPrime(self, self->current)){
        self->current += 1;
    }
    int err = MyIterator_AddPreviousPrime(self, self->current);
    if(err){
        PyErr_SetString(PyExc_RuntimeError, "Something went wrong adding previous prime to list");
        return NULL;
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
    .tp_dealloc = (destructor) MyIterator_dealloc,
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

