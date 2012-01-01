#ifndef YT_HSP_TYPE_PROFILE_HPP
#define YT_HSP_TYPE_PROFILE_HPP

#include "type_id.h"

#include <string>

#include <boost/operators.hpp>

namespace yt_hsp
{
	namespace core
	{
		namespace types
		{
			//ﾎﾜｧ!
			class support_flag
				: private boost::bitwise<support_flag, boost::shiftable<support_flag>>
			{
				typedef short		value_type;

			public:
				static value_type const storage			= 0x0001;
				static value_type const flex_storage	= 0x0002;
				static value_type const fixed_array		= 0x0004;
				static value_type const flex_array		= 0x0008;
				static value_type const array_obj		= 0x0010;
				static value_type const flex_size		= 0x0020;
				static value_type const no_convert		= 0x0040;
				static value_type const varuse			= 0x0080;
				static value_type const temp_var		= 0x0100;
				static value_type const user_1			= 0x1000;
				static value_type const user_2			= 0x2000;

				static value_type const misc			= array_obj;

				support_flag()
					: value_( storage )
				{};
				support_flag( value_type v )
					: value_( v )
				{};
				
				bool operator==( const support_flag& v ) const { return value_ == v.value_; }
				support_flag& operator=( value_type v ) { value_ = v; }
				support_flag& operator|=( const support_flag& v ) { value_ |=  v.value_; return *this; }
				support_flag& operator&=( const support_flag& v ) { value_ &=  v.value_; return *this; }
				support_flag& operator^=( const support_flag& v ) { value_ ^=  v.value_; return *this; }
				support_flag& operator<<( const support_flag& v ) { value_ <<= v.value_; return *this; }
				support_flag& operator>>( const support_flag& v ) { value_ >>= v.value_; return *this; }

			private:
				value_type value_;
			};


			//型プロファイル
			class type_profile
			{
			public:
				type_profile() {}
				type_profile(
					BOOST_SCOPED_ENUM(id::plain) id,
					support_flag support,
					const std::string& name,
					int version,
					std::size_t size
					)
					: typeid_( id )
					, support_( support )
					, typename_( name )
					, version_( version )
					, size_( size )
				{}

				inline BOOST_SCOPED_ENUM(id::plain) get_typeid() const
				{
					return typeid_;
				}

			private:
				BOOST_SCOPED_ENUM(id::plain) typeid_;		//
				support_flag support_;			//
				std::string typename_;			//
				int version_;					//

				//support_ に flex_storage を指定した場合は無効
				std::size_t size_;				//
			};

		} // namespace types
	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_TYPE_PROFILE_HPP*/
