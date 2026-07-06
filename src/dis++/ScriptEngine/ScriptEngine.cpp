module;

#include <ruby.h>

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
			[](const void *) -> std::size_t { return sizeof(disxx::loader::macho::Loader *); }
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
			[](const void *) -> std::size_t { return sizeof(disxx::loader::macho::ExecutableFile *); }
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
			[](const void *) -> std::size_t { return sizeof(disxx::loader::executable::Section *); }
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
			[](const void *) -> std::size_t { return sizeof(disxx::loader::executable::Section *); }
		},
		nullptr,
		nullptr,
		RUBY_TYPED_FREE_IMMEDIATELY
	};

	template <typename T>
	inline T *unwrap(VALUE)
	{ return nullptr; }

	template <>
	inline disxx::loader::macho::Loader unwrap(VALUE type) noexcept
	{
		disxx::loader::macho::Loader *ptr{nullptr};
		TypedData_Get_Struct(val, disxx::loader::macho::Loader, &loader, ptr);
		return ptr;
	}

	template <>
	inline disxx::loader::executable::ExecutableFile unwrap(VALUE type) noexcept
	{
		disxx::loader::executable::ExecutableFile *ptr{nullptr};
		TypedData_Get_Struct(val, disxx::loader::executable::Section, &executable, ptr);
		return ptr;
	}

	template <>
	inline disxx::loader::executable::Section unwrap(VALUE type) noexcept
	{
		disxx::loader::executable::Section *ptr{nullptr};
		TypedData_Get_Struct(val, disxx::loader::executable::Section, &section, ptr);
		return ptr;
	}

	template <>
	inline disxx::loader::executable::Label unwrap(VALUE type) noexcept
	{
		disxx::loader::executable::Label *ptr{nullptr};
		TypedData_Get_Struct(val, disxx::loader::executable::Section, &label, ptr);
		return ptr;
	}
} /* */

ScriptEngine::ScriptEngine(void) noexcept
	: m_Disxx{Qnil}
	, m_Loader{Qnil}
	, m_ExecutableFile{Qnil}
	, m_Section{Qnil}
	, m_Label{Qnil}
	, m_Disassembler{Qnil}
{
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
			[](VALUE self, VALUE path) -> VALUE
			{
				const std::filesystem::path &unwrapped{rb_string_value_cstr(path)};
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
			const auto data{unwrap<disxx::loader::macho::Loader>(self)->LoadData(unwrapped)};
			return TypedData_Wrap_Struct(this->m_Loader, &loader, &loader);
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
		this->m_Executable,
		"getSections",
		RUBY_METHOD_FUNC
		(
			[](VALUE self) -> VALUE
			{
				auto arr{rb_ary_new()};
				for (const auto sect : unwrap<disxx::loader::executable::ExecutableFile>(self)->GetSections())
				{
					const auto rubyfied{TypedData_Wrap_Struct(this->m_Section, &section, &sect)};
					rb_ary_push(arr, rubyfied);
				}

				return arr;
			}
		),
		0
	);
	rb_define_method
	(
		this->m_Executable,
		"getMagic",
		RUBY_METHOD_FUNC
		(
			[](VALUE self) -> VALUE
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
			[](VALUE self) -> VALUE
			{
				return rb_str_new_cstr
				(
					unwrap<disxx::loader::executable::Section>(self)
						->GetName()
						.c_str()
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
			[](VALUE self) -> VALUE
			{
				auto arr{rb_ary_new()};
				for (const auto lbl : *unwrap<disxx::loader::executable::Section>(self))
				{
					const auto rubyfied{TypedData_Wrap_Struct(this->m_Label, &label, &lbl)};
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
			[](VALUE self) -> VALUE
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
			[](VALUE self) -> VALUE
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
			[](VALUE self) -> VALUE
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

	this->m_Label = rb_define_under
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
			[](VALUE self) -> VALUE
			{
				return rb_str_new_cstr
				(
					unwrap<disxx::loader::executable::Label>(self)
						->GetName()
						.c_str()
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
			[](VALUE self) -> VALUE
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
			[](VALUE self) -> VALUE
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
			[](VALUE self) -> VALUE
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
	rb_define_mathod
	(
		this->m_Label,
		"data",
		RUBY_METHOD_FUNC
		(
			[](VALUE self) -> VALUE
			{
				auto arr{rb_ary_new()};
				for (const auto byte : label.GetData<std::uint8_t>())
					rb_ary_push(arr, USHORT2NUM(byte));
				return arr;
			}
		),
		0
	);
}
