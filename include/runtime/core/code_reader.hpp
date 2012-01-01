#ifndef YT_HSP_CODE_READER_HPP
#define YT_HSP_CODE_READER_HPP

#include "segment_type.h"

#include <algorithm>

namespace yt_hsp
{
	namespace core
	{
		struct code_value
		{
			bool is_48bit;
			bool ex2_flag, ex1_flag, ex0_flag;
			short type;
			int data;
		};

		// バイナリ読取(thread:unsafe)
		class code_reader
		{
		public:
			typedef segment::cs_type					cs_type;
			typedef segment::const_cs_iterator_type		const_iterator_type;

			code_reader( const_iterator_type address )
				: pc_( address )
			{}

			//プログラムカウンタを進める.
			inline void forward_pc() throw()
			{
				//std::advance( pc_, is_current_op_48bit() ? 3/*48bit*/ : 2/*32bit*/ );
				std::advance( pc_, 2 + is_current_op_48bit() );
			}

			//プログラムカウンタを進める.
			inline void forward_pc( const code_value& value ) throw()
			{
				std::advance( pc_, 2 + value.is_48bit );
			}

			//拡張コードを読み進める
			inline cs_type read_mono_code() throw()
			{
				return *pc_++;
			}

			//ジャンプ
			inline void jump( const_iterator_type address ) throw()
			{
				pc_ = address;
			}

			//
			inline const_iterator_type current_pc() const throw()
			{
				return pc_;
			}

			//プログラムカウンタの指すデータを分解.
			code_value current_code() const throw()
			{
				code_value value = {
					is_current_op_48bit(),
					current_ex2_flag(),
					current_ex1_flag(),
					current_ex0_flag(),
					current_type(),
					current_data()
				};

				return std::move( value );
			}

			inline bool is_current_op_48bit() const throw()
			{
				return ( *pc_ & 0x8000 ) != 0;
			}

			inline bool current_ex2_flag() const throw()
			{
				return ( *pc_ & 0x4000 ) != 0;
			}

			inline bool current_ex1_flag() const throw()
			{
				return ( *pc_ & 0x2000 ) != 0;
			}

			inline bool current_ex0_flag() const throw()
			{
				return ( *pc_ & 0x1000 ) != 0;
			}

			inline bool current_ex_flag() const throw()
			{
				return current_ex1_flag() | current_ex2_flag();
			}

			inline short current_type() const throw()
			{
				return ( *pc_ & 0x0fff );
			}

			inline int current_data() const throw()
			{
				//return is_current_op_48bit() ? *reinterpret_cast<const int*>( pc_ + 1 ) : *( pc_ + 1 );
				return *reinterpret_cast<const int*>( pc_ + 1 ) & ( ( 0xffffffff * is_current_op_48bit() ) | 0x0000ffff );
			}

		private:
			const_iterator_type pc_;	//プログラムカウンタ
		};
	}
}

#endif /*YT_HSP_CODE_READER_HPP*/