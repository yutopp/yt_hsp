#ifndef YT_HSP_COMMAND_EXECUTER_HPP
#define YT_HSP_COMMAND_EXECUTER_HPP

#include "detail/function_table.hpp"
#include "code_processor.hpp"
#include "code_definitions.hpp"

#include <boost/timer.hpp>

namespace yt_hsp
{
	namespace core
	{
		class function_executor
		{
			typedef code_processor					process_type;
			typedef BOOST_SCOPED_ENUM(run_mode)		result_type;

			typedef detail::function_table<
				result_type (*)(process_type&),
				max_command_types
			> func_table;
			func_table table_;

			//progcmd_executer<TypeConverter> ee__;

		public:
			function_executor()
			{
				initialize();
			}

			static result_type entry( process_type& flow )
			{
				const short type = flow.current_type();
				return func_table::call( type, flow );
			}

		private:
			static void initialize()
			{
				func_table::initialize( &entry_dummy );

				func_table::set_function( command_type::extcmd_t,	&entry_extcmd );
				func_table::set_function( command_type::cmpcmd_t,	&entry_cmpcmd );
				func_table::set_function( command_type::progcmd_t,	&entry_progcmd/*/&progcmd_executer<TypeConverter>::entry/**/ );
			}

			//
			static result_type entry_dummy( process_type& flow )
			{
				//std::cout << "--- dummy commmand." << std::endl;
				return run_mode::run;
			}


			// type_extcmd : 9
			static result_type entry_extcmd( process_type& flow )
			{
				switch( flow.current_data() )
				{
				case 0x0f:	// mes, print
					{
//						std::cout << "mes." << std::endl;

						flow.forward_pc();
						auto const chk = flow.get_code();
						if ( chk == code_parameter::end ) {
							std::puts( "" );	//改行
							return run_mode::run;
						}

						const auto& tmp = flow.get_temp_object();
						const auto type_id = tmp.get_typeid();
						if ( type_id == types::id::plain::string_type ) {
							std::puts( tmp.get<char const*>() );
							break;
						}
						
						// todo convert
						//tmp.convert_to();
						if ( type_id == types::id::plain::double_type ) {
							std::cout << tmp.get<double>() << std::endl;
							break;
						}

						std::cout << tmp.get<int>() << std::endl;

						

						break;
					}
				}

				return run_mode::run;
			}

			// type_cmpcmd : 12
			// -- 比較命令
			static result_type entry_cmpcmd( process_type& flow )
			{
				const int value = flow.current_code().data;

				flow.forward_pc();

				const auto offset = flow.read_mono_code();	//比較命令はオフセットが付いているため。
				const auto current_pc = flow.current_pc();
				
				// if
				if ( value == 0 ) {
					// true
					if ( flow.get_code_as_int() )
						return run_mode::run;
				}

				// else
				flow.jump( current_pc + offset );

				return run_mode::run;
			}

			// type_progcmd : 15
			//
			static result_type entry_progcmd( process_type& flow )
			{
				static boost::timer t;

				switch( flow.current_data() )
				{
				case 0x01:	// gosub
					{
//						std::cout << "gosub." << "\n";

						flow.forward_pc();
						const auto addr = flow.get_code_as_label();

						flow.push_subroutine_stack();
						flow.jump( addr );

						break;
					}
				

				case 0x02:	// return
//					std::cout << "return." << "\n";
					/*if ( !flow.get_ex_flag() )
						cmdfunc_return_setval();*/
					flow.set_runmode( run_mode::ret );
					return run_mode::ret;
				

				case 0x03:	// break
					if ( !flow.break_loop() )
						throw "";	//throw HSPERR_LOOP_WITHOUT_REPEAT
					break;


				case 0x04:	// repeat
					{
//						std::cout << "repeat." << "\n";

						flow.forward_pc();
						const auto break_addr = flow.get_code_as_label();
						
						flow.forward_pc();
						auto p1 = flow.get_code_as_int( loop_point::eternal );
						const auto p2 = flow.get_code_as_int( 0 );

						if ( p1 == 0 ) {	// 0は即break
							flow.jump( break_addr );
							flow.forward_pc();
							break;
						}
						
						if ( p1 < 0 ) {
							p1 = loop_point::eternal;
						} else {
							p1 += p2;
						}

						if ( !flow.set_loop_point( p1, p2 ) )
							throw "";	//throw HSPERR_TOO_MANY_NEST

						break;
					}
				

				case 0x05:	// loop
					{
//					std::cout << "loop." << "\n";

					if ( !flow.do_loop() )
						throw "";	//throw HSPERR_LOOP_WITHOUT_REPEAT

					break;
					}


				case 0x06:	// continue
					if ( !flow.continue_loop() )
						throw "";	//throw HSPERR_LOOP_WITHOUT_REPEAT
					break;


				case 0x11:	// stop
					//std::cout << "stop." << "\n";
					flow.set_runmode( run_mode::stop );
					return run_mode::stop;
				
				
				case 0x1e:	// resume
					t.restart();
					flow.forward_pc();
					break;
				
				case 0x1f:	/// yield
					std::cout << t.elapsed() << "sec.";
					flow.forward_pc();
					break;

				}

				return run_mode::run;
			}
		};

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_COMMAND_EXECUTER_HPP*/