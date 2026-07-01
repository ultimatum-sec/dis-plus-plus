module;

#include <string_view>
#include <utility>
#include <string>

module disxx.ui.utility.Text;

namespace disxx::ui::utility
{
	Text::Text(void) noexcept
		: Renderable{Renderable::Type::TYPE_TEXT}
		, m_Text{}
	{}

	Text::Text(std::string_view str) noexcept
		: Renderable{Renderable::Type::TYPE_TEXT}
		, m_Text{str}
	{}

	Text::Text(const Text &other) noexcept
		: Renderable{other}
		, m_Text{other.m_Text}
	{}

	Text &Text::operator=(const Text &other) noexcept
	{
		if (this != &other) [[likely]]
		{
			Renderable::operator=(other);
			this->m_Text = other.m_Text;
		}

		return *this;
	}

	Text::Text(Text &&other) noexcept
		: Renderable{std::move(other)}
		, m_Text{std::move(other.m_Text)}
	{}

	Text &Text::operator=(Text &&other) noexcept
	{
		Renderable::operator=(std::move(other));
		this->m_Text = std::move(other.m_Text);

		return *this;
	}
} /* disxx::ui::utility */
