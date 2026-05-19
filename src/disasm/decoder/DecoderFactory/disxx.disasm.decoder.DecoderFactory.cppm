module;

#include <disconf.hpp>

#include <cstdint>
#include <memory>

export module disxx.disasm.decoder.DecoderFactory;

import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.Decoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Decoder;
import disxx.disasm.decoder.DataProcessingImmediate.Decoder;
import disxx.disasm.decoder.DataProcessingRegister.Decoder;
import disxx.disasm.decoder.LoadsAndStores.Decoder;
import disxx.disasm.decoder.Reserved.Decoder;
import disxx.disasm.utility.bits;

export import disxx.disasm.decoder.abstract.Decoder;

export namespace disxx::disasm::decoder
{
	class __DISXX_PRIVATE__ [[nodiscard]] DecoderFactory
	{
	  public:
		static std::unique_ptr<abstract::Decoder> GetDecoder(std::uint32_t insn, std::uint64_t PC) noexcept;
	};
} /* disxx::disasm */
