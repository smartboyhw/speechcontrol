cimport cpython

from libsc cimport *

cdef unicode qstr(cQString qstring):
    cdef string s
    s = qstring.toStdString()
    cs = s.c_str()
    return cs.decode('UTF-8')

## Yeah, this is QObject wrapper but... anyway, don't use QObject very much in the C++ code.
cdef class QObject:
    cdef cQObject *obj
    def __cinit__(self):
        self.obj = new cQObject()
    def __dealloc__(self):
        del self.obj
    def __init__(self, QObject qobj):
        del self.obj
        self.obj = qobj.obj

cdef class Core:
    cdef cCore *core
    def __cinit__(self):
        self.core = new cCore(NULL)
    def __init__(self, argc, argv, QObject parent):
        del self.core
        self.core = new cCore(NULL, NULL, NULL)
    def __dealloc__(self):
        del self.core

    def confPath(self):
        return qstr(self.core.confPath())

