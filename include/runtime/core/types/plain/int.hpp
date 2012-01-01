#ifndef YT_HSP_VARIABLE_INT_HPP
#define YT_HSP_VARIABLE_INT_HPP

#include "basic_type.hpp"

namespace yt_hsp
{
	namespace core
	{
		namespace types
		{
			//YtHSPÇÃêÆêîÉNÉâÉX
			class int_t
				: public basic_type
			{
				static type_profile const int_prof;

			public:
				int_t();
				explicit int_t( int );

				int_t( int_t&& rhs )
					: basic_type( std::move( rhs ) )
					, data_( rhs.data_ )
				{}
				int_t& operator=( int_t&& rhs )
				{
					data_ = rhs.data_;
					return *this;
				}

				template<typename T>
				T convert_to() const;

				basic_pointer_type convert_to_by_id( BOOST_SCOPED_ENUM(id::plain) ) const;

				static inline type_profile const& profile()
				{
					return int_prof;
				}

			private:
				const type_profile& get_profile() const;

				void* get_pointer();
				const void* const_get_pointer() const;

				int data_;
			};

/*			template<>
			double_t&& int_t::convert_to<double_t>() const;
			template<>
			string_t&& int_t::convert_to<string_t>() const;*/

		} // namespace types
	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_VARIABLE_INT_HPP*/
