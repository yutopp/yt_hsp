#ifndef YT_HSP_VARIABLE_TYPE_BASIC_HPP
#define YT_HSP_VARIABLE_TYPE_BASIC_HPP

#include "../type_id.h"
#include "../type_profile.hpp"

#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/utility/enable_if.hpp>

namespace yt_hsp
{
	namespace core
	{
		namespace types
		{
			//
			class basic_type
			{
			public:
				typedef std::unique_ptr<basic_type>		basic_pointer_type;

				//virtual basic_type hh(){}
				inline BOOST_SCOPED_ENUM(id::plain) get_typeid() const
				{
					return get_profile().get_typeid();
				}

				//�^�̓����f�[�^���L���X�g���Ď擾���܂��B
				template<class T>
				inline T get()
				{
					return static_cast<T>( get_pointer() );
				}
				template<class T>
				inline const T const_get() const
				{
					return static_cast<const T>( const_get_pointer() );
				}

				//�Ⴄ�^�ɃL���X�g���܂��B�����^��ϊ��ł��Ȃ��^���w�肵���ꍇ��nullptr���Ԃ���܂��B
				virtual basic_pointer_type convert_to_by_id( BOOST_SCOPED_ENUM(id::plain) ) const =0;

			private:
				virtual const type_profile& get_profile() const =0;

				virtual void* get_pointer() =0;						//���킟�������I�I�I
				virtual const void* const_get_pointer() const =0;	//���킟�������I�I�I
			};

		} // namespace types
	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_VARIABLE_TYPE_BASIC_HPP*/
