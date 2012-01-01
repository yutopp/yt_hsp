#ifndef YT_HSP_SEGMENT_TYPE_H
#define YT_HSP_SEGMENT_TYPE_H

namespace yt_hsp
{
	namespace core
	{
		//セグメント
		struct segment
		{
			typedef char					binary_type;

			typedef unsigned short			cs_type;
			typedef cs_type*				cs_iterator_type;
			typedef const cs_type*			const_cs_iterator_type;

			typedef unsigned char			ds_type;
			typedef ds_type*				ds_iterator_type;
			typedef const ds_type*			const_ds_iterator_type;

			typedef ds_type					di_type;
			typedef ds_iterator_type		di_iterator_type;
			typedef const_ds_iterator_type	const_di_iterator_type;

			typedef unsigned int			ot_type;
			typedef ot_type*				ot_iterator_type;
			typedef const ot_type*			const_ot_iterator_type;
		};

		// アドレス
		typedef segment::const_cs_iterator_type label_address_type;

	} // namespace core
} // namespace yt_hsp

#endif /*YT_HSP_SEGMENT_TYPE_H*/