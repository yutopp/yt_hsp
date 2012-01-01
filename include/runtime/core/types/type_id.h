#ifndef YT_HSP_VARIABLE_TYPE_ID_H
#define YT_HSP_VARIABLE_TYPE_ID_H

#include <boost/detail/scoped_enum_emulation.hpp>

namespace yt_hsp
{
	namespace core
	{
		namespace types
		{
			//ïœêîå^
			namespace id
			{
				//
				BOOST_SCOPED_ENUM_START( system ) {
					invalid			= -2,
					system_val		= -1
//					mixin			= -4
				}; BOOST_SCOPED_ENUM_END

				//ÉäÉeÉâÉã
				BOOST_SCOPED_ENUM_START( literal ) {
					int_type		= 100,
					double_type		= 101,
					string_type		= 102
				}; BOOST_SCOPED_ENUM_END
				
				//
				BOOST_SCOPED_ENUM_START( plain ) {
					invalid_type	= -2,
					system_val_type	= -1,
					
					label_type		= 1,
					string_type		= 2,
					double_type		= 3,
					int_type		= 4,
					struct_type		= 5,
					comstruct_type	= 6,
					userdef_type	= 8
				}; BOOST_SCOPED_ENUM_END
			} // namespace id

		} // namespace types
	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_VARIABLE_TYPE_ID_H*/
