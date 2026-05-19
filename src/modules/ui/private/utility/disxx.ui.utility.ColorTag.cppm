export module disxx.ui.utility.ColorTag;

export import <string_view>;
export import <expected>;
export import <utility>;
export import <format>;
export import <vector>;
export import <string>;
export import <array>;

export namespace disxx::ui::utility
{
	using ColorTagResult = std::expected
	<
		std::vector
		<
			std::pair
			<
				std::string,
				std::array
				<
					float,
					4
				>
			>
		>,
		std::runtime_error
	>;

	class ColorTag
	{
	  public:
		explicit ColorTag(void) noexcept;

		explicit ColorTag(const ColorTag &) noexcept;
		ColorTag &operator=(const ColorTag &) noexcept;

		~ColorTag(void) noexcept;

		std::string RemoveTags(const std::string &) const noexcept;
		ColorTagResult Parse(const std::string &) const noexcept;
	};
} /* disxx::ui::utility */
