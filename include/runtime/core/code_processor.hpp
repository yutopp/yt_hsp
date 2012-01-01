#ifndef YT_HSP_CODE_PROCESSOR_HPP
#define YT_HSP_CODE_PROCESSOR_HPP

#include "types/composite.hpp"
#include "thread_context.hpp"
#include "code_definitions.hpp"
#include "code_reader.hpp"
#include "error.hpp"

#include <utility>

namespace yt_hsp
{
	namespace core
	{
		//実行フロー/操作
		//template<typename TypeFactory>
		class code_processor
		{
		//	typedef TypeFactory							factory_type;
			typedef types::composite	temporary_type;
			typedef code_reader			reader_type;

		public:
			code_processor( thread_context&& ctx, reader_type::const_iterator_type address )
				: context_( std::move( ctx ) )
				, reader_( address )
				, ex_flag_( reader_.current_ex_flag() )
			{}

			//
			inline reader_type::const_iterator_type current_pc() const
			{
				return reader_.current_pc();
			}

			inline code_value current_code() const
			{
				return reader_.current_code();
			}

			inline short current_type() const
			{
				return reader_.current_type();
			}

			inline short current_data() const
			{
				return reader_.current_data();
			}

			//プログラムカウンタを進める.
			inline void forward_pc() throw()
			{
				reader_.forward_pc();
				ex_flag_ = reader_.current_ex_flag();
			}
			//拡張16bit(cmp用オフセットなど)を読み進める
			inline reader_type::cs_type read_mono_code() throw()
			{
				return reader_.read_mono_code();
			}

			inline void step_next( code_value& value )
			{
				reader_.forward_pc( value );
				value = current_code();
				ex_flag_ = reader_.current_ex_flag();
			}

			//ジャンプ
			inline void jump( const reader_type::const_iterator_type address ) throw()
			{
				reader_.jump( address );
				ex_flag_ = reader_.current_ex_flag();
			}

			bool get_ex_flag() const
			{
				return ex_flag_;
			}

			void set_runmode( BOOST_SCOPED_ENUM(run_mode) stat ) throw()
			{
				context_.execute_mode = stat;
			}

			BOOST_SCOPED_ENUM(run_mode) get_runmode() const throw()
			{
				//return static_cast<runmode>( context_.execute_mode );
				return context_.execute_mode;
			}

			temporary_type const& get_temp_object() const
			{
				return temp_;
			}

			//
			void push_subroutine_stack( bool is_function_call = false )
			{
				//コールスタックに実行位置情報などをpush
				call_point point = {
					reader_.current_pc(),
					context_.subroutine_level,
					is_function_call
				};
				context_.calls.push( std::move( point ) );

				//ローカル変数を巻き戻すためにデリミタを入れておく
				if ( is_function_call ) {
					//context_.stack2.push_delimiter();
				}

				++context_.subroutine_level;

//				std::cout << "push > return_addr: " << reader_.current_pc() << std::endl;
			}

			void pop_subroutine_stack()
			{
				if ( context_.subroutine_level == 0 )
				{
					std::cout << "スタックの値がおかしい！" << "\n";
					throw "";
				}

				const call_point& point = context_.calls.top();
				if ( point.is_functon_call ) {
					// to do - スタックロールバックの処理
				}

				--context_.subroutine_level;
				context_.calls.pop();

//				std::cout << "pop > return_addr: " << point.return_addr << std::endl;

				jump( point.return_addr );
				forward_pc();
			}

			//
			bool set_loop_point( int loop_time, int cnt_offset = 0, int step = 0 )
			{
				loop_point point = {
					loop_time,
					cnt_offset,
					step,
					reader_.current_pc()
				};

				return context_.loops.push( std::move( point ) );
//				std::cout << "set_loop > return_addr: " << reader_.current_pc() << std::endl;

			}

			inline bool do_loop()
			{

				auto& point = context_.loops.get_current_point();

				++point.cnt;

				// タイムアウト && 無限ループで無い
				if ( point.cnt >= point.time && point.time != loop_point::eternal ) {
					
					forward_pc();
					return context_.loops.pop();
				}
	
				jump( point.addr );
				return true;
			}

			bool break_loop()
			{
				return true;
			}

			bool continue_loop()
			{
				return true;
			}

			//ラベルパラメーターを取得
			label_address_type get_code_as_label()
			{
				const code_value c = current_code();
				if ( c.type == command_type::label_t ) {
					//ラベル定数であればコンテキストから引いてくる
					return reinterpret_cast<label_address_type>( context_.cs + context_.ot[c.data] );

				} else {
/*					//そうでなれけばラベル変数のチェック
				    int chk;
				    unsigned short *p;
				    chk = code_get();
				    if ( chk<=PARAM_END ) { throw HSPERR_LABEL_REQUIRED; }
				    if ( mpval->flag != HSPVAR_FLAG_LABEL ) { throw HSPERR_LABEL_REQUIRED; }
				    p = *(unsigned short **)mpval->pt;
				    if ( p == NULL ) { // ラベル型変数の初期値はエラーに
				        throw HSPERR_LABEL_REQUIRED;
				    }
				    mcs = mcsbak;
				    return p;*/
					return nullptr;
				}			
			}

			//
			BOOST_SCOPED_ENUM(code_parameter) get_code()
			{
//				std::cout << "!-- type: " << reader_.current_type() << " val: " << reader_.current_data() << " exflag[" << " 0:" << std::boolalpha << reader_.current_ex0_flag() << " 1:" << reader_.current_ex1_flag() << " 2:" << reader_.current_ex2_flag() << " ]" << std::dec << std::endl;

				// パラメーター終端
				if ( reader_.current_ex1_flag() )
					return code_parameter::end;

				// パラメーター区切り(デフォルト時)
				if ( reader_.current_ex2_flag() ) {
					ex_flag_ = reader_.current_ex1_flag();
					return code_parameter::def;
				}

				//
				if ( reader_.current_type() == command_type::mark_t ) {
					int const val = reader_.current_data();
					if ( val == '?' ) {			// パラメーター省略時('?')
						forward_pc();
						ex_flag_ = reader_.current_ex1_flag();
						return code_parameter::def;

					} else if ( val == ')' ) {	// 関数内のパラメーター省略時
						ex_flag_ = reader_.current_ex1_flag();
						return code_parameter::endsprit;
					}
				}

				// 単項の高速化
				if ( reader_.current_ex0_flag() ) {
					switch( reader_.current_type() ) {
					case command_type::inum_t:
						temp_.assign( types::int_t( reader_.current_data() ) );
						break;

					case command_type::dnum_t:
						temp_.assign( types::double_t( *reinterpret_cast<const double*>( &context_.ds[reader_.current_data()] ) ) );
						break;

					case command_type::string_t:
						temp_.assign( types::string_t( reinterpret_cast<const char*>( &context_.ds[reader_.current_data()] ), false ) );
						break;

					default:
						throw error::unknown_code;
					}

					forward_pc();
					return code_parameter::ok;
				}

				//std::size_t stack_level = context_.stack.get_level();
				std::size_t stack_level = context_.stack.get_level();

				code_value value = reader_.current_code();
				auto return_status = code_parameter::ok;
				
				for(;;) {				
					switch( value.type ) {
					case command_type::mark_t:
						{
							if ( value.data == ')' ) {                 // 引数の終了マーク
								if ( stack_level == context_.stack.get_level() ) {
									throw error::wrong_expression;
								}
								return_status = code_parameter::sprit;
								ex_flag_ = reader_.current_ex1_flag();
								break;
							}
							calculate_stack( static_cast<BOOST_SCOPED_ENUM(op_code)>( value.data ) );
							forward_pc();
							break;
						}

					case command_type::var_t:
						break;

					case command_type::inum_t:
						context_.stack.push( types::int_t( value.data ) );
						//context_.stack.push( value.data );
						forward_pc();
						break;

					//
					case command_type::string_t:
						context_.stack.push( types::string_t( reinterpret_cast<const char*>( &context_.ds[value.data] ), false ) );
						//context_.stack.push( reinterpret_cast<const char*>( &context_.ds[value.data] ) );
						forward_pc();
						break;

					case command_type::dnum_t:
						context_.stack.push( types::double_t( *reinterpret_cast<const double*>( &context_.ds[value.data] ) ) );
						//context_.stack.push( *reinterpret_cast<const double*>( &context_.ds[value.data] ) );
						forward_pc();
						break;

					case command_type::struct_t:
						break;

					case command_type::label_t:
						break;

					default:
						break;
					}
					
					value = current_code();

					//std::cout << " type: " << value.type << " val: " << value.data << " exflag[" << " 0:" << std::boolalpha << value.ex0_flag << " 1:" << value.ex1_flag << " 2:" << value.ex2_flag << " ]" << std::dec << std::endl;

					if ( reader_.current_ex_flag() ) {	// パラメーター終端チェック
						ex_flag_ = reader_.current_ex1_flag();
						break;
					}
				}

				//
				//temp_ = factory_( context_.stack );
				temp_ = context_.stack.pop();
				if ( !temp_ ) {
					throw error::unknown_code;
				}

				//スタックが正常に復帰していない
				if ( context_.stack.get_level() != stack_level ) {
				//if ( context_.stack.get_level() != stack_level ) {
					 throw "";
				}

				return return_status;
			}

			//
			int get_code_as_int()
			{
				auto const c = get_code();
				if ( c <= code_parameter::end )
					throw "";	//
				
				return get_code_as_int_impl();
			}

			int get_code_as_int( int default_value )
			{
				auto const c = get_code();
				if ( c <= code_parameter::end )
					return default_value;
				
				return get_code_as_int_impl();
			}

		private:
			void calculate_stack( BOOST_SCOPED_ENUM(op_code) code )
			{
				//auto lhs( factory_( context_.stack ) );
				//auto rhs( factory_( context_.stack ) );

				auto rhs( context_.stack.pop() );
				auto lhs( context_.stack.pop() );

				// rhs の型を lhs に合わせる
				if ( rhs.get_typeid() != lhs.get_typeid() ) {
					rhs.convert_to( lhs.get_typeid() );
				}

				//計算
				calculate( code, lhs, rhs );
				//calc_cache_ = std::move( rhs );

				//
				context_.stack.push( std::move( lhs ) );

				//factory_.push( context_.stack, std::move( calc_cache_ ) );
/*				context_.stack.push(
					variable::type_id::system::mixin_id,
					std::ref( calc_cache_ )
					);*/
			}
			
			template<typename T>
			inline void calculate( BOOST_SCOPED_ENUM(op_code) code, T& dest, T& src )
			{
				switch( code )
				{
				case op_code::add:
					dest.add_assign( src );
				    break;
/*				case CALCCODE_SUB:
				    proc->SubI( pval, ptr );
				    break;
				case CALCCODE_MUL:
				    proc->MulI( pval, ptr );
				    break;
				case CALCCODE_DIV:
				    proc->DivI( pval, ptr );
				    break;
				case CALCCODE_MOD:                      // '%'
				    proc->ModI( pval, ptr );
				    break;
				
				case CALCCODE_AND:
				    proc->AndI( pval, ptr );
				    break;
				case CALCCODE_OR:
				    proc->OrI( pval, ptr );
				    break;
				case CALCCODE_XOR:
				    proc->XorI( pval, ptr );
				    break;
				
				case CALCCODE_EQ:
				    proc->EqI( pval, ptr );
				    break;
				case CALCCODE_NE:
				    proc->NeI( pval, ptr );
				    break;
				case CALCCODE_GT:
				    proc->GtI( pval, ptr );
				    break;
				case CALCCODE_LT:
				    proc->LtI( pval, ptr );
				    break;
				case CALCCODE_GTEQ:                     // '>='
				    proc->GtEqI( pval, ptr );
				    break;
				case CALCCODE_LTEQ:                     // '<='
				    proc->LtEqI( pval, ptr );
				    break;
				
				case CALCCODE_RR:                       // '>>'
				    proc->RrI( pval, ptr );
				    break;
				case CALCCODE_LR:                       // '<<'
				    proc->LrI( pval, ptr );
				    break;
				case '(':
				    throw HSPERR_INVALID_ARRAY;*/
				/*default:
				    throw HSPVAR_ERROR_INVALID;*/
				}
			}

			int get_code_as_int_impl()
			{
				if ( temp_.get_typeid() != types::id::plain::int_type ) {
					if ( temp_.get_typeid() != types::id::plain::double_type )
						throw "";	//HSPERR_TYPE_MISMATCH

					// double -> int
					return static_cast<int>( temp_.get<double>() );       
				}
				
				return temp_.get<int>();
			}

			thread_context context_;
			reader_type reader_;

			bool ex_flag_;

			//factory_type factory_;	//型標準器
			temporary_type temp_;	//テンポラリ

			//mutable temporary_type calc_cache_;		//キャッシュ
		};

	}
}

#endif /*YT_HSP_CODE_PROCESSOR_HPP*/