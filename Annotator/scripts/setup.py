from distutils.core import setup
import py2exe
includes = ["win32com.client", "encodings.*",
			"xml.parsers.expat", "xml.parsers.pyexpat", "xml.dom.ext.reader.PyExpat",
			"xml.sax.drivers.*", "xml.sax.drivers2.*"]
setup(
	options = {"py2exe": {"includes":includes}},
	console=['AggregationExtractor.py'])