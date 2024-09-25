#ifndef __LOG_QUERY_HPP__
#define __LOG_QUERY_HPP__
#include "marshal.h"
#include "protocol.h"
#include "logcommon/config.hpp"
#include "logcommon/baseclass.hpp"

namespace LOG
{

class Query
{
public:
	virtual type_t get_type() = 0;
        virtual std::string get_keyname() = 0;
        virtual GNET::Marshal::OctetsStream get_keyvalue() = 0;
        virtual TimeRange get_timerange() = 0;
        virtual void process_protocol(GNET::Marshal::OctetsStream &) = 0;

	/////////////////////////////////////////////////////////////////////////////////////
	//
	// �����ѯ����ʧ��, is_scan == true, ����get_file_or_dir()����ļ����б�ɨ���¼
	// file_or_dir rule:
	// "file1 file2 file3 dir"
	//
	/////////////////////////////////////////////////////////////////////////////////////

        virtual bool enable_scan_if_not_found_idx(){ return false; };
        virtual std::string get_file_or_dir() { return ""; }


	/*
        void process_record( const GNET::Marshal::OctetsStream &rec ) //MYTODO ���ٿ�������
        {
		GNET::Marshal::OctetsStream protocol;
		get_protocol_data( rec, protocol );
		process_protocol( protocol );
        }

	void get_protocol_data(const GNET::Marshal::OctetsStream &data, GNET::Marshal::OctetsStream & protocol)
        {
                RecordHead recordhead;
                data >> recordhead >> protocol;
        }
	*/
	virtual ~Query() {}
};

}
#endif
