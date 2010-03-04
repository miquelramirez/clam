#ifndef EmbeddedFile_hxx
#define EmbeddedFile_hxx

/// Include a given file as binary data
/// @arg varname is the variable that will hold the var (const char[])
/// @arg file is an absolute path or a relative path from the build point (not the source file!!)
/// @todo Try to make CLAM_EMBEDDED_FILE work with path relatives to the source that uses the macro.

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
" .endef\n" \
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
#else
#error "No support for embedded files in your platform"
#define CLAM_EMBEDDED_FILE(varname, file) \
extern const char varname[];
#endif


/// Alias to CLAM_EMBEDDED_FILE, for compatibility
#define CLAM_EXTERNAL_FILE_DATA(varname, file) CLAM_EMBEDDED_FILE(varname, file)


#endif//EmbeddedFile_hxx
