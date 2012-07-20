# Standard library
cdef extern from "<string>":
    cdef cppclass string:
        string()
        string(char *)
        char* c_str()

# Qt
cdef extern from "<QObject>":
    cdef cppclass c_QObject "QObject":
        c_QObject()
        c_QObject(c_QObject *)

cdef extern from "<QString>":
    cdef cppclass c_QString "QString":
        c_QString()
        c_QString(char *)

        string toStdString()

# Core
cdef extern from "../lib/core.hpp" namespace "SpeechControl":
    cdef cppclass c_Core "SpeechControl::Core":
        c_Core(c_QObject *)
        c_Core(int *, char ***, c_QObject *)

        c_QString confPath()

cdef extern from "../lib/core.hpp" namespace "SpeechControl::Core":
        c_Core *s_inst
        c_Core* instance()
        void setup()

# Audio classes
cdef extern from "../lib/audio/devicemanager.hpp" namespace "SpeechControl::Audio":
    cdef cppclass c_DeviceManager "SpeechControl::Audio::DeviceManager"

cdef extern from "../lib/audio/filerecorder.hpp" namespace "SpeechControl::Audio":
    cdef cppclass c_FileRecorder "SpeechControl::Audio::FileRecorder"

# Sphinx classes
