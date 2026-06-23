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
	static FileInput *s_pInstance;

  private:
	std::unique_ptr<disxx::ui::MainWindow> &m_pWindow;
	std::filesystem::path m_Path;

  private:
	explicit FileInput(void) noexcept;
	
	explicit FileInput(const FileInput &) noexcept(false) = delete;
	FileInput &operator=(const FileInput &) noexcept(false) = delete;
  
  public:
	static FileInput *Init(void) noexcept(false);

	~FileInput(void) noexcept;

	// If argv has the file path
	void SetPath(const std::filesystem::path &) noexcept;
	const std::filesystem::path &GetPath(void) const noexcept;
	void SetCallback(const std::function<void(void)>) noexcept;
};
