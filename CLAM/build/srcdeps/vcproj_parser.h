#ifndef __vcproj_parser__
#define __vcproj_parser__


/* Entry point for generating a .vcproj file from
 * the information extracted by srcdeps' parser
 * from the config file and the associated sources.
 */
extern void vcproj_parse(const char* outFilename);

#endif
