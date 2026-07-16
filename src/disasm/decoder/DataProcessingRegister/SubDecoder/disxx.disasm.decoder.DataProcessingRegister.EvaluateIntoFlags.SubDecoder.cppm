module;

#include <disconf.hpp>

#include <cstdint>
#include <memory>

export module disxx.disasm.decoder.DataProcessingRegister.EvaluateIntoFlags.SubDecoder;

import disxx.disasm.decoder.abstract.SubDecoder;

export namespace disxx::disasm::decoder::DataProcessingRegister::EvaluateIntoFlags
{
	class __DISXX_PRIVATE__ [[nodiscard]] SubDecoder final : public disxx::disasm::decoder::abstract::SubDecoder
	{
	  public:
		explicit SubDecoder(void) noexcept;
		explicit SubDecoder(std::uint32_t, std::uint64_t) noexcept;

		explicit SubDecoder(const SubDecoder &) noexcept;
		SubDecoder &operator=(const SubDecoder &) noexcept;

		explicit SubDecoder(SubDecoder &&) noexcept;
		SubDecoder &operator=(SubDecoder &&) noexcept;

		virtual std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder> Clone(void) const noexcept override;
		virtual DisassemblyResult Decode(void) const noexcept override;
	};
} /* disxx::disasm::decoder::DataProcessingRegister::EvaluateIntoFlags */
