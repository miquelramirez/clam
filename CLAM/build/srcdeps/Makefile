OBJECTS=listhash.o objdepname.o parser.o config_parser.o strptr.o includepaths.o recursedir.o list.o strfuncs.o hash.o stack.o conditions.o main.o makegen.o dsp_parser.o vcproj_parser.o tree.o emptydsp.o emptyvcproj.o

DEPENDS=$(OBJECTS:.o=.d)
PROGRAM=srcdeps
CFLAGS=-Wall -g

all: $(PROGRAM)

clean:
	rm -f srcdeps $(OBJECTS) $(DEPENDS)

depend:
	rm -f $(DEPENDS)

ifneq ($(MAKECMDGOALS),depend)
ifneq ($(MAKECMDGOALS),clean)
include $(DEPENDS)
endif
endif

$(PROGRAM): $(OBJECTS) mystrdup.o
	gcc $(OBJECTS) mystrdup.o  -o $(PROGRAM)

mystrdup.o: mystrdup.c
	gcc -c $(CFLAGS) $< -o $@


%.o: %.c
	gcc -c $(CFLAGS) $< -o $@



%.d: %.c
	gcc -M $< -o $@
