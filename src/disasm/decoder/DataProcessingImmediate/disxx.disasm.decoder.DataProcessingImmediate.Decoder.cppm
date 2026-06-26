module;

#include <disconf.hpp>

#include <cstdint>
#include <memory>

export module disxx.disasm.decoder.DataProcessingImmediate.Decoder;

import disxx.disasm.decoder.abstract.Decoder;
import disxx.utility.error.DisassemblyError;

export namespace disxx::disasm::decoder::DataProcessingImmediate
{
	class __DISXX_PRIVATE__ [[nodiscard]] Decoder final : public disxx::disasm::decoder::abstract::Decoder
	{
	  protected:
		std::expected
		<
			std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder>,
			disxx::utility::error::DisassemblyError
		> __GetDecoder(void) const noexcept override;

	  public:
		explicit Decoder(void) noexcept;
		explicit Decoder(std::uint32_t, std::uint64_t) noexcept;
	
		explicit Decoder(const Decoder &other) noexcept;
		Decoder &operator=(const Decoder &other) noexcept;

		explicit Decoder(Decoder &&other) noexcept;
		Decoder &operator=(Decoder &&other) noexcept;
	};
} /* disxx::disasm::decoder::DataProcessingImmediate */
