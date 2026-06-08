module;

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include <algorithm>
#include <cstdlib>
#include <ranges>
#include <format>
#include <vector>
#include <regex>
#include <span>

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

FailHandler::FailHandler(std::span<const char *> args) noexcept(false)
	: MainWindow{800, 600}
	, m_Args{args}
	, m_Parser{}
{
	// Init some glut stuff
	static auto argc{static_cast<int>(args.size() & std::numeric_limits<unsigned int>::max())};
	glutInit(&argc, const_cast<char **>(args.data()));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(this->m_Width, this->m_Height);
	this->m_Win = glutCreateWindow("dis++ crash reporter");

	this->m_Widgets.emplace_back
	(
		std::make_unique<disxx::ui::SourceEditor>
		(
			0.f,
			100.f,
			this->m_Width,
			this->m_Height - 100
		)
	);
	
	this->m_Widgets.emplace_back
	(
		std::make_unique<disxx::ui::Button>
		(
			50.f,
			10.f,
			this->m_Width / 6.f,
			this->m_Height / 7.f
		)
	);

	glutKeyboardFunc
    (
        [](unsigned char key, int x, int y) -> void
        { s_pInstance->__KeyboardFunc(key, x, y); }
    );

    glutMouseFunc
    (
		[](int button, int state, int x, int y) -> void
		{ s_pInstance->__MouseFunc(button, state, x, y); }
    );

    glutReshapeFunc
    (
        [](int width, int height) -> void
        { s_pInstance->__ReshapeFunc(width, height); }
    );

    glutMotionFunc
    (
        [](int x, int y) -> void
        { s_pInstance->__MotionFunc(x, y); }
    );

    glutDisplayFunc
    (
        [](void) -> void
        { s_pInstance->__DisplayFunc(); }
    );

	static_cast<disxx::ui::Button *>(this->m_Widgets.rbegin()->get())->SetText("Close");
	(*this->m_Widgets.rbegin())->SetColor(0.5f, 0.5f, 0.5f);
	
	const auto &ptr{reinterpret_cast<disxx::ui::SourceEditor *>(this->m_Widgets.at(0).get())};
	ptr->SetColor(0.2f, 0.2f, 0.2f);

	ptr->AddLine("{:-<64}", "");
    ptr->AddLine("Application dis++ was closed due to unexpected error!");
    ptr->AddLine("Please attach description of the crash and its reasons at:");
	ptr->AddLine("https://github.com/ultimatum-sec/dis-plus-plus/issues");
	ptr->AddLine("{:-<64}", "");

	ptr->AddLine("");

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

	ptr->AddLine("{}", frame);
	for (const auto &str : strings)
		ptr->AddLine("{}", str);
	ptr->AddLine("{}", CLEARFRAME(frame));

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
	
	ptr->AddLine("{}", frame);
	for (const auto &str : strings)
		ptr->AddLine("{}", str);
	ptr->AddLine("{}", CLEARFRAME(frame));

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
	
	ptr->AddLine("{}", frame);
	for (const auto &str : strings)
		ptr->AddLine("{}", str);
	ptr->AddLine("{}", CLEARFRAME(frame));
}

FailHandler *FailHandler::Init(int &argc, const char *argv[]) noexcept(false)
{
    if (!s_pInstance) [[likely]]
        s_pInstance = new FailHandler{std::span<const char *>(argv, argc)};
	return s_pInstance;
}

void FailHandler::__KeyboardFunc(unsigned char key, int x, int y) noexcept(false)
{
	for (auto &pWidget : this->m_Widgets)
		pWidget->HandleKeyboard(key, x, y);
}

void FailHandler::__MouseFunc(int button, int state, int x, int y) noexcept(false)
{
	for (auto &pWidget : this->m_Widgets)
		pWidget->HandleMouse(button, state, x, y);
	glutPostRedisplay();
}

void FailHandler::__ReshapeFunc(int width, int height) noexcept(false)
{
	this->m_Width = width;
	this->m_Height = height;

    this->m_Widgets.at(0)->Resize(this->m_Width, this->m_Height - 100);
	this->m_Widgets.at(0)->Replace(0.f, 100.f);

	this->m_Widgets.at(1)->Resize(this->m_Width / 6, this->m_Height / 7);
   	this->m_Widgets.at(1)->Replace(50.f, 10.f);

    glutPostRedisplay();
}

void FailHandler::__MotionFunc(int x, int y) noexcept(false)
{
	for (auto &pWidget : this->m_Widgets)
		pWidget->HandleMotion(x, y);
	glutPostRedisplay();
}

void FailHandler::__DisplayFunc(void) noexcept
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	for (const auto &pWidget : this->m_Widgets)
		pWidget->Render();
	glutSwapBuffers();
	disxx::ui::Widget::ClearBuffer();
}

[[nodiscard]] int FailHandler::Exec(void) const noexcept(false)
{
	try
	{ glutMainLoop(); }
	catch (...)
	{ return EXIT_FAILURE; }

	return EXIT_SUCCESS;
}
