#ifndef __LOG_PAGEBUFFER_HPP__
#define __LOG_PAGEBUFFER_HPP__
#include "logcommon/baseclass.hpp"
#include "logcommon/config.hpp"

namespace LOG
{

class PageBuffer
{
	// buffer: HDR + data
	struct HDR
	{
		unsigned short foffset;  // first record
		unsigned short loffset;  // data end-pos
		time_t timestamp;
	};

public:
	enum { SIZE = 4096, HDRSIZE = sizeof(struct HDR) };

	PageBuffer() { reset(time(NULL)); }

	// data
	const char * data() const { return buffer + HDRSIZE; }
	size_t dsize() const      { return Hdr()->loffset - HDRSIZE; }
	bool empty() const        { return dsize() == 0; }

	// record
	bool ispiece() const      { return Hdr()->foffset == 0; }

	// page
	time_t timestamp() const  { return Hdr()->timestamp; }
	ULL    getoffset() const  { return offset; }

	size_t first_record()     
	{ 
		if ( ispiece() ) throw LOG::Exception( "the page is piece");
		return Hdr()->foffset - HDRSIZE; /*����ȥ��ͷ���ľ���λ��, �����piece,����first_record������*/
	}


	/* 
	   PageBuffer pos �ĳ�ʼֵΪsizeof(HDR),��posָ��ľ���λ����bufferͷ��ʼ
	 */
	void reset( const time_t time )
	{
		memset(buffer, '\0', SIZE);
		Hdr()->foffset = 0;
		Hdr()->loffset = HDRSIZE;
		Hdr()->timestamp = time;
	}
private:
	friend class RecordWriter;
	// logserver
	// friend class 
	void fill_null( )
	{       
		size_t free = free_space();
		if(free > 0) memset(tail(), '\0', free);
	}       
	
	/* 
	   fill_record fill_piece �ֿ���ԭ��:
	   pagebuffer֪�����õ����¼�¼���Ƿ�Ƭ��¼
	   ����Ƿ�Ƭ��¼��ҳͷƫ�ƵȲ���
	 */
	size_t fill_record( const char * record, const size_t size )
	{
		if ( (Hdr()->foffset == 0) ) Hdr()->foffset = Hdr()->loffset;
		size_t fill = fill_piece( record,size ); 
		return fill;
	}

	size_t fill_piece( const char * record, const size_t size )
	{       
		size_t fill = std::min( size, free_space() );
		memcpy( tail(), record, fill );
		Hdr()->loffset += fill;
		return fill;
	}

	size_t free_space() const { return SIZE - Hdr()->loffset; }


private:

	friend class PageFile;
	// see PageFile::read
	void check()
	{
		size_t fo = Hdr()->foffset;
		size_t lo = Hdr()->loffset;
		if ( (0 == fo || (HDRSIZE <= fo && fo < SIZE)) && (HDRSIZE < lo && lo <= SIZE) ) return;
		throw LOG::Exception("bad pagebuffer");
	}

	// data
	char *tail() { return buffer + Hdr()->loffset; }
	HDR * Hdr() const { return reinterpret_cast<HDR *>(buffer); }

private:
	mutable char buffer[SIZE];
	ULL    offset;
};

} //end namespace LOG

#endif
