export module disxx.loader.utility.MappedFile:mapfile_t;

export extern "C"
{
	struct mapfile_t
	{
		const void *mptr;
		unsigned long long int msize;
	};

	/*
     * WARNING: Only ONE file
     * at the same time can be
     * mapped/unmapped with
     * these functions!
     */

    extern mapfile_t *mopen(const char *);
    extern signed long long int mclose(void);
}
