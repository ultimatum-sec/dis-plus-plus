#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

// If this file'll be compiled with C++ compiler
#ifdef __cplusplus
extern "C"
{
#endif

struct mapfile_t
{
	void *mptr;
	off_t msize;
} mapped = {.mptr = MAP_FAILED, .msize = -1};

struct mapfile_t *mopen(char *path)
{
	int fd = open(path, O_RDWR);
    if (fd == -1)
        return NULL;

   	mapped.msize = lseek(fd, 0, SEEK_END);
	if (mapped.msize == -1)
		return NULL;
	
    mapped.mptr = mmap
	(
		NULL,
		mapped.msize,
		PROT_READ,
		MAP_FILE | MAP_PRIVATE,
		fd,
		0
	);
	
	close(fd);

	if (mapped.mptr == MAP_FAILED)
		return NULL;
	return &mapped;
}

int mclose(void)
{
	if ((mapped.mptr != MAP_FAILED && mapped.mptr != NULL) && mapped.msize != -1)
		return munmap(mapped.mptr, mapped.msize);
	return -1;
}

#ifdef __cplusplus
}
#endif
