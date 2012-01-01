#ifndef YT_HSP_CONTEXT_MESSAGE_PASSENGER_H
#define YT_HSP_CONTEXT_MESSAGE_PASSENGER_H

namespace yt_hsp
{
	namespace core
	{
		//ëOï˚êÈåæ
		class global_context;

		//
		class context_message_passenger
		{
		public:
			context_message_passenger( global_context& ctx )
				: context_( ctx )
				, is_master_( false )
			{}

		private:
			global_context& context_;
			bool is_master_;
		};

	} // namespace core
} // namespace yt_hsp


#endif /*YT_HSP_CONTEXT_MESSAGE_PASSENGER_H*/