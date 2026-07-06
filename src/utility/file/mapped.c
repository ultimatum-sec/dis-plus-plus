#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

// If this file'll be compiled with C++ compiler
#ifdef __cplusplus
extern "C"
{
#endif

struct mapfile_t mopen(const char *path);
int mclose(mapfile_t);

struct mapfile_t mopen(const char *path)
{
	mapped_t mapped = { .mptr = NULL, .msize = 0 };

	int fd = open(path, O_RDWR);
    if (fd == -1)
        return mapped;

   	off_t size = lseek(fd, 0, SEEK_END);
	if (size == -1)
		return mapped;
	
    void *ptr = mmap
	(
		NULL,
		size,
		PROT_READ,
		MAP_FILE | MAP_PRIVATE,
		fd,
		0
	);
	
	close(fd);

	if (ptr == MAP_FAILED)
		return NULL;

	mapped.mptr = ptr;
	mapped.msize = size;
	return mapped;
}

int mclose(mapped_t mapped)
{
	if ((mapped.mptr != MAP_FAILED && mapped.mptr != NULL) && mapped.msize != -1)
		return munmap(mapped.mptr, mapped.msize);
	return -1;
}

#ifdef __cplusplus
}
#endif
