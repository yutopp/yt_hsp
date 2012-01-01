#ifndef YT_HSP_LOOP_STACK_HPP
#define YT_HSP_LOOP_STACK_HPP

#include "../segment_type.h"

#include <deque>

namespace yt_hsp
{
	namespace core
	{
		//
		typedef unsigned short		subroutine_level_type;		//
		typedef unsigned short		loop_level_type;			//

		//
		struct loop_point
		{
			typedef segment::const_cs_iterator_type address_type;

			const static std::size_t stack_max = 32;
			const static int eternal = -1;

			int time;				// loop times left
			int cnt;				// count
			int step;				// count add value
			address_type addr;		// loop start ptr
		};

		// HSPƒ‹[ƒv‚ðŠÇ—‚·‚é(thread unsafe)
		class loop_stack
		{
		public:
			loop_stack()
				: level_( 0 )
			{
				//”Ô•º
				loop_point p;
				points_.push_back( std::move( p ) );
				refresh_cache();
			}

			bool push( loop_point&& p )
			{
				if ( level_ >= loop_point::stack_max )
					return false;

				points_.push_back( std::move( p ) );
				++level_;

				refresh_cache();

				return true;
			}

			bool pop()
			{
				if ( level_ <= 0 )
					return false;

				points_.pop_back();
				--level_;

				refresh_cache();

				return true;
			}

			loop_point& get_current_point()
			{
				return *p_;
			}

		private:
			void inline refresh_cache()
			{
				p_ = &points_.back();
			}

			loop_level_type level_;				// loop nest level
			std::deque<loop_point> points_;		//

			loop_point* p_;
		};

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_LOOP_STACK_HPP*/