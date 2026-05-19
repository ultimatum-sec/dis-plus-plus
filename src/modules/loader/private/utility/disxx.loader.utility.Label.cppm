module;

#include <cstdint>
#include <string>

export module disxx.loader.utility.Label;

export namespace disxx::loader::utility
{
	struct func_t
	{
		std::string name;
		std::uint64_t vaddr; // virtuall address
		std::uint64_t addr; // file offset
		std::uint64_t size;
	};
} /* disxx::loader::utility */
