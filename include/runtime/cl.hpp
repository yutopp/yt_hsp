#ifndef YT_HSP_CL_HPP
#define YT_HSP_CL_HPP

#include "core/interpreter.hpp"
#include "core/command_executor.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <memory>

#include <stdexcept>

namespace yt_hsp
{
	namespace cl
	{
		//
		template<typename Executor = core::function_executor>
		class interpreter
			: public core::interpreter<interpreter<Executor>, Executor>
		{
		public:
			template<class Iterator>
			interpreter( Iterator begin, Iterator end )
				: base_type( begin, end )
			{}
		};
	}
}

#endif	//YT_HSP_CL_HPP