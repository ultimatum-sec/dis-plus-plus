module;

#include <disconf.hpp>

#include <utility>
#include <memory>

export module disxx.utility.wrapper.AbstractWrapper;

export import <type_traits>;

export namespace disxx::utility::wrapper
{
	/*
	 * An owning Base class for wrappers
	 * under STL containers/smart pointers
	 */
	template <typename T>
	class __DISXX_EXPORT__ [[nodiscard]] AbstractWrapper
	{
	  protected:
		T m_Object;

	  public:
		using ObjectType = T;

	  public:
		template <typename ...Args> requires std::is_constructible<T, Args...>::value
		explicit AbstractWrapper(Args &&...) noexcept(std::is_nothrow_constructible<T, Args...>::value);
		
		explicit AbstractWrapper(const AbstractWrapper &) noexcept(std::is_nothrow_copy_constructible<T>::value)
			requires std::is_copy_constructible<T>::type  = default;
		explicit AbstractWrapper(const AbstractWrapper &) noexcept(std::is_nothrow_copy_constructible<T>::value)
			requires (!std::is_copy_constructible<T>::type) = delete;
	
		AbstractWrapper &operator=(const AbstractWrapper &) noexcept(std::is_nothrow_copy_assignable<T>::value)
			requires std::is_copy_assignable<T>::type = default;
		AbstractWrapper &operator=(const AbstractWrapper &) noexcept(std::is_nothrow_copy_assignable<T>::value)
			requires (!std::is_copy_assignable<T>::type) = delete;

		explicit AbstractWrapper(AbstractWrapper &&) noexcept(std::is_nothrow_move_constructible<T>::value)
			requires std::is_move_constructible<T>::type = default;
		explicit AbstractWrapper(AbstractWrapper &&) noexcept(std::is_nothrow_move_constructible<T>::value)
			requires (!std::is_move_constructible<T>::type) = delete;

		AbstractWrapper &operator=(AbstractWrapper &&) noexcept(std::is_nothrow_move_assignable<T>::value)
			requires std::is_move_assignable<T>::type = default;
		AbstractWrapper &operator=(AbstractWrapper &&) noexcept(std::is_nothrow_move_assignable<T>::value)
			requires (!std::is_move_assignable<T>::type) = delete;
	
		virtual ~AbstractWrapper(void) noexcept(std::is_nothrow_destructible<T>::value) = default;

		// Both of this methods return a
		// reference for an original object	
		T &Get(void) noexcept;
		const T &Get(void) const noexcept;

		// Sets the m_Object and moves *this
		AbstractWrapper &&Object(T &&) noexcept(std::is_nothrow_move_constructible<T>::value);
	};

	template <typename T>	
	template <typename ...Args> requires std::is_constructible<T, Args...>::value
	AbstractWrapper<T>::AbstractWrapper(Args &&...args) noexcept(std::is_nothrow_constructible<T, Args...>::value)
		: m_Object{std::forward<Args...>(args...)}
	{}
	
	template <typename T>
	T &AbstractWrapper<T>::Get(void) noexcept
	{ return this->m_Object; }

	template <typename T>
	const T &AbstractWrapper<T>::Get(void) const noexcept
	{ return this->m_Object; }

	template <typename T>
	AbstractWrapper<T> &&AbstractWrapper<T>::Object(T &&obj) noexcept(std::is_nothrow_move_constructible<T>::value)
	{
		this->m_Object = std::move(obj);
		return std::move(*this);
	}
} /* disxx::util::wrapper */





