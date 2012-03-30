#
# Cython definition of SpeechControl::System (system.hpp)
#

cdef extern from "../system.hpp" namespace "SpeechControl":
    cdef cppclass System:
        System(int, char[])
        void start()
        void stop()