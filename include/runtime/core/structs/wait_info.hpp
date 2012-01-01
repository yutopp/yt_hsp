#ifndef YT_HSP_WAIT_INFO_HPP
#define YT_HSP_WAIT_INFO_HPP

namespace yt_hsp
{
	namespace core
	{
		// ë“ã@èÓïÒ
		class wait_info
		{
		public:
			wait_info()
				: count( 0 )
				, base( 0 )
				, tick( 0 )
				, lasttick( 0 )
			{}

		//private:

			int count;		// counter for wait
			int base;		// wait sleep base
			int tick;		// next tick for await
			int lasttick;	// previous tick
		};

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_WAIT_INFO_HPP*/