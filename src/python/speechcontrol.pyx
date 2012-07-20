cimport python_unicode
from libcpp cimport string

from libsc cimport *

#cdef class QString:
#    def __cinit__(self):
#        self.s = new cQString()
#    def __init__(self, char *cs):
#        del self.s
#        self.s = new cQString(cs)
#    def __dealloc(self):
#        del self.s
cdef unicode qstr(qstring):
    cdef std::string s
    s = qstring.toStdString()
    cs = s.c_str()
    return cs.decode('UTF-8')

cdef class Core:
    def __cinit__(self):
        self.core = new cCore(NULL)
    def __init__(self, int *argc, char ***argv, cQObject *parent):
        del self.core
        self.core = new cCore(argc, argv, parent)
    def __dealloc__(self):
        del self.core

    def confPath(self):
        return qstr(self.core.confPath())

