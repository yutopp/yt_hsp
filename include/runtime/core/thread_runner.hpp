#ifndef YT_HSP_THREAD_RUNNER
#define YT_HSP_THREAD_RUNNER

#include "thread_context.hpp"
#include "code_processor.hpp"

#include <iostream>

namespace yt_hsp
{
	namespace core
	{
		//コードの実行クラス
		template<typename Executor>
		class thread_runner
		{
//			friend Executer;

		public:
			thread_runner( thread_context&& ctx, segment::const_cs_iterator_type address/*, Functor callback */)
				: processor_( std::move( ctx ), address )
			{}

			// forward to 1 step.
			inline bool operator()() throw()
			{
				try {
					for(;;) {
						Executor::entry( processor_ );	//ifが必要かも...
						if ( processor_.get_runmode() == run_mode::ret ) {
							processor_.pop_subroutine_stack();
						}
					}
				}
				catch(...) {
					std::cout << "runtime error occured!" << std::endl;

					{
						int i;
						std::cin >> i;
					}
				}
				return false;	//bad
			}

		private:
			code_processor processor_;
			Executor executor_;
		};

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_THREAD_RUNNER*/