#ifndef MAPPED_H
#define MAPPED_H

#ifdef __cplusplus
extern "C"
{
#endif

struct mapped_t
{
	void *mptr;
	unsigned long long int msize;
};

mapped_t mopen(const char *);
signed long long int mclose(mapped_t);

#ifdef __cplusplus
}
#endif

#endif /* mapped.h */
