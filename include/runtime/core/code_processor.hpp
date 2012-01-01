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
		//���s�t���[/����
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

			//�v���O�����J�E���^��i�߂�.
			inline void forward_pc() throw()
			{
				reader_.forward_pc();
				ex_flag_ = reader_.current_ex_flag();
			}
			//�g��16bit(cmp�p�I�t�Z�b�g�Ȃ�)��ǂݐi�߂�
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

			//�W�����v
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
				//�R�[���X�^�b�N�Ɏ��s�ʒu���Ȃǂ�push
				call_point point = {
					reader_.current_pc(),
					context_.subroutine_level,
					is_function_call
				};
				context_.calls.push( std::move( point ) );

				//���[�J���ϐ��������߂����߂Ƀf���~�^�����Ă���
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
					std::cout << "�X�^�b�N�̒l�����������I" << "\n";
					throw "";
				}

				const call_point& point = context_.calls.top();
				if ( point.is_functon_call ) {
					// to do - �X�^�b�N���[���o�b�N�̏���
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

				// �^�C���A�E�g && �������[�v�Ŗ���
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

			//���x���p�����[�^�[���擾
			label_address_type get_code_as_label()
			{
				const code_value c = current_code();
				if ( c.type == command_type::label_t ) {
					//���x���萔�ł���΃R���e�L�X�g��������Ă���
					return reinterpret_cast<label_address_type>( context_.cs + context_.ot[c.data] );

				} else {
/*					//�����łȂꂯ�΃��x���ϐ��̃`�F�b�N
				    int chk;
				    unsigned short *p;
				    chk = code_get();
				    if ( chk<=PARAM_END ) { throw HSPERR_LABEL_REQUIRED; }
				    if ( mpval->flag != HSPVAR_FLAG_LABEL ) { throw HSPERR_LABEL_REQUIRED; }
				    p = *(unsigned short **)mpval->pt;
				    if ( p == NULL ) { // ���x���^�ϐ��̏����l�̓G���[��
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

				// �p�����[�^�[�I�[
				if ( reader_.current_ex1_flag() )
					return code_parameter::end;

				// �p�����[�^�[��؂�(�f�t�H���g��)
				if ( reader_.current_ex2_flag() ) {
					ex_flag_ = reader_.current_ex1_flag();
					return code_parameter::def;
				}

				//
				if ( reader_.current_type() == command_type::mark_t ) {
					int const val = reader_.current_data();
					if ( val == '?' ) {			// �p�����[�^�[�ȗ���('?')
						forward_pc();
						ex_flag_ = reader_.current_ex1_flag();
						return code_parameter::def;

					} else if ( val == ')' ) {	// �֐����̃p�����[�^�[�ȗ���
						ex_flag_ = reader_.current_ex1_flag();
						return code_parameter::endsprit;
					}
				}

				// �P���̍�����
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
							if ( value.data == ')' ) {                 // �����̏I���}�[�N
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

					if ( reader_.current_ex_flag() ) {	// �p�����[�^�[�I�[�`�F�b�N
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

				//�X�^�b�N������ɕ��A���Ă��Ȃ�
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

				// rhs �̌^�� lhs �ɍ��킹��
				if ( rhs.get_typeid() != lhs.get_typeid() ) {
					rhs.convert_to( lhs.get_typeid() );
				}

				//�v�Z
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

			//factory_type factory_;	//�^�W����
			temporary_type temp_;	//�e���|����

			//mutable temporary_type calc_cache_;		//�L���b�V��
		};

	}
}

#endif /*YT_HSP_CODE_PROCESSOR_HPP*/