ifeq ($(shell echo %OS%), Windows_NT)
	include ..\\..\\common.mk
else
	include ../../common.mk
endif

BUILDDIR=.$(PATHSEP)build
DEPENDENCIESDIR=$(BUILDDIR)$(PATHSEP)dependencies
PRECOMPILEDDIR=$(BUILDDIR)$(PATHSEP)precompiled
OBJECTSDIR=$(BUILDDIR)$(PATHSEP)objects

CXXFLAGS=\
	-c -std=c++26 -O3 -Weverything -Werror -arch $(ARCH) -isysroot $(shell xcrun --show-sdk-path) \
	-Wno-weak-vtables -Wno-c++98-compat -Wno-implicit-int-conversion -Wno-reserved-macro-identifier \
	-Wno-padded -Wno-c++98-c++11-compat-binary-literal -Wno-sign-conversion -Wno-reserved-identifier \
	-Wno-c++98-compat-pedantic -Wno-global-constructors -Wno-exit-time-destructors -Wno-switch-enum \
	-Wno-nrvo -Wno-undefined-func-template -Wno-shift-count-overflow -Wno-integer-overflow \
	-ftrapv -fmodules -fcxx-modules -fvisibility=hidden -fprebuilt-module-path=$(PRECOMPILEDDIR) \
	-Xclang -fmodules-local-submodule-visibility -I..$(PATHSEP)include -fstack-protector-all -DNDEBUG

LDFLAGS=\
	-dylib -O2 $(LTOFLAGS) -demangle -S -arch $(ARCH) -syslibroot $(shell xcrun --show-sdk-path) -lSystem -lc++ \
	-L$(DEPENDENCIESDIR) -ldisxx-util

ifeq ($(OS), Darwin)
	LDFLAGS+=-install_name @executable_path/../Frameworks/libdisxx-disasm.$(DYLIB)
else
	ifne ($(OS), Windows_NT)
		LDFLAGS+=-install_name $$ORIGIN/libdisxx-disasm.$(DYLIB)
endif

OPERANDS=.$(PATHSEP)operand$(PATHSEP)
INSNS=.$(PATHSEP)instruction$(PATHSEP)
 
MODULES=\
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)public$(PATHSEP)trait$(PATHSEP)disxx.utility.trait.Cloneable.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)error$(PATHSEP)disxx.utility.error.DisassemblyError.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)error$(PATHSEP)disxx.utility.error.NullPointerError.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)private$(PATHSEP)pointer$(PATHSEP)disxx.utility.pointer.DefaultDeleter.cppm \
	..$(PATHSEP)modules$(PATHSEP)utility$(PATHSEP)public$(PATHSEP)pointer$(PATHSEP)disxx.utility.pointer.NonNull.cppm \
	.$(PATHSEP)utility$(PATHSEP)bits$(PATHSEP)disxx.disasm.utility.bits.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)private$(PATHSEP)disxx.disasm.InstructionID.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.IOperand.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.Register-VectorArrangementSpecifier.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.Register.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.Immediate.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.Extension.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.PState.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.Shift.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.LoadsAndStoresAddress.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.PrefetchOperand.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.SystemOperand.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.MemoryBarrier.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)operand$(PATHSEP)disxx.disasm.operand.Condition.cppm \
	.$(PATHSEP)decoder$(PATHSEP)abstract$(PATHSEP)disxx.disasm.decoder.abstract.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)abstract$(PATHSEP)disxx.disasm.decoder.abstract.Decoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.EvaluateIntoFlags.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.ConditionalCompareImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.AddSubstractCheckedPointer.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.ConditionalSelect.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.DataProcessing1Source.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.DataProcessing3Source.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.ConditionalCompareRegister.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.AddSubstractWithCarry.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.RotateRightIntoFlags.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.LogicalShiftedRegister.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.AddSubstractShiftedRegister.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.DataProcessing2Source.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.AddSubstractExtendedRegister.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.Decoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.MinMaxImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.Bitfield.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.LogicalImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.AddSubstractImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.DataProcessing1SourceImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.PCRelAddressing.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.AddSubstractImmediateWithTags.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.MoveWideImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.Extract.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.Decoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)Reserved$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.Reserved.UDF.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)Reserved$(PATHSEP)disxx.disasm.decoder.Reserved.Decoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDModifiedImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeSameExtra.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDVectorXIndexedElement.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicThreeRegisterSHA512.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeSame.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.ConversionBetweenFloatingPointAndFixedPoint.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeDifferent.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDCopy.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointConditionalCompare.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarShiftByImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing2Source.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeRegisterExtension.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.XAR.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeSameFP16.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicTwoRegisterSHA512.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicThreeRegisterSHA.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeSameFP16.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeDifferent.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDAcrossLanes.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarCopy.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDShiftByImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarPairwise.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarXIndexedElement.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarTwoRegisterMiscellaneousFP16.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicTwoRegisterSHA.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointConditionalSelect.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDPermute.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeSame.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarTwoRegisterMiscellaneous.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.ConversionBetweenFloatingPointAndInteger.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTwoRegisterMiscellaneous.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDExtract.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicAES.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTableLookup.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing3Source.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTwoRegisterMiscellaneousFP16.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicThreeRegisterImm2.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicFourRegister.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointCompare.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing1Source.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.Decoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.Decoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.ExclusiveRegister.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.NoAllocatePairOffset.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.OneHundredAndTwentyEightBitAtomicMemoryOperations.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.GCS.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.AtomicMemoryOperations.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.SIMDSingleStructurePostIndexed.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterPairPostIndexed.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterPAC.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.CompareAndSwapPairUnprivileged.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.LDAPRSTLRSIMDAndFP.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.CompareAndSwapUnprivileged.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterUnscaledImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterPairPreIndexed.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.SIMDMultipleStructures.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.LDAPRSTLRWriteback.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterUnprivileged.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.CompareAndSwapPair.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.AtomicMemoryOperationsUnprivileged.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.ExclusiveRegisterUnprivileged.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.MemoryCopyAndMemorySet.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.Ordered.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterImmediatePreIndexed.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterPairOffset.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterLiteral.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.LDAPRSTLRUnscaledImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.LDIAPPSTILP.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.SIMDSingleStructure.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterUnsignedImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.SIMDMultipleStructuresPostIndexed.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RCWCompareAndSwapPair.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.CompareAndSwap.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RCWCompareAndSwap.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.MemoryTags.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterImmediatePostIndexed.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.ExclusivePair.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterOffset.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Decoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemInstructionIDsWithRegisterArgument.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemRegisterPairMove.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.ExceptionGeneration.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.UnconditionalBranchRegister.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareAndBranchImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareBytesHalfwordsInRegisterAndBranch.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareRegisterWithImmediateAndBranch.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Hints.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.TestAndBranchImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareRegistersAndBranch.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemPairInstructionIDs.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.MiscellaneousBranchImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Barriers.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemInstructionIDs.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.UnconditionalBranchImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.PSTATE.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemWithResult.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.ConditionalBranchImmediate.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemRegisterMove.SubDecoder.cppm \
	.$(PATHSEP)decoder$(PATHSEP)DecoderFactory$(PATHSEP)disxx.disasm.decoder.DecoderFactory.cppm \
    .$(PATHSEP)decoder$(PATHSEP)DecoderFactory$(PATHSEP)disxx.disasm.decoder.DecoderFactory-PseudoDecoder.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)disxx.disasm.Address.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)disxx.disasm.Bytes.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)disxx.disasm.Instruction.cppm \
	..$(PATHSEP)modules$(PATHSEP)disasm$(PATHSEP)public$(PATHSEP)disxx.disasm.Disassembler.cppm

SOURCES=\
	.$(PATHSEP)utility$(PATHSEP)bits$(PATHSEP)disxx.disasm.utility.bits.cpp \
	$(OPERANDS)disxx.disasm.operand.Register.cpp \
	$(OPERANDS)disxx.disasm.operand.Extension.cpp \
	$(OPERANDS)disxx.disasm.operand.PState.cpp \
	$(OPERANDS)disxx.disasm.operand.LoadsAndStoresAddress.cpp \
	$(OPERANDS)disxx.disasm.operand.PrefetchOperand.cpp \
	$(OPERANDS)disxx.disasm.operand.SystemOperand.cpp \
	$(OPERANDS)disxx.disasm.operand.MemoryBarrier.cpp \
	$(OPERANDS)disxx.disasm.operand.Shift.cpp \
	$(OPERANDS)disxx.disasm.operand.Condition.cpp \
	.$(PATHSEP)decoder$(PATHSEP)abstract$(PATHSEP)disxx.disasm.decoder.abstract.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)abstract$(PATHSEP)disxx.disasm.decoder.abstract.Decoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.AddSubstractShiftedRegister.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.LogicalShiftedRegister.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.DataProcessing2Source.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.DataProcessing3Source.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.AddSubstractWithCarry.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.AddSubstractExtendedRegister.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.ConditionalCompareImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.EvaluateIntoFlags.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.RotateRightIntoFlags.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.ConditionalSelect.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.DataProcessing1Source.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.ConditionalCompareRegister.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.AddSubstractCheckedPointer.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingRegister$(PATHSEP)disxx.disasm.decoder.DataProcessingRegister.Decoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.MoveWideImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.Bitfield.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.Extract.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.DataProcessing1SourceImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.MinMaxImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.AddSubstractImmediateWithTags.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.PCRelAddressing.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.AddSubstractImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.LogicalImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingImmediate$(PATHSEP)disxx.disasm.decoder.DataProcessingImmediate.Decoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)Reserved$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.Reserved.UDF.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)Reserved$(PATHSEP)disxx.disasm.decoder.Reserved.Decoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointCompare.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing3Source.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeSameFP16.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.ConversionBetweenFloatingPointAndFixedPoint.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeSameExtra.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarTwoRegisterMiscellaneousFP16.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointConditionalSelect.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTableLookup.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDAcrossLanes.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarPairwise.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing2Source.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTwoRegisterMiscellaneous.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicAES.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeSame.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDModifiedImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicTwoRegisterSHA.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicTwoRegisterSHA512.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeDifferent.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDExtract.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDPermute.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeDifferent.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarTwoRegisterMiscellaneous.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarXIndexedElement.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarCopy.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicThreeRegisterSHA.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicThreeRegisterImm2.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDVectorXIndexedElement.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicFourRegister.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeSameFP16.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTwoRegisterMiscellaneousFP16.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointConditionalCompare.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeSame.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.ConversionBetweenFloatingPointAndInteger.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.XAR.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing1Source.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDShiftByImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeRegisterExtension.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarShiftByImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDCopy.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicThreeRegisterSHA512.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DataProcessingScalarFPAndAdvancedSIMD$(PATHSEP)disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.Decoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)DecoderFactory$(PATHSEP)disxx.disasm.decoder.DecoderFactory.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.LDAPRSTLRWriteback.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.MemoryCopyAndMemorySet.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.CompareAndSwapUnprivileged.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.CompareAndSwapPairUnprivileged.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.CompareAndSwap.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.OneHundredAndTwentyEightBitAtomicMemoryOperations.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.GCS.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.AtomicMemoryOperationsUnprivileged.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterImmediatePostIndexed.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.LDIAPPSTILP.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterPairPreIndexed.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterImmediatePreIndexed.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.NoAllocatePairOffset.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.SIMDSingleStructurePostIndexed.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.SIMDSingleStructure.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.LDAPRSTLRSIMDAndFP.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.SIMDMultipleStructures.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RCWCompareAndSwapPair.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterPairOffset.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterPairPostIndexed.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.AtomicMemoryOperations.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.Ordered.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.CompareAndSwapPair.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterLiteral.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.SIMDMultipleStructuresPostIndexed.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RCWCompareAndSwap.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.ExclusiveRegisterUnprivileged.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterUnprivileged.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterPAC.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterUnsignedImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.LDAPRSTLRUnscaledImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.ExclusivePair.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.ExclusiveRegister.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.MemoryTags.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterOffset.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.RegisterUnscaledImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)LoadsAndStores$(PATHSEP)disxx.disasm.decoder.LoadsAndStores.Decoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Decoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.TestAndBranchImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.UnconditionalBranchRegister.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Barriers.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemRegisterMove.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.UnconditionalBranchImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.ConditionalBranchImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareRegisterWithImmediateAndBranch.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemRegisterPairMove.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemInstructionIDs.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareAndBranchImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemWithResult.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemPairInstructionIDs.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemInstructionIDsWithRegisterArgument.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareBytesHalfwordsInRegisterAndBranch.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Hints.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.PSTATE.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareRegistersAndBranch.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.MiscellaneousBranchImmediate.SubDecoder.cpp \
	.$(PATHSEP)decoder$(PATHSEP)BranchesExceptionsAndSystemInstructions$(PATHSEP)SubDecoder$(PATHSEP)disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.ExceptionGeneration.SubDecoder.cpp \
	.$(PATHSEP)disxx.disasm.Address.cpp \
	.$(PATHSEP)disxx.disasm.Instruction.cpp \
	.$(PATHSEP)disxx.disasm.Disassembler.cpp

.PHONY: all clean
all: $(BUILDDIR)$(PATHSEP)libdisxx-disasm.$(DYLIB)

UTILITIES=$(DEPENDENCIESDIR)$(PATHSEP)libdisxx-util.$(DYLIB)

PRECOMPILED=$(addprefix $(PRECOMPILEDDIR)$(PATHSEP), $(notdir $(MODULES:.cppm=.pcm)))
MOBJECTS=$(PRECOMPILED:.pcm=.o)
OBJECTS=$(addprefix $(OBJECTSDIR)$(PATHSEP), $(notdir $(SOURCES:.cpp=.o)))

vpath %.cppm $(sort $(dir $(MODULES)))
vpath %.cpp $(sort $(dir $(SOURCES)))
vpath %.pcm $(sort $(dir $(PRECOMPILED)))

$(DEPENDENCIESDIR):
	$(MKDIR) $@
$(PRECOMPILEDDIR):
	$(MKDIR) $@
$(OBJECTSDIR):
	$(MKDIR) $@
$(UTILITIES): | $(DEPENDENCIESDIR)
	make -C ..$(PATHSEP)utility
	$(MOVE) ..$(PATHSEP)utility$(PATHSEP)build$(PATHSEP)$(notdir $@) $@
$(PRECOMPILED): $(PRECOMPILEDDIR)$(PATHSEP)%.pcm: %.cppm | $(PRECOMPILEDDIR)
	$(CXX) $(CXXFLAGS) -Xclang -emit-module-interface --precompile -Wno-unused-command-line-argument $< -o $@
$(MOBJECTS): $(PRECOMPILEDDIR)$(PATHSEP)%.o: $(PRECOMPILEDDIR)$(PATHSEP)%.pcm | $(PRECOMPILEDDDIR)
	$(CXX) $(CXXFLAGS) -Wno-unused-command-line-argument $< -o $@
$(OBJECTS): $(OBJECTSDIR)$(PATHSEP)%.o: %.cpp | $(OBJECTSDIR)
	$(CXX) $(CXXFLAGS) $< -o $@
$(BUILDDIR)$(PATHSEP)libdisxx-disasm.$(DYLIB): $(MOBJECTS) $(OBJECTS) | $(UTILITIES)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	$(RMDIR) $(BUILDDIR)
