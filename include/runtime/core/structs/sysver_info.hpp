#ifndef YT_HSP_SYSVER_INFO_HPP
#define YT_HSP_SYSVER_INFO_HPP

namespace yt_hsp
{
	namespace core
	{
		// ƒVƒXƒeƒ€•Ï”î•ñ
		struct sysvar_info
		{
		public:
			sysvar_info()
				: stat( 0 )
				, strsize( 0 )
				, refdval( 0 )
				, refstr( nullptr )
			{}

		//private:

			int stat;				// sysvar 'stat'
			int strsize;			// sysvar 'strsize'
			double refdval;			// sysvar 'refdval'
			char* refstr;			// pointer to refstr
		};

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_SYSVER_INFO_HPP*/