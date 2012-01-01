#ifndef YT_HSP_FUNCTION_TABLE_HPP
#define YT_HSP_FUNCTION_TABLE_HPP

#include <cstddef>
#include <array>

namespace yt_hsp
{
	namespace core
	{
		namespace detail
		{
			//
			template<typename FuncT, std::size_t N>
			class function_table
			{
				typedef FuncT			function_type;
				static const int size = N;

				typedef std::array<function_type, size> table_type;

			public:
				void static initialize( const function_type& f )
				{
					table_.fill( f );
				}

				void static set_function( std::size_t index, const function_type& f )
				{
					table_[index] = f;
				}

				template<typename Arg1>
				auto static inline call( std::size_t index, Arg1&& arg ) -> decltype( (*table_[index])( std::forward<Arg1>( arg ) ) )
				{
					return (*table_[index])( std::forward<Arg1>( arg ) );
				}

			private:
				static table_type table_;
			};
			template<typename T, std::size_t N>
			typename function_table<T, N>::table_type function_table<T, N>::table_;

		} // namespace detail
	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_FUNCTION_TABLE_HPP*/