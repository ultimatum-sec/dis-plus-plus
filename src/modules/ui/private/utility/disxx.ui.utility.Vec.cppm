export module disxx.ui.utility.Vec;

export import <type_traits>;

export namespace disxx::ui::utility
{
	template <typename T> requires std::is_arithmetic<T>::value
	struct Vec2
	{
	  public:
		T x, y;
	
	  public:
		explicit Vec2(void) noexcept;
		explicit Vec2(T, T) noexcept;
		
		explicit Vec2(const Vec2 &) noexcept;
		Vec2 &operator=(const Vec2 &) noexcept;

		~Vec2(void) noexcept = default;
	};

	template <typename T> requires std::is_arithmetic<T>::value
	Vec2<T>::Vec2(void) noexcept
		: x{}
		, y{}	
	{}

	template <typename T> requires std::is_arithmetic<T>::value
	Vec2<T>::Vec2(T __x, T __y) noexcept
		: x{__x}
		, y{__y}	
	{}

	template <typename T> requires std::is_arithmetic<T>::value
	Vec2<T>::Vec2(const Vec2<T> &other) noexcept
		: x{other.x}
		, y{other.y}
	{}

	template <typename T> requires std::is_arithmetic<T>::value
	Vec2<T> &Vec2<T>::operator=(const Vec2<T> &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->x = other.x;
			this->y = other.y;
		}

		return *this;
	}

	template <typename T> requires std::is_arithmetic<T>::value
	struct Vec3
	{
	  public:
		T x, y, z;

	  public:
		explicit Vec3(void) noexcept;
		explicit Vec3(T, T, T) noexcept;

		explicit Vec3(const Vec3 &) noexcept;
		Vec3 &operator=(const Vec3 &) noexcept;

		~Vec3(void) noexcept = default;
	};

	template <typename T> requires std::is_arithmetic<T>::value
	Vec3<T>::Vec3(void) noexcept
		: x{}
		, y{}
		, z{}
	{}

	template <typename T> requires std::is_arithmetic<T>::value
	Vec3<T>::Vec3(T __x, T __y, T __z) noexcept
		: x{__x}
		, y{__y}
		, z{__z}
	{}

	template <typename T> requires std::is_arithmetic<T>::value
	Vec3<T>::Vec3(const Vec3<T> &other) noexcept
		: x{other.x}
		, y{other.y}
		, z{other.z}
	{}

	template <typename T> requires std::is_arithmetic<T>::value
	Vec3<T> &Vec3<T>::operator=(const Vec3<T> &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}

		return *this;
	}
} /* disxx::ui::utility */
