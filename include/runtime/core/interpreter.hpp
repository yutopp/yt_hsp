#ifndef YT_HSP_INTERPRETER_HPP
#define YT_HSP_INTERPRETER_HPP

#include "global_context.hpp"

#include <iostream>	//
#include <fstream>
#include <string>
#include <iterator>
#include <memory>
#include <stdexcept>

#include <boost/shared_ptr.hpp>

namespace yt_hsp
{
	namespace core
	{
		//
		template<typename Derived, typename Executor>
		class interpreter
		{
			typedef interpreter<Derived, Executor>	base_type;
			typedef Executor						executor_type;

		public:
			template<class Iterator>
			interpreter( Iterator begin, Iterator end )
				: ctx_( begin, end )
			{
				{
					hsp_header const* const h = ctx_.get_header();
					std::cout
						<< "- hsp3_header ----------" << std::endl
						<< "magic_code : " << std::string( h->magic_code, 4 ) << std::endl
						<< "version    : " << std::hex << h->version << std::dec << std::endl
						<< "maxval     : " << h->var_count << std::endl
						<< "" << std::endl
						<< "" << std::endl
						<< "" << std::endl
						<< "------------------------" << std::endl;
				}

				//
				master_thread_ = ctx_.create_master_thread<executor_type>();
			}

			//‘–‚ç‚¹Žn‚ß‚é
			bool run()
			{
				return (*master_thread_)();
			};

		private:
			global_context ctx_;
			boost::shared_ptr<thread_runner<executor_type>> master_thread_;		
		};
	}

	template<typename Interpreter>
	Interpreter make_interpreter( const std::string& file_name )
	{
		std::ifstream fs( file_name, std::ios::binary );
		if ( !fs )
			throw std::runtime_error( "Couldn't read an ax file." );

		const std::istreambuf_iterator<char> begin = fs, end;
		return Interpreter( begin, end );
	}
}

#endif /*YT_HSP_INTERPRETER_HPP*/