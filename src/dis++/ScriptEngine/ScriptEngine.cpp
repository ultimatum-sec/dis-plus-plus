module;

#include <Ruby/ruby.h>

#include <utility>
#include <format>
#include <bit>

module ScriptEngine;

import disxx.loader.macho.Loader;
//import disxx.disasm.Disassembler;

namespace
{
	static constexpr rb_data_type_t loader
	{
		"Disxx::Loader",
		{
			nullptr,
			[](void *) -> void {},
			[](const void *) -> std::size_t { return sizeof(disxx::loader::macho::Loader *); },
			{nullptr, nullptr}
		},
		nullptr,
		nullptr,
		RUBY_TYPED_FREE_IMMEDIATELY
	};

	static constexpr rb_data_type_t executable
	{
		"Disxx::ExecutableFile",
		{
			nullptr,
			[](void *) -> void {},
			[](const void *) -> std::size_t { return sizeof(disxx::loader::executable::ExecutableFile *); },
			{nullptr, nullptr}
		},
		nullptr,
		nullptr,
		RUBY_TYPED_FREE_IMMEDIATELY
	};

	static constexpr rb_data_type_t section
	{
		"Disxx::Section",
		{
			nullptr,
			[](void *) -> void {},
			[](const void *) -> std::size_t { return sizeof(disxx::loader::executable::Section *); },
			{nullptr, nullptr}
		},
		nullptr,
		nullptr,
		RUBY_TYPED_FREE_IMMEDIATELY
	};

	static constexpr rb_data_type_t label
	{
		"Disxx::Label",
		{
			nullptr,
			[](void *) -> void {},
			[](const void *) -> std::size_t { return sizeof(disxx::loader::executable::Section *); },
			{nullptr, nullptr}
		},
		nullptr,
		nullptr,
		RUBY_TYPED_FREE_IMMEDIATELY
	};

	template <typename T>
	inline T *unwrap(VALUE) noexcept
	{ return nullptr; }

	template <>
	inline disxx::loader::macho::Loader *unwrap<disxx::loader::macho::Loader>(VALUE val) noexcept
	{
		disxx::loader::macho::Loader *ptr{nullptr};
		TypedData_Get_Struct(val, disxx::loader::macho::Loader, &loader, ptr);
		return ptr;
	}

	template <>
	inline disxx::loader::executable::ExecutableFile *unwrap<disxx::loader::executable::ExecutableFile>(VALUE val) noexcept
	{
		disxx::loader::executable::ExecutableFile *ptr{nullptr};
		TypedData_Get_Struct(val, disxx::loader::executable::ExecutableFile, &executable, ptr);
		return ptr;
	}

	template <>
	inline disxx::loader::executable::Section *unwrap<disxx::loader::executable::Section>(VALUE val) noexcept
	{
		disxx::loader::executable::Section *ptr{nullptr};
		TypedData_Get_Struct(val, disxx::loader::executable::Section, &section, ptr);
		return ptr;
	}

	template <>
	inline disxx::loader::executable::Label *unwrap<disxx::loader::executable::Label>(VALUE val) noexcept
	{
		disxx::loader::executable::Label *ptr{nullptr};
		TypedData_Get_Struct(val, disxx::loader::executable::Label, &label, ptr);
		return ptr;
	}
} /* */

ScriptEngine::EngineError::EngineError(void) noexcept
	: m_Err{}
{}

ScriptEngine::EngineError::EngineError(std::string_view err) noexcept
	: m_Err{err.data()}
{}

ScriptEngine::EngineError::EngineError(const EngineError &other) noexcept
	: m_Err{other.m_Err}
{}

ScriptEngine::EngineError &ScriptEngine::EngineError::operator=(const EngineError &other) noexcept
{
	if (this != &other) [[likely]]
		this->m_Err = other.m_Err;
	return *this;
}

ScriptEngine::EngineError::EngineError(EngineError &&other) noexcept
	: m_Err{std::move(other.m_Err)}
{}

ScriptEngine::EngineError &ScriptEngine::EngineError::operator=(EngineError &&other) noexcept
{
	this->m_Err = std::move(other.m_Err);
	return *this;
}

const char *ScriptEngine::EngineError::what(void) const noexcept
{ return this->m_Err.c_str(); }

ScriptEngine::ScriptEngine(void) noexcept
	: m_Disxx{Qnil}
	, m_Loader{Qnil}
	, m_ExecutableFile{Qnil}
	, m_Section{Qnil}
	, m_Label{Qnil}
	, m_Disassembler{Qnil}
{
	int argc{0};
	char **argv = {nullptr};
	ruby_sysinit(&argc, &argv);
	ruby_init();
	ruby_init_loadpath();

	// Variables for using inside lambdas
	static VALUE ldr{Qnil};
	static VALUE sect{Qnil};
	static VALUE lbl{Qnil};
	
	this->m_Disxx = rb_define_module("Disxx");

	this->m_Loader = rb_define_class_under
	(
		this->m_Disxx,
		"Loader",
		rb_cObject
	);
	rb_define_method
	(
		this->m_Loader,
		"loadFile",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self, VALUE path) -> VALUE
			{
				const std::filesystem::path &unwrapped{rb_string_value_cstr(&path)};
				unwrap<disxx::loader::macho::Loader>(self)->LoadFile(unwrapped);
				return Qnil;
			}
		),
		1
	);
	rb_define_method
	(
		this->m_Loader,
		"loadData",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				const auto data{unwrap<disxx::loader::macho::Loader>(self)->LoadData()};
				return TypedData_Wrap_Struct(ldr, &loader, std::bit_cast<void *>(&data));
			}
		),
		0
	);

	this->m_ExecutableFile = rb_define_class_under
	(
		this->m_Disxx,
		"ExecutableFile",
		rb_cObject
	);
	rb_define_method
	(
		this->m_ExecutableFile,
		"getSections",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				auto arr{rb_ary_new()};
				for (auto var : unwrap<disxx::loader::executable::ExecutableFile>(self)->GetSections())
				{
					const auto rubyfied{TypedData_Wrap_Struct(sect, &section, std::bit_cast<void *>(&var))};
					rb_ary_push(arr, rubyfied);
				}

				return arr;
			}
		),
		0
	);
	rb_define_method
	(
		this->m_ExecutableFile,
		"getMagic",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				return UINT2NUM
				(
					unwrap<disxx::loader::executable::ExecutableFile>(self)
						->GetMagic()
				);
			}
		),
		0
	);

	this->m_Section = rb_define_class_under
	(
		this->m_Disxx,
		"Section",
		rb_cObject
	);
	rb_define_method
	(
		this->m_Section,
		"name",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				return rb_str_new_cstr
				(
					unwrap<disxx::loader::executable::Section>(self)
						->GetName()
						.data()
				);
			}
		),
		0
	);
	rb_define_method
	(
		this->m_Section,
		"labels",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				auto arr{rb_ary_new()};
				for (auto var : unwrap<disxx::loader::executable::Section>(self)->GetLabels())
				{
					const auto rubyfied{TypedData_Wrap_Struct(lbl, &label, std::bit_cast<void *>(&var))};
					rb_ary_push(arr, rubyfied);
				}

				return arr;
			}
		),
		0
	);
	rb_define_method
	(
		this->m_Section,
		"address",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				return ULL2NUM
				(
					unwrap<disxx::loader::executable::Section>(self)
						->GetAddress()
				);
			}
		),
		0
	);
	rb_define_method
	(
		this->m_Section,
		"offset",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				return ULL2NUM
				(
					unwrap<disxx::loader::executable::Section>(self)
						->GetOffset()
				);
			}
		),
		0
	);
	rb_define_method
	(
		this->m_Section,
		"index",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				return USHORT2NUM
				(
					unwrap<disxx::loader::executable::Section>(self)
						->GetIndex()
				);
			}
		),
		0
	);

	this->m_Label = rb_define_class_under
	(
		this->m_Disxx,
		"Label",
		rb_cObject
	);
	rb_define_method
	(
		this->m_Label,
		"name",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				return rb_str_new_cstr
				(
					unwrap<disxx::loader::executable::Label>(self)
						->GetName()
						.data()
				);
			}
		),
		0
	);
	rb_define_method
	(
		this->m_Label,
		"address",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				return ULL2NUM
				(
					unwrap<disxx::loader::executable::Label>(self)
						->GetAddress()
				);
			}
		),
		0
	);
	rb_define_method
	(
		this->m_Label,
		"offset",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				return ULL2NUM
				(
					unwrap<disxx::loader::executable::Label>(self)
						->GetOffset()
				);
			}
		),
		0
	);
	rb_define_method
	(
		this->m_Label,
		"offset",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				return ULL2NUM
				(
					unwrap<disxx::loader::executable::Label>(self)
						->GetSize()
				);
			}
		),
		0
	);
	rb_define_method
	(
		this->m_Label,
		"data",
		RUBY_METHOD_FUNC
		(
			+[](VALUE self) -> VALUE
			{
				auto arr{rb_ary_new()};
				for (const auto byte : unwrap<disxx::loader::executable::Label>(self)->GetData<std::uint8_t>())
					rb_ary_push(arr, USHORT2NUM(byte));
				return arr;
			}
		),
		0
	);
}

ScriptEngine::~ScriptEngine() noexcept
{ ruby_cleanup(0); }

ScriptEngine::ExecResult ScriptEngine::ExecFile(const std::filesystem::path &path) noexcept
{
	auto state{0};
	const char *ptr{path.c_str()};
	rb_protect(reinterpret_cast<VALUE (*)(VALUE)>(rb_load_file_str), std::bit_cast<VALUE>(&ptr), &state);
	if (state) [[unlikely]]
	{
		std::string error{};
		if (const auto err{rb_errinfo()}; !NIL_P(err)) [[likely]]
		{
			auto name{rb_funcall(rb_obj_class(err), rb_intern("name"), 0)};
			auto message{rb_funcall(rb_obj_class(err), rb_intern("message"), 0)};
			error = std::format("{}: {}", StringValueCStr(name), StringValueCStr(message));
	
			/*	
			if (auto bt{rb_funcall(err, rb_intern("backtrace"), 0)}; !NIL_P(bt) && TYPE(bt) == T_ARRAY)
			{
				const auto len{RARRAY_LEN(bt)};
				for (auto i{0l}; i < len && i < 5; ++i)
					error += std::format("at {}; ", StringValueCStr(RARRAY_AREF(bt, i)));
			}
			*/
	
			rb_set_errinfo(Qnil);
		}

		return std::unexpected{EngineError{std::format("EngineError: {}", error)}};
	}

	return std::monostate{};
}

ScriptEngine::ExecResult ScriptEngine::ExecString(std::string_view str) noexcept
{
	auto state{0};
	rb_protect(reinterpret_cast<VALUE (*)(VALUE)>(rb_eval_string), std::bit_cast<VALUE>(str.data()), &state);
	if (state) [[unlikely]]
		return std::unexpected{EngineError{"ExecStringError"}};
	return std::monostate{};
}
