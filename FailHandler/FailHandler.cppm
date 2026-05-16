module;

#include <disconf.hpp>

#include <span>

export module FailHandler;

import disxx.utility.ini.Parser;
import disxx.ui.MainWindow;

export class __DISXX_PRIVATE__ [[nodiscard]] FailHandler final : public disxx::ui::MainWindow
{
  private:
	static Application *s_pInstance;

  private:
	std::span<const char *> m_Args;
	disxx::utility::ini::Parser m_Parser;

  private:
	virtual void __KeyboardFunc(unsigned char, int, int) noexcept(false) override;
    virtual void __MouseFunc(int, int, int, int) noexcept(false) override;
    virtual void __ReshapeFunc(int, int) noexcept(false) override;
    virtual void __MotionFunc(int, int) noexcept(false) override;
    virtual void __DisplayFunc(void) noexcept override;

	explicit FailHandler(void) noexcept = delete;
	explicit FailHandler(std::span<const char *>) noexcept(false);
	
	FailHandler(const FailHandler &) noexcept(false) = delete;
	FailHandler &operator=(const FailHandler &) noexcept(false) = delete;

  public:
	// THIS FUNCTION CALLS ONCE!
	static FailHandler *Init(int &, const char *[]) noexcept(false);
	
	virtual ~FailHandler(void) noexcept override;
	int Exec(void) const noexcept(false);
}
