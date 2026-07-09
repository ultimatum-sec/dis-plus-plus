module;

#include <functional>
#include <algorithm>
#include <cstdlib>
#include <ranges>
#include <format>
#include <vector>
#include <regex>

#define CLEARFRAME(frame) \
	(frame) \
		| std::views::all \
		| std::views::transform \
			( \
				[](char &ch) -> char \
				{ \
					if (ch != '-' && ch != '+') \
						return '-'; \
					return ch; \
				} \
			) \
		| std::ranges::to<std::string>()

module FailHandler;

import disxx.ui.Widget;
import disxx.ui.Button;
import disxx.ui.SourceEditor;

FailHandler *FailHandler::s_pInstance = nullptr;

FailHandler::FailHandler(void) noexcept(false)
	: m_Window{disxx::ui::utility::Vec2<int>{800, 600}, "dis++ crash reporter"}
	, m_Parser{}
{
	this->m_Window.SetVisible(true);

	disxx::ui::SourceEditor report
	{
		0.f,
		100.f,
		800,
		500
	};
	report.SetColor(0.2f, 0.2f, 0.2f);
	report.AddLine("{:-<64}", "");
    report.AddLine("Application dis++ was closed due to unexpected error!");
    report.AddLine("Please attach description of the crash and its reasons at:");
	report.AddLine("https://github.com/ultimatum-sec/dis-plus-plus/issues");
	report.AddLine("{:-<64}", "");
	report.AddLine("");

	// Load file with data of the crash
	this->m_Parser.Load
	(
		#ifdef _WIN32
			std::format("{}\\dis++\\crash.ini", std::getenv("LOCALAPPDATA"))
		#elif __APPLE__
			std::format("{}/Library/logs/dis++/crash.ini", std::getenv("HOME"))
		#else
			std::format("{}/.local/state/dis++/crash/crash.ini", std::getenv("HOME"))
		#endif
	);

	// Get general information about the crash
	const auto &time{this->m_Parser.Read<std::string>("crash.time").value_or("[unknown]")};
	const auto &path{this->m_Parser.Read<std::string>("crash.path").value_or("[unknown]")};
	const auto &pid{this->m_Parser.Read<std::string>("crash.pid").value_or("[unknown]")};
	const auto &exception{this->m_Parser.Read<std::string>("crash.exception").value_or("[unknown]")};
	const auto &reason{this->m_Parser.Read<std::string>("crash.reason").value_or("[unknown]")};

	std::vector<unsigned long int> sizes{};
	std::vector<std::string> strings{};

	strings.emplace_back
	(
		std::format
		(
			"| {} {}[{}]: Terminating",
			std::regex_replace(time, std::regex{R"(\,)"}, " "),
			path,
			pid
		)
	);
	sizes.emplace_back(strings.rbegin()->size());

	strings.emplace_back(std::format("| due to uncaught exception of type {} -> {}", exception, reason));
	sizes.emplace_back(strings.rbegin()->size());

	std::string frame{"+-Exception-reason-"};
	if (const auto max{std::ranges::max(sizes)}; max > frame.size())
		for (const auto _ : std::views::iota(0ul, max - frame.size() + 1))
			frame += "-";
	frame += "+";

	for (auto &str : strings)
	{
		if (const auto max{std::ranges::max(sizes)}; max > str.size())
			for (const auto _ : std::views::iota(0ul, max - str.size()))
				str += " ";
		str += " |";
	}

	report.AddLine("{}", frame);
	for (const auto &str : strings)
		report.AddLine("{}", str);
	report.AddLine("{}", CLEARFRAME(frame));

	sizes.clear();
	strings.clear();
	frame.clear();

	frame = "+-Registers-";
	
	// Get thread state
	const auto &registers{this->m_Parser.Read<std::string>("crash.registers").value_or("[unknown]")};
	std::string formatted{"| "};
	for (auto &ch : registers)
	{
		if (ch == ',' && std::ranges::count(formatted, ':') == 4)
		{
			formatted += " ";
			strings.push_back(std::regex_replace(formatted, std::regex{R"(\:)"}, "="));
			sizes.emplace_back(strings.rbegin()->size());
			formatted = "| ";
		}
		else
			formatted += ch;
	}

	if (const auto max{std::ranges::max(sizes)}; max > frame.size())
		for (const auto _ : std::views::iota(0ul, max - frame.size() + 1))
			frame += "-";
	frame += "+";

	for (auto &str : strings)
	{
		if (const auto max{std::ranges::max(sizes)}; max > str.size())
			for (const auto _ : std::views::iota(0ul, max - str.size()))
				str += " ";
		str += " |";
	}
	
	report.AddLine("{}", frame);
	for (const auto &str : strings)
		report.AddLine("{}", str);
	report.AddLine("{}", CLEARFRAME(frame));

	sizes.clear();
	strings.clear();
	frame.clear();

	frame = "+-Stack-trace-";

	// Get call stack
	const auto &stack{this->m_Parser.Read<std::string>("crash.stack").value_or("[unknown]")};
	formatted.clear();
	for (const auto ch : stack)
	{
		if (ch == ',' && *formatted.rbegin() == ']')
		{
			strings.emplace_back(std::format("| at {}", formatted));
			sizes.emplace_back(strings.rbegin()->size());
			formatted.clear();
		}
		else
			formatted += ch;
	}

	if (const auto max{std::ranges::max(sizes)}; max > frame.size())
		for (const auto _ : std::views::iota(0ul, max - frame.size()))
			frame += "-";
	frame += "+";

	for (auto &str : strings)
	{
		if (const auto max{std::ranges::max(sizes)}; max > str.size())
			for (const auto _ : std::views::iota(0ul, max - str.size()))
				str += " ";
		str += "|";
	}
	
	report.AddLine("{}", frame);
	for (const auto &str : strings)
		report.AddLine("{}", str);
	report.AddLine("{}", CLEARFRAME(frame));

	this->m_Window.AddWidget(std::make_unique<disxx::ui::SourceEditor>(report));
}

FailHandler *FailHandler::Init(int &argc, const char *argv[]) noexcept(false)
{
	disxx::ui::MainWindow::Init(&argc, const_cast<char **>(argv));
    if (!s_pInstance) [[likely]]
        s_pInstance = new FailHandler{};
	return s_pInstance;
}


[[nodiscard]] int FailHandler::Exec(void) const noexcept(false)
{
	try
	{ this->m_Window.Exec(); }
	catch (...)
	{ return EXIT_FAILURE; }

	return EXIT_SUCCESS;
}
