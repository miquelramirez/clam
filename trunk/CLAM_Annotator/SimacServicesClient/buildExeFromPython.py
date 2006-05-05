#setup.py
from distutils.core import setup
import sys
import py2exe

sys.argv += "py2exe -b 1".split(" ")

setup(console=[{
	'script':'Manager.py', 
	'dest_base':'BocaTaskManager',
	'includes' : ['dbhash','xml.parser.expat'],
	}])

