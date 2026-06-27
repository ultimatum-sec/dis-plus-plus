module;

#include <disconf.hpp>

#define DANGLING(T) \
	[] -> T * \
	{ \
		const auto __ptr{new char}; \
		delete __ptr; \
		return reinterpret_cast<T *>(__ptr); \
	}()

export module disxx.utility.pointer.NonNull;

export import <cstdint>;

export import disxx.utility.pointer.DefaultDeleter;
export import disxx.utility.error.NullPointerError;

export namespace disxx::utility::pointer
{
	template <typename T, typename D>
	concept Deleter = requires(T var, D deleter)
	{ { deleter(var) }; };

	/*
	 * disxx::utility::pointer::NonNull
	 *
	 * Just a stupid ptr wrapper, that
	 * guarantees it won't be constructed
	 * if the pointer is null
	 */
	template <typename T, typename D = DefaultDeleter<T *>> requires Deleter<T *, D>
	class __DISXX_EXPORT__ [[nodiscard]] NonNull
	{
	  private:
		T *m_pPointer;
		D m_Deleter;

	  public:
		explicit NonNull(void) noexcept;
		explicit NonNull(T *) noexcept(false);
		explicit NonNull(std::nullptr_t) noexcept = delete;

		NonNull(const NonNull &) noexcept;
		NonNull &operator=(const NonNull &) noexcept;
		
		NonNull(NonNull &&) noexcept;
		NonNull &operator=(NonNull &&) noexcept;

		NonNull &operator=(T *) noexcept(false);
		NonNull &operator=(std::nullptr_t) noexcept = delete;

		operator bool(void) const noexcept;
	
		T *operator->(void) noexcept;
		const T *operator->(void) const noexcept;

		T &operator*(void) noexcept;
		const T &operator*(void) const noexcept;

		NonNull &operator++(void) noexcept;
		NonNull &operator--(void) noexcept;
		
		NonNull operator++(const int) noexcept;
		NonNull operator--(const int) noexcept;

		NonNull operator+(const std::size_t) const noexcept;
		NonNull operator-(const std::size_t) const noexcept;

		NonNull &operator+=(const std::size_t) noexcept;
		NonNull &operator-=(const std::size_t) noexcept;
		
		typename std::decay<T>::type &operator[](std::size_t) const noexcept
			requires std::is_array<T>::value;
	
		void Delete(void) noexcept;
	};

	// WARNING: m_pPointer will be dangling!
	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D>::NonNull(void) noexcept
		: m_pPointer{DANGLING(T)}
		, m_Deleter{}
	{}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D>::NonNull(T *ptr) noexcept(false)
		: m_pPointer{DANGLING(T)}
		, m_Deleter{}
	{
		if (!ptr) [[unlikely]]
			throw disxx::utility::error::NullPointerError{"NonNull"};
		this->m_pPointer = ptr;
	}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D>::NonNull(const NonNull &other) noexcept
		: m_pPointer{other.m_pPointer}
		, m_Deleter{other.m_Deleter}
	{}
	
	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> &NonNull<T, D>::operator=(const NonNull &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_pPointer = other.m_pPointer;
			this->m_Deleter = other.m_Deleter;
		}

		return *this;
	}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D>::NonNull(NonNull &&other) noexcept
		: m_pPointer{std::move(other.m_pPointer)}
		, m_Deleter{std::move(other.m_Deleter)}
	{}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> &NonNull<T, D>::operator=(NonNull &&other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->m_pPointer = std::move(other.m_pPointer);
			this->m_Deleter = std::move(other.m_Deleter);
		}

		return *this;
	}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> &NonNull<T, D>::operator=(T *ptr) noexcept(false)
	{
		if (!ptr) [[unlikely]]
			throw disxx::utility::error::NullPointerError{"NonNull"};
		else if (this->m_pPointer != ptr) [[likely]]
			this->m_pPointer = ptr;
		return *this;
	}

	// This method should always return true, I hope...
	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D>::operator bool(void) const noexcept
	{ return this->m_pPointer == nullptr; }

	template <typename T, typename D> requires Deleter<T *, D>
	T *NonNull<T, D>::operator->(void) noexcept
	{ return this->m_pPointer; }
	
	template <typename T, typename D> requires Deleter<T *, D>	
	const T *NonNull<T, D>::operator->(void) const noexcept
	{ return this->m_pPointer; }

	template <typename T, typename D> requires Deleter<T *, D>
	T &NonNull<T, D>::operator*(void) noexcept
	{ return *this->m_pPointer; }

	template <typename T, typename D> requires Deleter<T *, D>
	const T &NonNull<T, D>::operator*(void) const noexcept
	{ return *this->m_pPointer; }

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> &NonNull<T, D>::operator++(void) noexcept
	{
		++this->m_pPointer;
		return *this;
	}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> &NonNull<T, D>::operator--(void) noexcept
	{
		--this->m_pPointer;
		return *this;
	}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> NonNull<T, D>::operator++(int) noexcept
	{
		const auto old{*this};
		this->m_pPointer++;
		return old;
	}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> NonNull<T, D>::operator--(int) noexcept
	{
		const auto old{*this};
		this->m_pPointer--;
		return old;
	}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> NonNull<T, D>::operator+(const std::size_t value) const noexcept
	{
		auto copy{*this};
		copy.m_pPointer += value;

		return copy;
	}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> NonNull<T, D>::operator-(const std::size_t value) const noexcept
	{
		auto copy{*this};
		copy.m_pPointer -= value;

		return copy;
	}

	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> &NonNull<T, D>::operator+=(const std::size_t value) noexcept
	{
		this->m_pPointer += value;
		return *this;
	}
	
	template <typename T, typename D> requires Deleter<T *, D>
	NonNull<T, D> &NonNull<T, D>::operator-=(const std::size_t value) noexcept
	{
		this->m_pPointer -= value;
		return *this;
	}

	template <typename T, typename D> requires Deleter<T *, D>
	typename std::decay<T>::type &NonNull<T, D>::operator[](const std::size_t index) const noexcept
		requires std::is_array<T>::value
	{ return *(*this + index); }

	template <typename T, typename D> requires Deleter<T *, D>
	void NonNull<T, D>::Delete(void) noexcept
	{ this->m_Deleter(this->m_pPointer); }
} /* disxx::utility::pointer */
