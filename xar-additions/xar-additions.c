#include <xar/xar.h>

extern int32_t xar_arcmod_extract(xar_t x, xar_file_t f, const char *file, char *buffer, size_t len);

/* xar_extract_tobuffer
* x: archive to extract from
* buffer: buffer to extract to
* size: On return, this will contain the size of the memory pointed to by buffer
* Returns 0 on success, -1 on failure.
* Summary: This is the entry point for extraction to a buffer.
* On success, a buffer is allocated with the contents of the file
* specified.  The caller is responsible for freeing the returend buffer.
* Example: xar_extract_tobuffer(x, "foo/bar/blah",&buffer)
*/
int32_t xar_extract_tobuffersz(xar_t x, xar_file_t f, char **buffer, size_t *size) {
	const char *sizestring = NULL;
	int32_t ret;
	
	if(0 != xar_prop_get(f,"data/size",&sizestring)){
		if(0 != xar_prop_get(f, "type", &sizestring))
			return -1;
		if(strcmp(sizestring, "file") == 0) {
			*size = 0;
			return 0;
		}
		return -1;
	}

	*size = strtoull(sizestring, (char **)NULL, 10);
	*buffer = malloc(*size);
	
	if(!(*buffer)){
		return -1;
	}
	
	ret = xar_arcmod_extract(x,f,NULL,*buffer,*size);
	if( ret ) {
		*size = 0;
		free(*buffer);
		*buffer = NULL;
	}

	return ret;
}
