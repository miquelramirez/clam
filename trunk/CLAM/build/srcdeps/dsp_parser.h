#ifndef __dsp_parser__
#define __dsp_parser__


void dsp_parse_inplace(const char* filename);

//! the same as dsp_parse_inplace. @deprecated.
void dsp_parse(const char* filename);
void dsp_parse_from_empty(const char* outFilename);
void dsp_parse_from_file(const char* inFilename, const char* outFilename);

void dsp_parse_from_empty(const char* outFilename);
void dsp_parse_from_file(const char* inFilename, const char* outFilename);

#endif
