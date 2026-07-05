module;

#include <disconf.hpp>

#include <functional>
#include <memory>
#include <array>

export module Application;

export import <filesystem>;

import disxx.ui.MainWindow;
import disxx.ui.Widget;
import FileInput;
import DisLog;

export class __DISXX_PRIVATE__ [[nodiscard]] Application
{
  private:
	static Application *s_pInstance;
	
  private:
	std::reference_wrapper<std::unique_ptr<disxx::ui::MainWindow>> m_pWindow;
	DisLog m_Logger{};
	FileInput *m_pInput{};

  private:
	static void Init(void) noexcept(false);

  private:
	explicit Application(void) noexcept(false);
	
	Application(const Application &) noexcept(false) = delete;
	Application &operator=(const Application &) noexcept(false) = delete;

	void Disassemble(const std::filesystem::path &) noexcept(false);

  public:
	// THIS FUNCTION CALLS ONCE!
	static Application *Init(int &, char **) noexcept(false);
	
	~Application(void) noexcept;
	int Exec(void) const noexcept(false);
};
