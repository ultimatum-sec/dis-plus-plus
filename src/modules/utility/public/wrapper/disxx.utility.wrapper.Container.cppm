module;

#include <disconf.hpp>

#include <concepts>
#include <utility>
#include <format>

export module disxx.utility.wrapper.Container;

export import <type_traits>;
export import <functional>;
export import <optional>;

import disxx.utility.wrapper.AbstractWrapper;

template <typename T>
concept ContainerType = requires(T arr)
{
	{ arr.size() } -> std::same_as<typename T::size_type>;
	{ arr[0] } -> std::same_as<typename std::add_lvalue_reference<typename T::value_type>::type>; 
};

export namespace disxx::utility::wrapper
{
	// Stupid wrapper for STL containers returning std::optional
	// for attempts to get the value by index
	// In fact, ContainerWrapper<T>::At works the
	// same way as the "get" method in Rust
	template <ContainerType T>
	class __DISXX_EXPORT__ [[nodiscard]] Container : public AbstractWrapper<T>
	{
	  public:
		using ValueType = typename T::value_type;
		using SizeType = typename T::size_type;

	  public:
		template <typename ...Args> requires std::is_constructible<T, Args...>::value
		explicit Container(Args &&...args) noexcept(std::is_nothrow_constructible<T, Args...>::value);
		
		explicit Container(const Container &) noexcept(std::is_nothrow_copy_constructible<T>::value)
			requires std::is_copy_constructible<T>::value = default;
		explicit Container(const Container &) noexcept(std::is_nothrow_copy_constructible<T>::value)
			requires (!std::is_copy_constructible<T>::value) = delete;

		Container &operator=(const Container &) noexcept(std::is_nothrow_copy_assignable<T>::value)
			requires std::is_copy_assignable<T>::value = default;
		Container &operator=(const Container &) noexcept(std::is_nothrow_copy_assignable<T>::value)
			requires (!std::is_copy_assignable<T>::value) = delete;
		
		explicit Container(Container &&) noexcept(std::is_nothrow_move_constructible<T>::value)
			requires std::is_move_constructible<T>::value = default;
		explicit Container(Container &&) noexcept(std::is_nothrow_move_constructible<T>::value)
			requires (!std::is_move_constructible<T>::value) = delete;

		Container &operator=(Container &&) noexcept(std::is_nothrow_move_assignable<T>::value)
			requires std::is_move_assignable<T>::value = default;
		Container &operator=(Container &&) noexcept(std::is_nothrow_move_assignable<T>::value)
			requires (!std::is_move_assignable<T>::value) = delete;

		virtual ~Container(void) noexcept = default;
		
		std::optional<std::reference_wrapper<typename Container<T>::ValueType>> At(const SizeType) noexcept;
		std::optional<std::reference_wrapper<const typename Container<T>::ValueType>> At(const SizeType) const noexcept;
	};

	template <ContainerType T>
	template <typename ...Args> requires std::is_constructible<T, Args...>::value
	Container<T>::Container(Args &&...args) noexcept(std::is_nothrow_constructible<T, Args...>::value)
		: AbstractWrapper<T>{std::forward<Args...>(args...)}
	{}
	
	template<ContainerType T>
	std::optional<std::reference_wrapper<typename Container<T>::ValueType>> Container<T>::At(const SizeType index) noexcept
	{
		if (index < this->m_Object.size()) [[likely]]
			return std::ref(this->m_Object[index]);
		return std::nullopt;
	}

	template<ContainerType T>
	std::optional<std::reference_wrapper<const typename Container<T>::ValueType>> Container<T>::At(const SizeType index) const noexcept
	{
		if (index < this->m_Object.size()) [[likely]]
			return std::cref(this->m_Object[index]);
		return std::nullopt;
	}
} /* disxx::util */
