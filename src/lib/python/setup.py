#
# Setup of Cython usage for libspeechcontrol.
# 
# @todo Add other extensions here.
#
# @author SpeechControl Developement <spchcntrl-devel@thesii.org>
#         Jacky Alcine <jacky.alcine@thesii.org>
#

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

# Is this right?
setup(ext_modules=[Extension(
                    "SpeechControl.Dictionary",
                    ["system.pxd", "../dictionary.cpp"],
                    language = "c++",
		    flags = "-I/usr/include/qt4")
    ],
    cmdclass={'build_ext': build_ext})
