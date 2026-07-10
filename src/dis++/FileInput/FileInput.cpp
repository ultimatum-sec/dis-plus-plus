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

import disxx.ui.TextInput;
import disxx.ui.Widget;
import disxx.ui.Button;
import disxx.ui.Label;
import disxx.ui.Frame;

FileInput::FileInput(void) noexcept
	: m_Window{disxx::ui::utility::Vec2<int>{400, 300}, "Main menu"}
	, m_Callback{[] -> void {}}
	, m_Path{}
{
	this->m_Window.SetVisible(true);
	
	{
		disxx::ui::Button open{50.f, 250.f, 100.f, 25.f};
		open.SetColor(0.3f, 0.3f, 0.3f);
		open.SetText("Open...");
		open.SetCallback
		(
			disxx::ui::Button::Trigger::BTN_CLICKED,
			[this](const disxx::ui::Widget *const) -> void
			{
				disxx::ui::Frame frame{0.f, 0.f, 400.f, 300.f};
				frame.SetColor(0.3f, 0.3f, 0.3f);
				this->m_Window.AddWidget(std::make_unique<disxx::ui::Frame>(frame));

				disxx::ui::TextInput input{125.f, 150.f, 150.f, 40.f};
				input.SetColor(4.f, 4.f, 4.f);
				this->m_Window.AddWidget(std::make_unique<disxx::ui::TextInput>(input));
				
				const auto &ref
				{
					dynamic_cast<disxx::ui::TextInput &>
					(
						**this
							->m_Window
							.GetWidgets()
							.rbegin()
					)
				};
	
				disxx::ui::Button ok{150.f, 100.f, 100.f, 40.f};
				ok.SetColor(3.f, 3.f, 3.f);
				ok.SetText("OK");
				ok.SetCallback
				(
					disxx::ui::Button::Trigger::BTN_CLICKED,
					[this, ref](const disxx::ui::Widget *const) mutable -> void
					{
						this->m_Path = ref.GetText();
						this->m_Callback();
					}
				);
				this->m_Window.AddWidget(std::make_unique<disxx::ui::Button>(ok));
			}
		);
		this->m_Window.AddWidget(std::make_unique<disxx::ui::Button>(open));
	}

	{
		disxx::ui::Label text{175.f, 250.f, 0.f, 0.f};
		text.SetColor(1.f, 1.f, 1.f);
		text.SetText("Select an executable to disassemble");
		this->m_Window.AddWidget(std::make_unique<disxx::ui::Label>(text));
	}

	{
		disxx::ui::Button pass{50.f, 200.f, 100.f, 25.f};
		pass.SetColor(0.3f, 0.3f, 0.3f);
		pass.SetText("Pass");
		pass.SetCallback
		(
			disxx::ui::Button::Trigger::BTN_CLICKED,
			[this](const disxx::ui::Widget *const) -> void
			{ this->m_Callback(); }
		);
		this->m_Window.AddWidget(std::make_unique<disxx::ui::Button>(pass));
	}

	{
		disxx::ui::Label text{175.f, 200.f, 0.f, 0.f};
		text.SetColor(1.f, 1.f, 1.f);
		text.SetText("Continue without opening a file");
		this->m_Window.AddWidget(std::make_unique<disxx::ui::Label>(text));
	}
}

FileInput::FileInput(const FileInput &other) noexcept
	: m_Window{other.m_Window}
	, m_Callback{other.m_Callback}
	, m_Path{other.m_Path}
{}

FileInput &FileInput::operator=(const FileInput &other) noexcept
{
	if (this != &other) [[likely]]
	{
		this->m_Window = other.m_Window;
		this->m_Callback = other.m_Callback;
		this->m_Path = other.m_Path;
	}

	return *this;
}

FileInput::FileInput(FileInput &&other) noexcept
	: m_Window{std::move(other.m_Window)}
	, m_Callback{std::move(other.m_Callback)}
	, m_Path{std::move(other.m_Path)}
{}

FileInput &FileInput::operator=(FileInput &&other) noexcept
{
	if (this != &other) [[likely]]
	{
		this->m_Window = std::move(other.m_Window);
		this->m_Callback = std::move(other.m_Callback);
		this->m_Path = std::move(other.m_Path);
	}

	return *this;
}

void FileInput::SetCallback(std::function<void(void)> func) noexcept
{ this->m_Callback = func; }

const std::filesystem::path &FileInput::GetPath(void) const noexcept
{ return this->m_Path; }
