module;

#include <disconf.hpp>

export module disxx.utility.trait.Cloneable;

export import <memory>;

export namespace disxx::utility::trait
{
	template <typename T>
	class Cloneable
	{
	  public:
		virtual ~Cloneable(void) noexcept = default;

		virtual std::unique_ptr<T> Clone(void) const noexcept = 0;
	};
} /* disxx::utility::trait */
