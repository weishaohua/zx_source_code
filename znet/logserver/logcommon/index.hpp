#ifndef __LOG_INDEX_HPP__
#define __LOG_INDEX_HPP__
#include "logcommon/baseclass.hpp"
#include "protocol.h"
#include "marshal.h"

namespace LOG
{

struct Index
{
	GNET::Protocol * createProtocol(const GNET::Marshal::OctetsStream &data)
	{
                return create(data);
	}

	virtual bool is_rebuild() const { return false; }
	virtual void destroy() { delete this; }

	///////////////////////////////////////////////////
	//
	//����Э�����, protocol ָ����indexmaker����ɾ��
	//
	///////////////////////////////////////////////////
	virtual GNET::Protocol * create(const GNET::Marshal::OctetsStream &data) const = 0;

	///////////////////////////////////////////////////
	//
	//���keyvalueֵ, �û���д
	//
	///////////////////////////////////////////////////
	virtual keyvalue_t extractkeyvalue(GNET::Protocol *p) const  = 0;
	virtual type_t get_type() const  = 0;	
	virtual std::string get_keyname() const  = 0;	
	virtual ~Index() {}
};

}//end namespace LOG

#endif
