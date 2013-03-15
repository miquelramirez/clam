#ifndef EmbeddedFile_hxx
#define EmbeddedFile_hxx

/// Include a given file as binary data
/// @arg varname is the variable that will hold the var (const char[])
/// @arg file is an absolute path or a relative path from the build point (not the source file!!)
/// @todo Try to make CLAM_EMBEDDED_FILE work with path relatives to the source that uses the macro.
///
/// @section Adding CLAM_EMBEDDED_FILE support for your platform
///
/// If you want to help supporting CLAM_EMBEDDED_FILE on your platform,
/// please send CLAM developers the output of the following command:
/// @code
/// echo 'const char mydata[]="content";' | gcc -x c - -S -o -
/// @endcode
/// That's a good starting point for the assembler part. Normally it just
/// requires to substitute the directive containing the "content" literal
/// by the .incbin directive, appending the .byte 0 directive and
/// deducing the size with address math.
///

#if defined __MINGW32__
// COFF binary target version
#define CLAM_EMBEDDED_FILE(varname, file) \
asm ( \
"  .section \".rodata\"\n" \
"  .def _" #varname "\n" \
" _" #varname ":\n" \
"   .incbin \"" file "\"\n" \
"   .byte 0\n" \
"   .scl 2\n" \
"   .size . - _" #varname "\n" \
" .enddef\n" \
); \
extern const char varname[];
// This was needed but at some version it made it not to compile
// "   .scl 1\n"


#elif defined __ELF__
// ELF binary target version

#define CLAM_EMBEDDED_FILE(varname, file) \
asm ( \
"  .section \".rodata\"\n" \
"   .type " #varname ", @object\n" \
" " #varname ":\n" \
"   .incbin \"" file "\"\n" \
"   .byte 0\n" \
"   .size " #varname ", .-" #varname "\n" \
); \
extern const char varname[];

// WIP
#elif 0 && defined __MACH__ && defined __APPLE__

#define CLAM_EMBEDDED_FILE(varname, file) \
asm ( \
".cstring \n" \
"	.globl  _" #varname " \n" \
"_" #varname ": \n" \
"	.asciz   "mycontent" \n" \
"	.byte 0 \n" \
"_" #varname "_end: \n" \
); \
extern const char varname[];


#else
#error No support for embedded files in your platform.
#define CLAM_EMBEDDED_FILE(varname, file) \
extern const char varname[];
#endif


/// Alias to CLAM_EMBEDDED_FILE, for compatibility
#define CLAM_EXTERNAL_FILE_DATA(varname, file) CLAM_EMBEDDED_FILE(varname, file)


#endif//EmbeddedFile_hxx
