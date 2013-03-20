#ifndef UTIL_H
#define UTIL_H

/* Reads in an entire file. Returns NULL on error, or a malloc'd pointer to a
 * string which should later be freed. */
char *read_file(const char *filename);

#endif
