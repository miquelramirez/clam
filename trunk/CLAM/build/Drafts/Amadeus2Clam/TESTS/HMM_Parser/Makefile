include ../../clam-top.cfg

# Top points to the root directory of your project
TOP = ../../$(CLAM_TOP)

include $(TOP)/build/Makefile.rules

depend:  extra_sources/lex.HTK_.cxx  extra_sources/HTK_Parser.tab.cxx

extra_sources/lex.HTK_.cxx: $(TOP)/$(TOP_SOURCE)/tool/HTKLexerParser/HTK_LexerwBison.lex
	@echo Flexing HTK_LexerBison.lex
	flex -i -PHTK_ -o$@ $<

extra_sources/HTK_Parser.tab.cxx: $(TOP)/$(TOP_SOURCE)/tool/HTKLexerParser/HTK_Parser.yxx
	@echo Generating the HTK parser
	bison -d -v -pHTK_ -o $@ $< 



# The program name to compile and its settings must be in the 
# settings.cfg at this same directory

