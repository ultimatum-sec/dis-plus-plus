module;

#include <disconf.hpp>

#include <functional>
#include <memory>

export module FailHandler;

import disxx.utility.ini.Parser;
import disxx.ui.MainWindow;

export class __DISXX_PRIVATE__ [[nodiscard]] FailHandler
{
  private:
	static FailHandler *s_pInstance;

  private:
	disxx::ui::MainWindow m_Window{};
	disxx::utility::ini::Parser m_Parser{};

  private:
	explicit FailHandler(void) noexcept(false);
	
	FailHandler(const FailHandler &) noexcept(false) = delete;
	FailHandler &operator=(const FailHandler &) noexcept(false) = delete;

  public:
	// THIS FUNCTION CALLS ONCE!
	static FailHandler *Init(int &, const char *[]) noexcept(false);
	
	~FailHandler(void) noexcept = default;
	int Exec(void) const noexcept(false);
};
