#ifndef YT_HSP_THREAD_CONTEXT
#define YT_HSP_THREAD_CONTEXT

#include "segment_type.h"
#include "hsp_header.h"
#include "hsp_structs.hpp"
#include "context_message_passenger.h"
#include "code_definitions.hpp"

namespace yt_hsp
{
	namespace core
	{
		//スレッドごとのコンテキスト
		class thread_context
		{
		public:
			thread_context( const context_message_passenger& psg, hsp_header const* header, segment::binary_type const* offset )
				: passenger_( psg )
				, hsp3header( header )
				, cs( reinterpret_cast<segment::const_cs_iterator_type>( offset + hsp3header->cs_ptr ) )
				, ds( reinterpret_cast<segment::const_ds_iterator_type>( offset + hsp3header->ds_ptr ) )
				, di( reinterpret_cast<segment::const_di_iterator_type>( offset + hsp3header->dinfo_ptr ) )
				, ot( reinterpret_cast<segment::const_ot_iterator_type>( offset + hsp3header->ot_ptr ) )
				, execute_mode( run_mode::run )
			{}

			thread_context( thread_context&& rhs )
				: passenger_( rhs.passenger_ )
				, hsp3header( rhs.hsp3header )
				, cs( rhs.cs )
				, ds( rhs.ds )
				, di( rhs.di )
				, ot( rhs.ot )
				, calls( std::move( rhs.calls ) )
				, stack( std::move( rhs.stack ) )
				, loops( std::move( rhs.loops ) )
				, irq( std::move( rhs.irq ) )
				, wait( std::move( rhs.wait ) )
				, sysvar( std::move( rhs.sysvar ) )
				, execute_mode( rhs.execute_mode )
			{}

		//private:
			// HSP runner context
			context_message_passenger passenger_;		//

			hsp_header const* hsp3header;				// HSP object file header
			segment::const_cs_iterator_type cs;			// Code segment ptr
			segment::const_ds_iterator_type ds;			// Data segment ptr
			segment::const_di_iterator_type di;			// Debug info ptr
			segment::const_ot_iterator_type ot;			// Object temp segment ptr

			subroutine_level_type	subroutine_level;	// subroutine level
			system_stack			calls;				//
			value_stack<128>		stack;				//
			loop_stack				loops;				//

			irq_info				irq;
			wait_info				wait;
			sysvar_info				sysvar;

			BOOST_SCOPED_ENUM(run_mode) execute_mode;	// HSP execute mode

		private:
			thread_context( const thread_context& );
			thread_context& operator=( const thread_context& );
		};

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_THREAD_CONTEXT*/