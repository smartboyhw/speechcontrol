from libcpp cimport string

# Qt
cdef extern from "<QObject>":
    cdef cppclass cQObject "QObject":
        cQObject()
        cQObject(cQObject *)

cdef extern from "<QString>":
    cdef cppclass cQString "QString":
        cQString()
        cQString(char *)

        std::string toStdString()

# Core
cdef extern from "../lib/core.hpp" namespace "SpeechControl":
    cdef cppclass cCore "SpeechControl::Core":
        cCore(cQObject *)
        cCore(int *, char ***, cQObject *)

        QString confPath()
        void setup() #@staticmethod

        cCore *s_inst
        cCore* instance() #@staticmethod

#cdef class QString:
#    cdef cQString *s

cdef class Core:
    cdef cCore *core

# Audio classes
cdef extern from "../lib/audio/devicemanager.hpp" namespace "SpeechControl::Audio":
    cdef cppclass cDeviceManager "SpeechControl::Audio::DeviceManager"

cdef extern from "../lib/audio/filerecorder.hpp" namespace "SpeechControl::Audio":
    cdef cppclass cFileRecorder "SpeechControl::Audio::FileRecorder"

# Sphinx classes
