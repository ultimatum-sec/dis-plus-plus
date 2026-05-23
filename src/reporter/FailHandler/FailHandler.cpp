module;

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include <algorithm>
#include <ranges>
#include <format>
#include <vector>
#include <regex>
#include <span>

#include <print>

module FailHandler;

import disxx.ui.Widget;
import disxx.ui.SourceEditor;

FailHandler *FailHandler::s_pInstance = nullptr;

FailHandler::FailHandler(std::span<const char *> args) noexcept(false)
	: MainWindow{800, 600}
	, m_Args{args}
	, m_Parser{}
{
	// Check if args not null
	if (!this->m_Args.data()) [[unlikely]]
		throw std::invalid_argument{"ArgumentsValueError"};
	
	this->m_Widgets.emplace_back
	(
		std::make_unique<disxx::ui::SourceEditor>
		(
			0.f,
			0.f,
			this->m_Width,
			this->m_Height
		)
	);

	// Init some glut stuff
	static auto argc{static_cast<int>(args.size() & std::numeric_limits<unsigned int>::max())};
	glutInit(&argc, const_cast<char **>(args.data()));
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(this->m_Width, this->m_Height);
	this->m_Win = glutCreateWindow("dis++ crash reporter");

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

	const auto &ptr{reinterpret_cast<disxx::ui::SourceEditor *>(this->m_Widgets.at(0).get())};
	ptr->SetColor(0.2f, 0.2f, 0.2f);

	ptr->AddLine("{:-<64}", "");
    ptr->AddLine("Application dis++ was closed due to unexpected error!");
    ptr->AddLine("Please attach description of the crash and its reasons at:");
	ptr->AddLine("https://github.com/ultimatum-sec/dis-plus-plus/issues");
	ptr->AddLine("{:-<64}", "");

	ptr->AddLine("");

	// Start the report
	ptr->AddLine("{:-<32}BEGIN-REPORT{:-<32}", "", "");

	// Load file with data of the crash
	this->m_Parser.LoadFile
	(
		std::filesystem::path{args.at(0)}
			.parent_path()
			.string()
			+ std::filesystem::path::preferred_separator
			+ std::string{".."}
			+ std::filesystem::path::preferred_separator
			+ "crash.ini"
	);

	ptr->AddLine("-*- General information -*-");
	ptr->AddLine("==={:-<64}===", "");

	// Get general information about the crash
	const auto &time{this->m_Parser.Get<std::string>("crash.time").value_or("[unknown]")};
	const auto &path{this->m_Parser.Get<std::string>("crash.path").value_or("[unknown]")};
	const auto &pid{this->m_Parser.Get<std::string>("crash.pid").value_or("[unknown]")};
	const auto &type{this->m_Parser.Get<std::string>("crash.type").value_or("[unknown]")};
	ptr->AddLine("{} {}[{}]: Terminating due to uncaught exception", time, path, pid);
	ptr->AddLine("of type {}", type);

	ptr->AddLine("==={:-<64}===", "");

	ptr->AddLine("-*- Thread state -*-");
	ptr->AddLine("==={:-<64}===", "");

	// Get thread state
	std::string registers{};
	if (const auto &var{this->m_Parser.Get<std::string>("crash.registers")})
		registers = var.value();
	else
		registers = var.error().what();

	//const auto &registers{this->m_Parser.Get<std::string>("crash.registers").value_or("[unknown]")};
	
	auto index{0ul}, end{static_cast<unsigned long int>(std::ranges::count(registers, ',') + 1ul)};
	std::vector<std::string> formatted{};
	std::regex regex{R"((\S+)(?=(\,|$)))"};
	for (std::sregex_iterator it{registers.begin(), registers.end(), regex}, var{}; it != var; ++it)
	{
		const auto &match{(*it)[0].str()};
		
		const auto &reg
		{
			match
				| std::views::all
				| std::views::take_while([](auto ch) -> bool { return ch != ':'; })
				| std::ranges::to<std::string>()
		};
		
		const auto &value
		{
			match
				| std::views::all
				| std::views::reverse
				| std::views::take_while([](auto ch) -> bool { return ch != ':'; })
				| std::ranges::to<std::string>()
		};

		formatted.emplace_back(std::format("{}: {}", reg, value));

		index++;

		if (index % 4 == 0 || index == end)
		{
			const auto count
			{
				index % 4 == 0
					? 4
					: index % 4
			};

			ptr->AddLine("{} ", formatted.at(index - count));
			if (count > 1)
				for (const auto i : std::views::iota(index - count + 1, index))
					ptr->AddString("{} ", formatted.at(i));
		}
	}

	ptr->AddLine("==={:-<64}===", "");

	// Get call stack
	ptr->AddLine("-*- Call stack -*-");
	ptr->AddLine("==={:-<64}===", "");

	const auto &stack{this->m_Parser.Get<std::string>("crash.stack").value_or("[unknown]")};
	regex = std::regex{R"((\S+)(?=(\,|$)))"};
	for (std::sregex_iterator it{stack.begin(), stack.end(), regex}, var{}; it != var; ++it)
	{
		const auto &match{(*it)[0].str()};
		ptr->AddLine("{}", match);
	}

	ptr->AddLine("==={:-<64}===", "");
	
	// End the report
	ptr->AddLine("{:-<32}END-REPORT{:-<32}", "", "");
}

FailHandler *FailHandler::Init(int &argc, const char *argv[]) noexcept(false)
{
    if (!s_pInstance) [[likely]]
        s_pInstance = new FailHandler{std::span<const char *>(argv, argc)};
	glutPostRedisplay();
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

	glViewport(0, 0, this->m_Width, this->m_Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, this->m_Width, 0, this->m_Height);
    glMatrixMode(GL_MODELVIEW);

    this->m_Widgets.at(0)->Resize(this->m_Width, this->m_Height);
	this->m_Widgets.at(0)->Replace(0.f, 0.f);
    
    glutPostRedisplay();
}

void FailHandler::__MotionFunc(int x, int y) noexcept(false)
{
	for (auto &pWidget : this->m_Widgets)
		pWidget->HandleMotion(x, y);
}

void FailHandler::__DisplayFunc(void) noexcept
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, this->m_Width, 0, this->m_Height);

	for (const auto &pWidget : this->m_Widgets)
		pWidget->Render();
	glutSwapBuffers();
}

[[nodiscard]] int FailHandler::Exec(void) const noexcept(false)
{
	try
	{ glutMainLoop(); }
	catch (...)
	{ return EXIT_FAILURE; }

	return EXIT_SUCCESS;
}
