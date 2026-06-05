module;

export module disxx.ui.utility.Vertex;

import disxx.ui.utility.Vec;

export namespace disxx::ui::utility
{
	// I guess this structure will be loaded into GPU memory
	template <typename T> requires std::is_arithmetic<T>::value
	struct Vertex
	{
		Vec2<T> position;
		Vec3<T> color;
	};
} /* disxx::ui::utility */
