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

FileInput *FileInput::s_pInstance{nullptr};

// WARNING: this constructor shouldn't be used
FileInput::FileInput(void) noexcept
	: m_pWindow{disxx::ui::MainWindow::Init(disxx::ui::utility::Vec2<int>{400, 300}, "Select an executable to disassemble")}
	, m_Path{""}
{
	this->m_pWindow->SetVisible(true);

	{
		disxx::ui::TextInput txt{125, 150, 300, 40};
		txt.SetColor(0.3f, 0.3f, 0.3f);
		this->m_pWindow->AddWidget(std::make_unique<disxx::ui::TextInput>(txt));
	}

	{
		disxx::ui::Button btn{150, 100, 100, 40};
		btn.SetColor(0.3f, 0.3f, 0.3f);
		btn.SetText("OK");
		this->m_pWindow->AddWidget(std::make_unique<disxx::ui::Button>(btn));
	}
}

FileInput *FileInput::Init(void) noexcept(false)
{
	if (!s_pInstance) [[likely]]
		s_pInstance = new FileInput{};
	return s_pInstance;
}

void FileInput::SetPath(const std::filesystem::path &path) noexcept
{
	static_cast<disxx::ui::TextInput *>
	(
		this->m_pWindow
			->GetWidgets()
			.begin()
			->get()
	)->SetText(path.string());
}

const std::filesystem::path &FileInput::GetPath(void) const noexcept
{ return this->m_Path; }

void FileInput::SetCallback(std::function<void(void)> func) noexcept
{
	static_cast<disxx::ui::Button *>
	(
		this
			->m_pWindow
			->GetWidgets()
			.rbegin()
			->get()
	)->SetCallback
	(
		disxx::ui::Button::ButtonTrigger::BTN_CLICKED,
		std::bind
		(
			[](FileInput *const obj, std::function<void(void)> func) mutable -> void
			{
				obj->m_Path = static_cast<disxx::ui::TextInput *>
				(
					obj
						->m_pWindow
						->GetWidgets()
						.begin()
						->get()
				)->GetText();
				func();
			},
			static_cast<FileInput *>(this),
			func
		)
	);
}
