#ifndef YT_HSP_SYSTEM_STACK_HPP
#define YT_HSP_SYSTEM_STACK_HPP

#include "../segment_type.h"

#include <stack>

namespace yt_hsp
{
	namespace core
	{
		//
		struct call_point
		{
			label_address_type return_addr;		//
			subroutine_level_type stack_level;	//
			bool is_functon_call;				//
		};

		//システムスタック
		typedef std::stack<call_point>	system_stack;

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_SYSTEM_STACK_HPP*/