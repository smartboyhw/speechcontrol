#
# Cython definition of SpeechControl::Dictionary (dictionary.hpp)
#

cdef extern from "../dictionary.hpp" namespace "SpeechControl":
    cdef cppclass DictionaryEntry:
        DictionaryEntry(Dictionary, char, char)
        char word()
        char phoneme()

    cdef cppclass Dictionary:
        Dictionary()