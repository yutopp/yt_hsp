#ifndef YT_HSP_ERROR_HPP
#define YT_HSP_ERROR_HPP

#include <boost/detail/scoped_enum_emulation.hpp>

namespace yt_hsp
{
	namespace core
	{
		class error
		{
		public:
			enum code {
				none,					// スクリプト終了時
				unknown_code,
				syntax,
				illegal_function,
				wrong_expression,
				no_default,
				type_mismatch,
				array_overflow,
				label_required,
				too_many_nest,
				return_without_gosub,
				loop_without_repeat,
				file_io,
				picture_missing,
				external_execute,
				priority,
				too_many_parameters,
				temp_buffer_overflow,
				wrong_name,
				divided_by_zero,
				buffer_overflow,
				unsupported_function,
				expression_complex,
				variable_required,
				integer_required,
				bad_array_expression,
				out_of_memory,
				type_initalization_failed,
				no_function_parameters,
				stack_overflow,
				invalid_parameter,
				invalid_arraystore,
				invalid_funcparam,
				window_object_full,
				invalid_array,
				struct_required,
				invalid_struct_source,
				invalid_type,
				dll_error,
				comdll_error,
				noretval,
				function_syntax,

				intjump,					// 割り込みジャンプ時
				exitrun						// 外部ファイル実行
			};

			error()
				: value_( none )
			{}

			error( code c )
				: value_( c )
			{}

			error& operator=( code c )
			{
				value_ = c;
			}

		private:
			code value_;
		};
	}
}

#endif /*YT_HSP_ERROR_HPP*/