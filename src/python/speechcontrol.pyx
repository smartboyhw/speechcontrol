cimport cpython

from libsc cimport *

cdef unicode from_QString(c_QString qstring):
    cdef string s
    s = qstring.toStdString()
    cs = s.c_str()
    return cs.decode('UTF-8')

## Yeah, this is QObject wrapper but... anyway, don't use QObject very much in the C++ code.
cdef class QObject:
    cdef c_QObject *obj
    def __cinit__(self):
        self.obj = new c_QObject()
    def __dealloc__(self):
        del self.obj
    def __init__(self, QObject qobj):
        del self.obj
        self.obj = qobj.obj

cdef class Core:
    cdef c_Core *core
    def __cinit__(self):
        self.core = new c_Core(NULL)
    def __dealloc__(self):
        del self.core
    def __init__(self, argc, argv, QObject parent):
        del self.core
        self.core = new c_Core(NULL, NULL, NULL)

    def confPath(self):
        return from_QString(self.core.confPath())

