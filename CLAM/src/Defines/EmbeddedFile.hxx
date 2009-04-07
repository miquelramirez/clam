#ifndef EmbeddedFile_hxx
#define EmbeddedFile_hxx

/// Include a given file as binary data
/// @arg varname is the variable that will hold the var (const char[])
/// @arg file is an absolute path or a relative path from the build point (not the source file!!)
/// @todo Try to make CLAM_EXTERNAL_FILE_DATA work with path relatives to the source that uses the macro.
#define CLAM_EMBEDDED_FILE(varname, file) \
asm ( \
"  .section \".rodata\"\n" \
"   .type " #varname ", @object\n" \
" " #varname ":\n" \
"   .incbin \"" file "\"\n" \
"   .byte 0\n" \
"   .size " #varname ", .-" #varname "\n" \
); \
extern const char varname[]; \

/// Alias to CLAM_EMBEDDED_FILE, for compatibility
#define CLAM_EXTERNAL_FILE_DATA(varname, file) CLAM_EMBEDDED_FILE(varname, file)


#endif//EmbeddedFile_hxx
