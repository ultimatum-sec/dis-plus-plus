module;

#include <disconf.hpp>

#include <functional>
#include <memory>
#include <array>

export module Application;

export import <span>;

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
	DisLog m_Logger;
	std::span<const char *> m_Args;
	FileInput *m_pInput;

  private:
	static void __InitFunc(void) noexcept(false);

  private:
	explicit Application(void) noexcept = delete;
	explicit Application(std::span<const char *>) noexcept(false);
	
	Application(const Application &) noexcept(false) = delete;
	Application &operator=(const Application &) noexcept(false) = delete;

  public:
	// THIS FUNCTION CALLS ONCE!
	static Application *Init(int &, const char *[]) noexcept(false);
	
	~Application(void) noexcept;
	int Exec(void) const noexcept(false);
};
