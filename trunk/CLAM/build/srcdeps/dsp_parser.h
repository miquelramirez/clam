#ifndef __dsp_parser__
#define __dsp_parser__


/* Entry point for generating a .dsp file from
 * the information extracted by srcdeps' parser
 * from the config file and the associated sources.
 */
extern void dsp_parse(const char* filename);

#endif
