module;

#include <filesystem>
#include <functional>
#include <memory>
#include <span>

export module FileInput;

import disxx.ui.MainWindow;
import disxx.ui.TextInput;
import disxx.ui.Button;
import disxx.ui.Widget;

export class FileInput
{
  private:
	disxx::ui::MainWindow m_Window{};
	std::filesystem::path m_Path{};

  public:
	explicit FileInput(void) noexcept;

	FileInput(const FileInput &) noexcept;
	FileInput &operator=(const FileInput &) noexcept;

	FileInput(FileInput &&) noexcept;
	FileInput &operator=(FileInput &&) noexcept;

	~FileInput(void) noexcept = default;

	// If argv has the file path
	void SetPath(const std::filesystem::path &) noexcept;
	const std::filesystem::path &GetPath(void) const noexcept;
	void SetCallback(const std::function<void(void)>) noexcept;
};
