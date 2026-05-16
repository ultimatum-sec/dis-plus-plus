module;

#include <disconf.hpp>

#include <memory>
#include <array>

export module Application;

export import <span>;

import disxx.ui.MainWindow;
import disxx.ui.Widget;
import FileInput;

export class __DISXX_PRIVATE__ [[nodiscard]] Application final : public disxx::ui::MainWindow
{
  private:
	static Application *s_pInstance;
	std::span<const char *> m_Args;
	FileInput *m_pInput;

  private:
	virtual void __KeyboardFunc(unsigned char, int, int) noexcept(false) override;
    virtual void __MouseFunc(int, int, int, int) noexcept(false) override;
    virtual void __ReshapeFunc(int, int) noexcept(false) override;
    virtual void __MotionFunc(int, int) noexcept(false) override;
    virtual void __DisplayFunc(void) noexcept override;

	static void __InitFunc(const disxx::ui::MainWindow *const) noexcept(false);

	explicit Application(void) noexcept = delete;
	explicit Application(std::span<const char *>) noexcept(false);
	
	Application(const Application &) noexcept(false) = delete;
	Application &operator=(const Application &) noexcept(false) = delete;

  public:
	// THIS FUNCTION CALLS ONCE!
	static Application *Init(int &, const char *[]) noexcept(false);
	
	virtual ~Application(void) noexcept override;
	int Exec(void) const noexcept(false);
};
