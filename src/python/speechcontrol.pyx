cimport cpython

from libsc cimport *

cdef unicode from_QString(c_QString qstring):
    cdef string s
    cdef char* cs
    cdef bytes bs
    s = qstring.toStdString()
    cs = s.c_str()
    bs = cs
    return bs.decode('UTF-8')

cdef c_QString to_QString(unicode pystring):
    cdef char *cs
    py_bytestring = pystring.encode('UTF-8')
    cs = py_bytestring
    return c_QString(cs)

## Yeah, this is QObject wrapper but... anyway, don't use QObject very much in the C++ code.
cdef class QObject:
    cdef c_QObject *obj
    def __cinit__(self):
        self.obj = new c_QObject()
    def __dealloc__(self):
        del self.obj

cdef class Core:
    cdef c_Core *core
    def __cinit__(self):
        self.core = new c_Core(NULL, NULL, NULL)
    def __dealloc__(self):
        del self.core
    def __init__(self, argc, argv, QObject parent):
        del self.core
        self.core = new c_Core(NULL, NULL, NULL)

    def confPath(self):
        return from_QString(self.core.confPath())

    def test(self):
        self.core.test()

## SpeechControl::Audio
cdef class FileRecorder:
    cdef c_FileRecorder *fr
    def __cinit__(self, unicode fileName):
        self.fr = new c_FileRecorder(to_QString(fileName), NULL)
    def __dealloc__(self):
        del self.fr

    def setFile(self, unicode fileName):
        self.fr.setFile(to_QString(fileName))

## SpeechControl::Sphinx
