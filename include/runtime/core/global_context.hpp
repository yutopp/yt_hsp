#ifndef YT_HSP_GLOBAL_CONTEXT_HPP
#define YT_HSP_GLOBAL_CONTEXT_HPP

#include "hsp_header.h"
#include "segment_type.h"
#include "context_message_passenger.h"
#include "thread_runner.hpp"

#include <vector>
#include <stdexcept>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace yt_hsp
{
	namespace core
	{
		//HSPのグローバルなコンテキスト
		class global_context
		{
			friend context_message_passenger;

			typedef segment::binary_type			binary_type;
			typedef std::vector<binary_type>		segment_type;

		public:
			template<class Iterator>
			global_context( Iterator begin, Iterator end )
			{
				//バイナリをコピー
				std::copy( begin, end, std::back_inserter( binary_ ) );

				//ヘッダチェック
				if ( binary_.size() < sizeof( hsp_header ) )
					throw std::runtime_error( "binary is too short." );

				//
				hsp3header_ = static_cast<hsp_header*>( (void*)&binary_[0] );

				//コードセグメントチェック
				if ( binary_.size() < hsp3header_->cs_ptr + hsp3header_->cs_size )
					throw std::runtime_error( "invalid code segment." );

				if ( binary_.size() < hsp3header_->ds_ptr + hsp3header_->ds_size )
					throw std::runtime_error( "invalid data segment." );

				if ( binary_.size() < hsp3header_->ot_ptr + hsp3header_->ot_size )
					throw std::runtime_error( "invalid object temp segment." );

				//hsp3header_->var_count
			}

			inline hsp_header const* get_header() const
			{
				return hsp3header_;
			}

			inline segment::const_cs_iterator_type get_entry_cs_ptr() const
			{
				return reinterpret_cast<segment::const_cs_iterator_type>(
					get_binary_offset() + hsp3header_->cs_ptr
					);
			}

			inline binary_type const* get_binary_offset() const
			{
				return &binary_[0];
			}

			template<typename Executer>
			boost::shared_ptr<thread_runner<Executer>> create_master_thread()
			{
				return boost::make_shared<thread_runner<Executer>>(
					thread_context(
						context_message_passenger( *this ),
						get_header(),
						get_binary_offset()
						),
					get_entry_cs_ptr()
					);
			}

/*			variable::manager& get_globalvar()
			{
				return global_var_;
			}
			const variable::manager& get_globalvar() const
			{
				return global_var_;
			}*/
			//  HSP Context
			//

		private:
			segment_type binary_;				// Code binary
			hsp_header const* hsp3header_;		// HSP object file header

/*			variable::manager global_var_;

			thread_manager threads_;*/

		public:
	//		PVal *mem_var;                      // var storage index
	//		HSPEXINFO30 exinfo;                 // HSP function data(3.0)
	//		LOOPDAT mem_loop[HSP3_REPEAT_MAX];  // repeat loop info
	//		int looplev;                        // repeat loop level
	//		HSPERROR err;                       // error code
//			int hspstat;                        // HSP status
//			char *fnbuffer;                     // buffer for FILENAME
//			void *instance;                     // Instance Handle (windows)
//			int intwnd_id;                      // Window ID (interrupt)
	//		PVal *note_pval;                    // MemNote pval
	//		APTR note_aptr;                     // MemNote aptr
	//		PVal *notep_pval;                   // MemNote pval (previous)
	//		APTR notep_aptr;                    // MemNote aptr (previous)
	//		char *stmp;                         // String temporary buffer
	
	//		LIBDAT *mem_linfo;                  // Library info
	//		STRUCTPRM *mem_minfo;               // Parameter info
	//		STRUCTDAT *mem_finfo;               // Function/Struct info
//			int retval_level;                   // subroutine level (return code)
//			int endcode;                        // End result code
	//		void (*msgfunc) (HSPCTX *);         // Message Callback Proc.
//			void *wnd_parent;                   // Parent Window Handle
//			char *cmdline;                      // Command Line Parameters
	
	//		HSPEXINFO *exinfo2;                 // HSP function data(3.1)
	
	//		int prmstack_max;                   // Parameter Stack Max(hsp3cnv) (3.3)
		};

	} // namespace core
} // namespace yt_hsp


#endif /*YT_HSP_GLOBAL_CONTEXT_HPP*/