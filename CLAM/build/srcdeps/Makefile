OBJECTS=listhash.o objdepname.o parser.o config_parser.o strptr.o includepaths.o recursedir.o list.o strfuncs.o hash.o stack.o conditions.o linmain.o
DEPENDS=$(OBJECTS:.o=.d)
PROGRAM=srcdeps
CFLAGS=-Wall -O6

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
