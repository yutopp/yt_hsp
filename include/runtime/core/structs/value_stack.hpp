#ifndef YT_HSP_VALUE_STACK_HPP
#define YT_HSP_VALUE_STACK_HPP

#include "../types/composite.hpp"

//#include <boost/array.hpp>
#include <vector>

namespace yt_hsp
{
	namespace core
	{
		//
		template<std::size_t N>
		class value_stack
		{
			typedef types::composite					value_type;
			//typedef boost::array<value_type, N>			container_type;
			typedef std::vector<value_type>				container_type;
			typedef typename container_type::size_type	size_type;

		public:
			value_stack()
				: current_( 0 )
				, container_( N )
			{}

			value_stack( value_stack&& rhs )
				: container_( std::move( rhs.container_ ) )
				, current_( rhs.current_ )
				, last_element_( rhs.last_element_ )
			{}

			template<typename T>
			void push( T&& value )
			{
				container_[current_] = std::forward<T>( value );
				++current_;
			}

			value_type pop()
			{
				if ( current_ <= 0 )
					throw "";

				--current_;
				return std::move( container_[current_] );
			}

			std::size_t get_level() const
			{
				return current_;
			}

		private:
			value_stack(value_stack const&);
			value_stack& operator=(value_stack const&);

			container_type container_;
			size_type current_;

			value_type* last_element_;
		};

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_VALUE_STACK_HPP*/