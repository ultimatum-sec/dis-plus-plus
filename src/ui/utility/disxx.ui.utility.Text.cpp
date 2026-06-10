module;

#include <string_view>
#include <utility>
#include <string>

module disxx.ui.utility.Text;

namespace disxx::ui::utility
{
	Text::Text(void) noexcept
		: Renderable{}
		, m_Text{}
	{}

	Text::Text(std::string_view str) noexcept
		: Renderable{}
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

	void Text::SetText(std::string_view str) noexcept
	{ this->m_Text = str; }

	std::string_view Text::GetText(void) const noexcept
	{ return this->m_Text; }
} /* disxx::ui::utility */
