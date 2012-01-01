#ifndef YT_HSP_IRQ_INFO_HPP
#define YT_HSP_IRQ_INFO_HPP


namespace yt_hsp
{
	namespace core
	{
		// äÑÇËçûÇ›èÓïÒ
		class irq_info
		{
		public:
			irq_info()
				: max_num( 0 )
			{}

		//private:

			//IRQDAT *mem_irq;	// IRQ data ptr
			int max_num;			// IRQ data count
			//int iparam;		// IRQ Info data1
			//int wparam;		// IRQ Info data2
			//int lparam;		// IRQ Info data3
		};

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_IRQ_INFO_HPP*/