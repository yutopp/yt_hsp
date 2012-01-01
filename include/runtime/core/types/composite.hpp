#ifndef YT_HSP_COMPOSITE_TYPE_HPP
#define YT_HSP_COMPOSITE_TYPE_HPP

#include "plain/int.hpp"
#include "plain/double.hpp"
#include "plain/string.hpp"

#include <boost/type_traits/is_pointer.hpp>

namespace yt_hsp
{
	namespace core
	{
		namespace types
		{
			class composite
			{
			public:
				// -- default
				composite()
					: is_constructed_( false )
					, type_id_( id::plain::invalid_type )
				{}

				// -- move
				composite( composite&& rhs )
					: is_constructed_( true )
					, type_id_( rhs.type_id_ )
					, int_value_( std::move( rhs.int_value_ ) )
					, double_value_( std::move( rhs.double_value_ ) )
					, string_value_( std::move( rhs.string_value_ ) )
				{}

				composite& operator=( composite&& rhs )
				{
					if ( this != &rhs ) {
						is_constructed_ = rhs.is_constructed_;
						type_id_ = rhs.type_id_;
						int_value_ = std::move( rhs.int_value_ );
						double_value_ = std::move( rhs.double_value_ );
						string_value_ = std::move( rhs.string_value_ );
					}

					return *this;
				}

				// -- user defined
				explicit composite( int_t&& value )
				{
					assign( std::move( value ) );
				}
				composite& operator=( int_t&& rhs )
				{
					assign( std::move( rhs ) );
					return *this;
				}

				explicit composite( double_t&& value )
				{
					assign( std::move( value ) );
				}
				composite& operator=( double_t&& rhs )
				{
					assign( std::move( rhs ) );
					return *this;
				}

				explicit composite( string_t&& value )
				{
					assign( std::move( value ) );
				}
				composite& operator=( string_t&& rhs )
				{
					assign( std::move( rhs ) );
					return *this;
				}

				// -- operators
				void add_assign( const composite& rhs )
				{
					if ( type_id_ != rhs.get_typeid() )
						throw "";

					//
					switch( type_id_ )
					{
					/*case id::plain::int_type:
						int_value_.add_assign()

					case id::plain::double_type:
						return convert_to_impl( id, double_value_ );*/
				
					case id::plain::string_type:
						string_value_.add_assign( rhs.string_value_ );
					}
				}


				// -- 
				void assign( int_t&& value )
				{
					set_id( value );
					int_value_ = std::move( value );

					is_constructed_ = true;
				}

				void assign( double_t&& value )
				{
					set_id( value );
					double_value_ = std::move( value );

					is_constructed_ = true;
				}
			
				void assign( string_t&& value )
				{
					set_id( value );
					string_value_ = std::move( value );

					is_constructed_ = true;
				}

				bool convert_to( BOOST_SCOPED_ENUM(id::plain) id )
				{
					//
					switch( type_id_ )
					{
					case id::plain::int_type:
						return convert_to_impl( id, int_value_ );

					case id::plain::double_type:
						return convert_to_impl( id, double_value_ );
				
					case id::plain::string_type:
						return convert_to_impl( id, string_value_ );
					}

					return false;
				}

				//
				inline BOOST_SCOPED_ENUM(id::plain) get_typeid() const
				{
					return type_id_;
				}

				//
				template<class T>
				inline T& get( typename boost::disable_if<boost::is_pointer<T>>::type* =0 )
				{
					return *get_basic_ptr()->get<T*>();
				}
				template<class T>
				inline T get( typename boost::enable_if<boost::is_pointer<T>>::type* =0 )
				{
					return get_basic_ptr()->get<T>();
				}
				//
				template<class T>
				inline const T& get( typename boost::disable_if<boost::is_pointer<T>>::type* =0 ) const
				{
					return *get_basic_ptr()->const_get<const T*>();
				}
				template<class T>
				inline const T get( typename boost::enable_if<boost::is_pointer<T>>::type* =0 ) const
				{
					return get_basic_ptr()->const_get<const T>();
				}

				//
				operator bool() const
				{
					return is_constructed_;
				}

			private:
				composite( const composite& );
				composite& operator=( const composite& );

				template<typename T>
				void set_id( const T& v )
				{
					type_id_ = T::profile().get_typeid();
				}

				template<typename T>
				bool convert_to_impl( BOOST_SCOPED_ENUM(id::plain) convert_to_id, T& val )
				{
					//
					switch( convert_to_id )
					{
					case id::plain::int_type:
						assign( std::move( val.convert_to<int_t>() ) );
						break;

					case id::plain::double_type:
						assign( std::move( val.convert_to<double_t>() ) );
						break;
				
					case id::plain::string_type:
						assign( std::move( val.convert_to<string_t>() ) );
						break;

					default:
						return false;
					}

					return true;
				}

				//
				basic_type* get_basic_ptr()
				{
					//
					switch( type_id_ )
					{
					case id::plain::int_type:
						return &int_value_;

					case id::plain::double_type:
						return &double_value_;
				
					case id::plain::string_type:
						return &string_value_;
					}

					return nullptr;
				}

				const basic_type* get_basic_ptr() const
				{
					//
					switch( type_id_ )
					{
					case id::plain::int_type:
						return &int_value_;

					case id::plain::double_type:
						return &double_value_;
				
					case id::plain::string_type:
						return &string_value_;
					}

					return nullptr;
				}

				bool is_constructed_;
				BOOST_SCOPED_ENUM(id::plain) type_id_;

				int_t		int_value_;
				double_t	double_value_;
				string_t	string_value_;
			};

		} // namespace types
	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_COMPOSITE_TYPE_HPP*/