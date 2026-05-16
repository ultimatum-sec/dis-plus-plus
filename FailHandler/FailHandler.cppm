module;

#include <disconf.hpp>

export module FailHandler;

class [[nodiscard]] FailHandler final : public disxx::ui::MainWindow
{
  private:
	disxx::utility::wrapper::Pointer<std::shared_ptr<std::ostream>> m_pLogStream;
}
