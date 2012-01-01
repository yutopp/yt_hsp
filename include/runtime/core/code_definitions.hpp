#ifndef YT_HSP_CODE_DEFINITIONS_HPP
#define YT_HSP_CODE_DEFINITIONS_HPP

#include <boost/detail/scoped_enum_emulation.hpp>

namespace yt_hsp
{
	namespace core
	{
		//コードの状態
		BOOST_SCOPED_ENUM_START( code_parameter ) {
			ok = 0,
			sprit = -1,
			end = -2,
			def = -3,	//default
			endsprit = -4
		}; BOOST_SCOPED_ENUM_END
		
		//実行状態
		BOOST_SCOPED_ENUM_START( run_mode ) {
			run = 0,	//正常
			wait,
			await,
			stop,
			end,
			error,
			ret,		//return
			intjump,
			assert,
			logmes,
			exitrun
		}; BOOST_SCOPED_ENUM_END

		//実行コマンド
		BOOST_SCOPED_ENUM_START( command_type )
		{
			mark_t	= 0,
			var_t,
			string_t,
			dnum_t,
			inum_t,
			struct_t,
			xlabel_t,
			label_t,
			intcmd_t,
			extcmd_t,		//実装は継承したクラスに
			extsysvar_t,	//実装は継承したクラスに
			cmpcmd_t,
			modcmd_t,
			intfunc_t,
			sysvar_t,
			progcmd_t,
			dllfunc_t,
			dllctrl_t,
			userdef_t
		}; BOOST_SCOPED_ENUM_END
		static int const max_command_types = 19;

		//演算子
		BOOST_SCOPED_ENUM_START( op_code ) {
			add, sub, mul, div, mod,
			and, or, xor,
			eq, ne,
			gt, lt, gteq, lteq,
			rr, lr,
			max
		}; BOOST_SCOPED_ENUM_END

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_CODE_DEFINITIONS_HPP*/