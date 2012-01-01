#ifndef YT_HSP_VARIABLE_STRING_CPP
#define YT_HSP_VARIABLE_STRING_CPP

#include "string.hpp"

#include "int.hpp"
#include "double.hpp"

namespace yt_hsp
{
	namespace core
	{
		namespace types
		{
			//profile
			type_profile const string_t::string_prof =
				type_profile(
					id::plain::string_type,
					support_flag::flex_storage | support_flag::flex_array,
					"str",
					0x001,
					0
					);

			//
			string_t::string_t( std::size_t size )
				: is_reference_( false )
				, ptr_to_ds_( nullptr )
				, data_( new std::string( size, 0 ) )
			{}

			string_t::string_t( const std::string& data )
				: is_reference_( false )
				, ptr_to_ds_( nullptr )
				, data_( new std::string( data.c_str() ) )
			{}

			//
			string_t::string_t( const char* src, bool do_copy )
				: is_reference_( !do_copy )
				, ptr_to_ds_( do_copy ? nullptr : src )
				, data_( do_copy ? new std::string( src ) : nullptr )
			{}

			template<>
			int_t string_t::convert_to<int_t>() const
			{
				return int_t( boost::lexical_cast<int>( is_reference_ ? ptr_to_ds_ : data_->c_str() ) );
			}

			template<>
			double_t string_t::convert_to<double_t>() const
			{
				return double_t( boost::lexical_cast<double>( is_reference_ ? ptr_to_ds_ : data_->c_str() ) );
			}

			template<>
			string_t string_t::convert_to<string_t>() const
			{
				return string_t( is_reference_ ? ptr_to_ds_ : data_->c_str(), !is_reference_ );		//copy
			}

			string_t::basic_pointer_type string_t::convert_to_by_id( BOOST_SCOPED_ENUM(id::plain) id ) const
			{
				switch( id )
				{
				case id::plain::int_type:
					return basic_pointer_type(
						new int_t( boost::lexical_cast<int>( is_reference_ ? ptr_to_ds_ : data_->c_str() ) )
						);

				case id::plain::double_type:
					return basic_pointer_type(
						new double_t( boost::lexical_cast<double>( is_reference_ ? ptr_to_ds_ : data_->c_str() ) )
						);
				}

				return nullptr;
			}

			const type_profile& string_t::get_profile() const
			{
				return string_prof;
			}

			//
			void* string_t::get_pointer()
			{
				substantiate();
				return &data_->at( 0 );
			}

			const void* string_t::const_get_pointer() const
			{
				return is_reference_ ? ptr_to_ds_ : data_->c_str();
			}

			//
			inline void string_t::substantiate()
			{
				// todo: ï¿óÒèàóùÇÃé¿ëïÇ™ì¸Ç¡ÇΩÇÁÉçÉbÉNì±ì¸

				if ( is_reference_ ) {
					// COW
					data_.reset( new std::string( ptr_to_ds_ ) );
					is_reference_ = false;
					ptr_to_ds_ = nullptr;
				}
			}

		} // namespace types
	} // namespace core
} // namespace yt_hsp

#endif /*COPY_HSP_VARIABLE_STRING_CPP*/
