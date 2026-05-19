module;

#include <charconv>
#include <ranges>
#include <regex>

#include <print>

module disxx.ui.utility.ColorTag;

namespace disxx::ui::utility
{
	ColorTag::ColorTag(void) noexcept = default;
	ColorTag::ColorTag(const ColorTag &) noexcept = default;
	ColorTag &ColorTag::operator=(const ColorTag &) noexcept = default;
	ColorTag::~ColorTag(void) noexcept = default;

	std::string ColorTag::RemoveTags(const std::string &str) const noexcept
	{
		return std::string
		{
			std::regex_replace
			(
				std::regex_replace
				(
					str,
					std::regex{R"(<color\s+value=\"([0-1]\.\d)\s+([0-1]\.\d)\s+([0-1]\.\d)\s+([0-1]\.\d)\">)"},
					std::string{}
				),
				std::regex{R"(</color>)"},
				std::string{}
			)
		};
	}

	ColorTagResult ColorTag::Parse(const std::string &str) const noexcept
	{
		std::vector<std::pair<std::string, std::array<float, 4>>> vec{};
    
		const std::regex pattern{R"(<color\s+value=\"([0-1]\.\d)\s+([0-1]\.\d)\s+([0-1]\.\d)\s+([0-1]\.\d)\">([\s\S_]*?)</color>)"};

		// Default all the untagged colors to white
    	std::string tagged{};
    	for (std::sregex_token_iterator it{str.begin(), str.end(), pattern, {-1, 0}}, end{}; const auto& part : std::vector<std::string>{it, end})
		{
    	    if (part.empty())
				continue;
    	    else if (std::regex_match(part, pattern))
        	    tagged += part;
        	else
        	    tagged += "<color value=\"1.0 1.0 1.0 1.0\">" + part + "</color>";
    	}

		for (std::sregex_iterator it{tagged.begin(), tagged.end(), pattern}, end{}; it != end; ++it)
		{
			const auto &match{*it};
	
			std::array<float, 4> color{};
			for (const auto &i : std::views::iota(0, 4))
			{
				const auto diget{match[i + 1].str()};
				if (float n{}; std::from_chars(diget.data(), diget.data() + diget.size(), n))
					color.at(i) = n;
				else
					return std::unexpected{std::runtime_error{"NumberConvertionError"}};
			}

			vec.emplace_back
			(
				std::make_pair
				(
					match[5].str(),
					color	
				)
			);
		}

		return vec;
	}
} /* disxx::ui::utility */
