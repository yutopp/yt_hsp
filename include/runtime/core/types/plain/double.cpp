#ifndef YT_HSP_VARIABLE_DOUBLE_CPP
#define YT_HSP_VARIABLE_DOUBLE_CPP

#include "double.hpp"

#include "int.hpp"
#include "string.hpp"

namespace yt_hsp
{
	namespace core
	{
		namespace types
		{
			//profile
			type_profile const double_t::double_prof =
				type_profile(
					id::plain::double_type,
					support_flag::storage,
					"double",
					0x001,
					8
					);

			//
			double_t::double_t()
				: data_( 0.0 )
			{}

			double_t::double_t( const double& d )
				: data_( d )
			{}

			template<>
			int_t double_t::convert_to<int_t>() const
			{
				return int_t( static_cast<int>( data_ ) );
			}

			template<>
			double_t double_t::convert_to<double_t>() const
			{
				return double_t( data_ );
			}

			template<>
			string_t double_t::convert_to<string_t>() const
			{
				return string_t( boost::lexical_cast<std::string>( data_ ) );
			}

			double_t::basic_pointer_type double_t::convert_to_by_id( BOOST_SCOPED_ENUM(id::plain) id ) const
			{
				switch( id )
				{
				case id::plain::int_type:
					return basic_pointer_type( new int_t( static_cast<int>( data_ ) ) );

				case id::plain::string_type:
					return basic_pointer_type( new string_t( boost::lexical_cast<std::string>( data_ ) ) );
				}

				return nullptr;
			}

			const type_profile& double_t::get_profile() const
			{
				return double_prof;
			}

			//
			void* double_t::get_pointer()
			{
				return &data_;
			}

			const void* double_t::const_get_pointer() const
			{
				return &data_;
			}

		} // namespace types
	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_VARIABLE_DOUBLE_CPP*/
