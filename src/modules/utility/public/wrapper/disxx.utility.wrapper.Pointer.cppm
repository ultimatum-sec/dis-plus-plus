module;

#include <disconf.hpp>

#include <type_traits>
#include <stdexcept>
#include <format>

export module disxx.utility.wrapper.Pointer;

export import <concepts>;
export import <cstdint>;

import disxx.utility.wrapper.AbstractWrapper;
import disxx.utility.error.NullPointerError;

template <typename T>
concept SmartPointer = requires (T &ptr)
{
	typename T::element_type;
	{ *ptr } -> std::same_as<typename std::add_lvalue_reference<typename T::element_type>::type>;
	{ ptr.get() } -> std::same_as<typename std::add_pointer<typename T::element_type>::type>;
	{ ptr == nullptr } -> std::same_as<bool>;
};

export namespace disxx::utility::wrapper
{
	/*
	 * The stupid wrapper for smart pointers
	 * that should throw an error
	 * if the address in the pointer equals
	 * to nullptr
	 */
	template <SmartPointer T>
	class __DISXX_EXPORT__ [[nodiscard]] Pointer : public AbstractWrapper<T>
	{
	  public:
		template <typename ...Args> requires std::is_constructible<T, Args...>::value
		explicit Pointer(Args &&...args) noexcept(std::is_nothrow_constructible<T, Args...>::value);
		
		explicit Pointer(const Pointer &) noexcept(std::is_nothrow_copy_constructible<T>::value)
			requires std::is_copy_constructible<T>::value = default;
		explicit Pointer(const Pointer &) noexcept(std::is_nothrow_copy_constructible<T>::value)
			requires (!std::is_copy_constructible<T>::value) = delete;

		Pointer &operator=(const Pointer &) noexcept(std::is_nothrow_copy_assignable<T>::value)
			requires std::is_copy_assignable<T>::value = default;
		Pointer &operator=(const Pointer &) noexcept(std::is_nothrow_copy_assignable<T>::value)
			requires (!std::is_copy_assignable<T>::value) = delete;
		
		explicit Pointer(Pointer &&) noexcept(std::is_nothrow_move_constructible<T>::value)
			requires std::is_move_constructible<T>::value = default;
		explicit Pointer(Pointer &&) noexcept(std::is_nothrow_move_constructible<T>::value)
			requires (!std::is_move_constructible<T>::value) = delete;

		Pointer &operator=(Pointer &&) noexcept(std::is_nothrow_move_assignable<T>::value)
			requires std::is_move_assignable<T>::value = default;
		Pointer &operator=(Pointer &&) noexcept(std::is_nothrow_move_assignable<T>::value)
			requires (!std::is_move_assignable<T>::value) = delete;

		virtual ~Pointer(void) noexcept override = default;

		T::element_type *operator->(void) noexcept(false);
		const T::element_type *operator->(void) const noexcept(false);

		T::element_type &operator*(void) noexcept(false);
		const T::element_type &operator*(void) const noexcept(false);

		operator bool(void) const noexcept;

		// This wrapper intended for single objects only
		Pointer &operator++(void) noexcept = delete;
		Pointer &operator--(void) noexcept = delete;
		Pointer &operator++(int) noexcept = delete;
		Pointer &operator--(int) noexcept = delete;
		Pointer &operator+=(std::size_t) noexcept = delete;
		Pointer &operator-=(std::size_t) noexcept = delete;
		T::element_type &operator[](std::size_t) noexcept = delete;
	};

	template <SmartPointer T>
	template <typename ...Args> requires std::is_constructible<T, Args...>::value
	Pointer<T>::Pointer(Args &&...args) noexcept(std::is_nothrow_constructible<T, Args...>::value)
		: AbstractWrapper<T>{std::forward<Args...>(args...)}
	{}

	template <SmartPointer T>
	T::element_type *Pointer<T>::operator->(void) noexcept(false)
	{
		if (this->m_Object == nullptr) [[unlikely]]
			throw error::NullPointerError{"Attempt to get violated access to nullptr"};
		return this->m_Object.get();
	}

	template <SmartPointer T>
	const T::element_type *Pointer<T>::operator->(void) const noexcept(false)
	{
		if (this->m_Object == nullptr) [[unlikely]]
			throw error::NullPointerError{"Attempt to get violated access to nullptr"};
		return this->m_Object.get();
	}

	template <SmartPointer T>
	T::element_type &Pointer<T>::operator*(void) noexcept(false)
	{
		if (this->m_Object == nullptr) [[unlikely]]
			throw error::NullPointerError{"Attempt to get violated access to nullptr"};
		return *this->m_Object;
	}

	template <SmartPointer T>
	const T::element_type &Pointer<T>::operator*(void) const noexcept(false)
	{
		if (this->m_Object == nullptr) [[unlikely]]
			throw error::NullPointerError{"Attempt to get violated access to nullptr"};
		return *this->m_Object;
	}

	template <SmartPointer T>
	Pointer<T>::operator bool(void) const noexcept
	{ return this->m_Object != nullptr; }
} /* disxx::util::wrapper */
