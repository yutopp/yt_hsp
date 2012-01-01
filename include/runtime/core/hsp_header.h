#ifndef YT_HSP_HEADER_H
#define YT_HSP_HEADER_H

#include <cstddef>

namespace yt_hsp
{
	namespace core
	{
		struct hsp_header
		{
			//      HSP3.0 header structure
			//
			char				magic_code[4];		// magic code[0,3]
			int					version;			// version number info
			std::size_t			var_count;			// max count of VAL Object
			std::size_t			allsize;			// total file size

			unsigned int		cs_ptr;				// ptr to Code Segment
			std::size_t			cs_size;			// size of CS

			unsigned int		ds_ptr;				// ptr to Data Segment
			std::size_t			ds_size;			// size of DS
	
			unsigned int		ot_ptr;				// ptr to Object Temp
			std::size_t			ot_size;			// size of OT

			unsigned int		dinfo_ptr;			// ptr to Debug Info
			std::size_t			dinfo_size;			// size of DI
	
			unsigned int		pt_linfo;			// ptr to LibInfo(2.3)
			std::size_t			max_linfo;			// size of LibInfo(2.3)

			unsigned int		pt_finfo;			// ptr to FuncInfo(2.3)
			std::size_t			max_finfo;			// size of FuncInfo(2.3)
	
			unsigned int		pt_minfo;			// ptr to ModInfo(2.5)
			std::size_t			max_minfo;			// size of ModInfo(2.5)

			unsigned int		pt_finfo2;			// ptr to FuncInfo2(2.5)
			std::size_t			max_finfo2;			// size of FuncInfo2(2.5)
	
			unsigned int		pt_hpidat;			// ptr to HPIDAT(3.0)
			unsigned short		max_hpi;			// size of HPIDAT(3.0)

			unsigned short		max_varhpi;			// Num of Vartype Plugins(3.0)

			unsigned int		bootoption;			// bootup options
			unsigned int		runtime;			// ptr to runtime name
		};

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_HEADER_H*/