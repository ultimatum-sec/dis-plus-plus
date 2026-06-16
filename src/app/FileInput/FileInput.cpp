module;

#ifndef _WIN32
#	include <sys/syslimits.h> // For PATH_MAX
#else
#	include <windows.h>
#	define PATH_MAX MAX_PATH
#endif

#include <functional>
#include <filesystem>
#include <cstdlib>
#include <memory>
#include <vector>
#include <array>
#include <span>

module FileInput;

import DisLog;

// Good luck use it without Init method!
FileInput *FileInput::s_pInstance{nullptr};

// WARNING: this constructor shouldn't be used
FileInput::FileInput(void) noexcept
	: MainWindow{400, 300}
	, m_Args{std::span<const char *>{(const char*[]){"unknown"}, 1}}
	, m_Path{""}
{
	this->m_Win = this->CreateWindow
	(
		disxx::ui::utility::Vec2<int>
		{
			this->m_Width,
			this->m_Height
		},
		"Select an executable to disassemble"
	);
	this->SwitchWindow(this->m_Win);

	this->m_Widgets.emplace_back(std::make_unique<disxx::ui::TextInput>(50, 50, 300, 40));
	(*this->m_Widgets.rbegin())->SetColor(0.3f, 0.3f, 0.3f);
	this->m_Widgets.emplace_back(std::make_unique<disxx::ui::Button>(150, 100, 100, 40));
	(*this->m_Widgets.rbegin())->SetColor(0.3f, 0.3f, 0.3f);
	static_cast<disxx::ui::Button *>(this->m_Widgets.rbegin()->get())->SetText("OK");

	/* Set up callbacks */
	s_pInstance->SetDisplayCallback
    (
        [](void) -> void
        { s_pInstance->__DisplayFunc(); }
    );

	s_pInstance->SetReshapeCallback
    (
        [](int width, int height) -> void
        { s_pInstance->__ReshapeFunc(width, height); }
    );

	s_pInstance->SetKeyboardCallback
    (
        [](unsigned char key, int x, int y) -> void
        { s_pInstance->__KeyboardFunc(key, x, y); }
    );

    s_pInstance->SetMouseButtonCallback
    (
		[](int button, int state, int x, int y) -> void
		{ s_pInstance->__MouseFunc(button, state, x, y); }
    );

    s_pInstance->SetMouseMotionCallback
    (
        [](int x, int y) -> void
        { s_pInstance->__MotionFunc(x, y); }
    );
}

FileInput::FileInput(std::span<const char *> args) noexcept(false)
	: MainWindow{400, 300}
	, m_Args{args}
	, m_Path{""}
{
	// Check if args not null
	if (!this->m_Args.data()) [[unlikely]]
		throw std::invalid_argument{"ArgumentsValueError"};
	
	this->m_Win = this->CreateWindow
	(
		disxx::ui::utility::Vec2<int>
		{
			this->m_Width,
			this->m_Height
		},
		"Select an executable to disassemble"
	);
	this->SwitchWindow(this->m_Win);

	this->m_Widgets.emplace_back(std::make_unique<disxx::ui::TextInput>(50, 50, 300, 40));
	(*this->m_Widgets.rbegin())->SetColor(0.3f, 0.3f, 0.3f);
	this->m_Widgets.emplace_back(std::make_unique<disxx::ui::Button>(150, 100, 100, 40));
	(*this->m_Widgets.rbegin())->SetColor(0.3f, 0.3f, 0.3f);
	static_cast<disxx::ui::Button *>(this->m_Widgets.rbegin()->get())->SetText("OK");

	/* Set up callbacks */
	s_pInstance->SetDisplayCallback
    (
        [](void) -> void
        { s_pInstance->__DisplayFunc(); }
    );

	s_pInstance->SetReshapeCallback
    (
        [](int width, int height) -> void
        { s_pInstance->__ReshapeFunc(width, height); }
    );

	s_pInstance->SetKeyboardCallback
    (
        [](unsigned char key, int x, int y) -> void
        { s_pInstance->__KeyboardFunc(key, x, y); }
    );

    s_pInstance->SetMouseButtonCallback
    (
		[](int button, int state, int x, int y) -> void
		{ s_pInstance->__MouseFunc(button, state, x, y); }
    );

    s_pInstance->SetMouseMotionCallback
    (
        [](int x, int y) -> void
        { s_pInstance->__MotionFunc(x, y); }
    );
}

FileInput *FileInput::Init(std::span<const char *> args) noexcept(false)
{
	if (!s_pInstance) [[likely]]
		s_pInstance = new FileInput{args};
	return s_pInstance;
}

FileInput::~FileInput(void) noexcept
{ this->DestroyWindow(); }

void FileInput::SetPath(const std::filesystem::path &path) noexcept
{ static_cast<disxx::ui::TextInput *>(this->m_Widgets.at(0).get())->SetText(path.string()); }

const std::filesystem::path &FileInput::GetPath(void) const noexcept
{ return this->m_Path; }

void FileInput::SetCallback(std::function<void(const disxx::ui::MainWindow *const)> func) noexcept
{
	static_cast<disxx::ui::Button *>(this->m_Widgets.at(1).get())->SetCallback
	(
		disxx::ui::Button::ButtonTrigger::BTN_CLICKED,
		std::bind
		(
			[](FileInput *const obj, std::function<void(const disxx::ui::MainWindow *const)> func) mutable -> void
			{
				obj->m_Path = static_cast<disxx::ui::TextInput *>(obj->m_Widgets.at(0).get())->GetText();
				func(obj);
			},
			static_cast<FileInput *>(this),
			func
		)
	);
}

void FileInput::__KeyboardFunc(unsigned char key, int x, int y)
{
	// BUFSIZ chars - max size
	if (auto pTxt{static_cast<disxx::ui::TextInput *>(this->m_Widgets.at(0).get())}; pTxt->GetText().size() < PATH_MAX || key == 0x7F) [[likely]]
		pTxt->HandleKeyboard(key, x, y);
}

void FileInput::__MouseFunc(int button, int state, int x, int y) noexcept(false)
{
	for (auto &pWidget : this->m_Widgets)
		pWidget->HandleMouse(button, state, x, y);
}

// Do nothing
void FileInput::__ReshapeFunc(int, int) noexcept(false)
{ return; }

// Do nothing
void FileInput::__MotionFunc(int, int) noexcept(false)
{ return; }

void FileInput::__DisplayFunc(void) noexcept
{
	for (const auto &pWidget : this->m_Widgets)
		pWidget->Render();
	this->SwapBuffers();
	disxx::ui::Widget::ClearBuffers();
}
