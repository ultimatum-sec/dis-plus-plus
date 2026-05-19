module;

#include <disconf.hpp>

export module disxx.disasm.decoder.DataProcessingRegister.Decoder;

export import <expected>;
export import <cstdint>;
export import <memory>;

import disxx.disasm.decoder.abstract.Decoder;
import disxx.utility.error.DisassemblyError;

export namespace disxx::disasm::decoder::DataProcessingRegister
{
	class __DISXX_PRIVATE__ [[nodiscard]] Decoder final : public disxx::disasm::decoder::abstract::Decoder
	{
	  protected:
		virtual std::expected
		<
			std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder>,
			disxx::utility::error::DisassemblyError
		> __GetDecoder(void) const noexcept override;

	  public:
		explicit Decoder(void) noexcept;
		explicit Decoder(std::uint32_t, std::uint64_t) noexcept;
	
		explicit Decoder(const Decoder &other) noexcept(false);
		Decoder &operator=(const Decoder &other) noexcept(false);

		explicit Decoder(Decoder &&other) noexcept;
		Decoder &operator=(Decoder &&other) noexcept;
	};
} /* disxx::disasm::decoder::LoadsAndStores */
