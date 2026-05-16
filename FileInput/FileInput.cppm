module;

#include <filesystem>
#include <functional>
#include <span>

export module FileInput;

import disxx.ui.MainWindow;
import disxx.ui.TextInput;
import disxx.ui.Button;
import disxx.ui.Widget;

export class FileInput final : public disxx::ui::MainWindow
{
  private:
	static FileInput *s_pInstance;
	std::span<const char *> m_Args;
	std::filesystem::path m_Path;

  private:
	explicit FileInput(void) noexcept;
	explicit FileInput(std::span<const char *>) noexcept(false);
	explicit FileInput(const FileInput &) noexcept(false) = delete;
	FileInput &operator=(const FileInput &) noexcept(false) = delete;
  
  protected:
	virtual void __KeyboardFunc(unsigned char, int, int) noexcept(false) override;
    virtual void __MouseFunc(int, int, int, int) noexcept(false) override;
    virtual void __ReshapeFunc(int, int) noexcept(false) override;
    virtual void __MotionFunc(int, int) noexcept(false) override;
    virtual void __DisplayFunc(void) noexcept override;
 
  public:
	static FileInput *Init(std::span<const char *>) noexcept(false);

	virtual ~FileInput(void) noexcept override;
	// If argv has the file path
	void SetPath(const std::filesystem::path &) noexcept;
	const std::filesystem::path &GetPath(void) const noexcept;
	void SetCallback(const std::function<void(const disxx::ui::MainWindow *const)>) noexcept;
};




