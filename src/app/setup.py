from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules = [Extension('sc-core', ['sc-core.pyx'])]

setup(name='speechcontrol',
        version='0.1',
        description='Integrated voice control interface.',
        cmdclass = {'build_ext': build_ext}
        ext_modules = ext_modules
        )
