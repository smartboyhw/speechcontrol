# Standard library
cdef extern from "<string>":
    cdef cppclass string:
        string()
        string(char *)
        char* c_str()

# Qt
cdef extern from "<QObject>":
    cdef cppclass cQObject "QObject":
        cQObject()
        cQObject(cQObject *)

cdef extern from "<QString>":
    cdef cppclass cQString "QString":
        cQString()
        cQString(char *)

        string toStdString()

# Core
cdef extern from "../lib/core.hpp" namespace "SpeechControl":
    cdef cppclass cCore "SpeechControl::Core":
        cCore(cQObject *)
        cCore(int *, char ***, cQObject *)

        cQString confPath()

cdef extern from "../lib/core.hpp" namespace "SpeechControl::Core":
        cCore *s_inst
        cCore* instance()
        void setup()

# Audio classes
cdef extern from "../lib/audio/devicemanager.hpp" namespace "SpeechControl::Audio":
    cdef cppclass cDeviceManager "SpeechControl::Audio::DeviceManager"

cdef extern from "../lib/audio/filerecorder.hpp" namespace "SpeechControl::Audio":
    cdef cppclass cFileRecorder "SpeechControl::Audio::FileRecorder"

# Sphinx classes
