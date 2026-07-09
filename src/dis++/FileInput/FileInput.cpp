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

module FileInput;

import DisLog;

FileInput::FileInput(void) noexcept
	: m_Window{disxx::ui::utility::Vec2<int>{400, 300}, "Main menu"}
	, m_Path{}
{
	this->m_Window.SetVisible(true);

	{
		disxx::ui::TextInput txt{75, 150, 250, 40};
		txt.SetColor(0.3f, 0.3f, 0.3f);
		this->m_Window.AddWidget(std::make_unique<disxx::ui::TextInput>(txt));
	}

	{
		disxx::ui::Button btn{150, 100, 100, 40};
		btn.SetColor(0.3f, 0.3f, 0.3f);
		btn.SetText("OK");
		this->m_Window.AddWidget(std::make_unique<disxx::ui::Button>(btn));
	}
}

FileInput::FileInput(const FileInput &other) noexcept
	: m_Window{other.m_Window}
	, m_Path{other.m_Path}
{}

FileInput &FileInput::operator=(const FileInput &other) noexcept
{
	if (this != &other) [[likely]]
	{
		this->m_Window = other.m_Window;
		this->m_Path = other.m_Path;
	}

	return *this;
}

FileInput::FileInput(FileInput &&other) noexcept
	: m_Window{std::move(other.m_Window)}
	, m_Path{std::move(other.m_Path)}
{}

FileInput &FileInput::operator=(FileInput &&other) noexcept
{
	this->m_Window = std::move(other.m_Window);
	this->m_Path = std::move(other.m_Path);

	return *this;
}

void FileInput::SetPath(const std::filesystem::path &path) noexcept
{
	static_cast<disxx::ui::TextInput *>
	(
		this->m_Window
			.GetWidgets()
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
			->m_Window
			.GetWidgets()
			.rbegin()
			->get()
	)->SetCallback
	(
		disxx::ui::Button::Trigger::BTN_CLICKED,
		std::bind
		(
			[](FileInput *const obj, std::function<void(void)> function) mutable -> void
			{
				obj->m_Path = static_cast<disxx::ui::TextInput *>
				(
					obj
						->m_Window
						.GetWidgets()
						.begin()
						->get()
				)->GetText();
				function();
			},
			static_cast<FileInput *>(this),
			func
		)
	);
}
