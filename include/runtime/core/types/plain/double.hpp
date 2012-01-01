#ifndef YT_HSP_VARIABLE_DOUBLE_HPP
#define YT_HSP_VARIABLE_DOUBLE_HPP

#include "basic_type.hpp"

namespace yt_hsp
{
	namespace core
	{
		namespace types
		{
			//YtHSPÇÃé¿êîÉNÉâÉX
			class double_t
				: public basic_type
			{
				static type_profile const double_prof;

			public:
				double_t();
				double_t( const double& );

				template<typename T>
				T convert_to() const;

				basic_pointer_type convert_to_by_id( BOOST_SCOPED_ENUM(id::plain) ) const;

				static inline type_profile const& profile()
				{
					return double_prof;
				}

			private:
				const type_profile& get_profile() const;

				void* get_pointer();
				const void* const_get_pointer() const;

				double data_;
			};

		} // namespace types
	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_VARIABLE_DOUBLE_HPP*/
