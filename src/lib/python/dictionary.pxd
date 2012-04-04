cdef extern from "../dictionary.hpp" from namespace SpeechControl
    cdef cppclass DictionaryEntry:
        word()
        phoneme()

## @todo Export the rest of Dictionary to Cython.
cdef extern from "../dictionary.hpp" from namespace SpeechControl:
    cdef cppclass Dictionary:
        Dictionary()
        Dictionary(id)
        addEntry(entry)