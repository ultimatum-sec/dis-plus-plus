export module disxx.ui.utility.Vec;

export import <type_traits>;

export namespace disxx::ui::utility
{
	template <typename T> requires std::is_arithmetic<T>::value
	struct Vec2
	{
		T x;
		T y;
	};

	template <typename T> requires std::is_arithmetic<T>::value
	struct Vec3
	{
		T x;
		T y;
		T z;
	};
} /* disxx::ui::utility */
