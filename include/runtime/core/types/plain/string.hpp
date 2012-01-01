#ifndef YT_HSP_VARIABLE_STRING_HPP
#define YT_HSP_VARIABLE_STRING_HPP

#include "basic_type.hpp"

#include <string>
#include <memory>


#include <iostream>

namespace yt_hsp
{
	namespace core
	{
		namespace types
		{
			//YtHSPの文字列クラス
			class string_t
				: public basic_type
			{
				static type_profile const string_prof;

			public:
				// -- default
				string_t()
					: is_reference_( true )
					, ptr_to_ds_( nullptr )
					, data_( nullptr )
				{}

/*				// -- copy
				string_t( const string_t& rhs )
					: is_reference_( rhs.is_reference_ )
					, ptr_to_ds_( rhs.ptr_to_ds_ )
					, data_( is_reference_ ? nullptr : new std::string( *rhs.data_ ) )
				{
					std::cout << "copy ctor!" << std::endl;
				}*/

				// -- move
				string_t( string_t&& rhs )
					: is_reference_( rhs.is_reference_ )
					, ptr_to_ds_( rhs.ptr_to_ds_ )
					, data_( std::move( rhs.data_ ) )
				{}

				string_t& operator=( string_t&& rhs )
				{
					if ( this != &rhs ) {
						is_reference_ = rhs.is_reference_;
						ptr_to_ds_ = rhs.ptr_to_ds_;
						data_ = std::move( rhs.data_ );
					}

					return *this;
				}

				// -- user defined
				explicit string_t( std::size_t size );
				explicit string_t( const std::string& data );
				string_t( const char* src, bool do_copy = true );

				// -- convert
				template<typename T>
				T convert_to() const;

				basic_pointer_type convert_to_by_id( BOOST_SCOPED_ENUM(id::plain) ) const;

				// -- operators
				// +=
				void add_assign( const string_t& rhs )	 
				{
					substantiate();
					*data_ += rhs.const_get<const char*>();
				}

				static inline type_profile const& profile()
				{
					return string_prof;
				}

			private:
				string_t( const string_t& rhs );
				string_t& operator=( const string_t& rhs );

				const type_profile& get_profile() const;

				void* get_pointer();
				const void* const_get_pointer() const;

				void substantiate();	//ポインタから実体化

				bool is_reference_;
				const char* ptr_to_ds_;
				std::unique_ptr<std::string> data_;
			};

		} // namespace types
	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_VARIABLE_STRING_HPP*/
