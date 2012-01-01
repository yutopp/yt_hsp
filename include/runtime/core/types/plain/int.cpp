#ifndef YT_HSP_VARIABLE_INT_CPP
#define YT_HSP_VARIABLE_INT_CPP

#include "int.hpp"

#include "double.hpp"
#include "string.hpp"

namespace yt_hsp
{
	namespace core
	{
		namespace types
		{
			//profile
			type_profile const int_t::int_prof =
				type_profile(
					id::plain::int_type,
					support_flag::storage,
					"int",
					0x001,
					4
					);

			//
			int_t::int_t()
				: data_( 0 )
			{}

			int_t::int_t( int n )
				: data_( n )
			{}

			template<>
			int_t int_t::convert_to<int_t>() const
			{
				return int_t( data_ );
			}

			template<>
			double_t int_t::convert_to<double_t>() const
			{
				return double_t( static_cast<double>( data_ ) );
			}

			template<>
			string_t int_t::convert_to<string_t>() const
			{
				return string_t( boost::lexical_cast<std::string>( data_ ) );
			}

			int_t::basic_pointer_type int_t::convert_to_by_id( BOOST_SCOPED_ENUM(id::plain) id ) const
			{
				switch( id )
				{
				case id::plain::double_type:
					return basic_pointer_type( new double_t( static_cast<double>( data_ ) ) );

				case id::plain::string_type:
					return basic_pointer_type( new string_t( boost::lexical_cast<std::string>( data_ ) ) );
				}

				return nullptr;
			}

			const type_profile& int_t::get_profile() const
			{
				return int_prof;
			}

			//
			void* int_t::get_pointer()
			{
				return &data_;
			}

			const void* int_t::const_get_pointer() const
			{
				return &data_;
			}

		} // namespace types
	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_VARIABLE_INT_CPP*/
