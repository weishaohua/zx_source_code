#include "topflowermanager.h"
#include "dbgettopflowerdata.hrp"
#include "dbupdatetopflowerdata.hrp"
bool TopFlowerManager::InitTop(bool isRecv, std::vector<GTopFlowerData>& data)
{
	TMultiMap& _topMap = isRecv ? m_RecvTopMap : m_SendTopMap;

	for(int i=0; i<(int)data.size(); ++i)
	{
		STopFlowerID _id;
		_id.count = data[i].count;
		_id.stamp = data[i].stamp;
		_topMap.insert(std::make_pair(_id, data[i]));
		//if(data[i].count >= 999)
		//	LOG_TRACE("TopFlowerManager::InitTop i:%d, count:%d. isRecv:%d", i, data[i].count, isRecv);

	}
	return true;
}

bool TopFlowerManager::InitAddTop(bool isRecv, std::vector<GTopFlowerData>& data)
{
        TMap& _allMap = isRecv ? m_RecvAllMap : m_SendAllMap;

	for(int i=0; i<(int)data.size(); ++i)
	{
		_allMap.insert(std::make_pair(data[i].roleid, data[i]));
	}

	return true;
}

bool TopFlowerManager::CheckGetTop()// ���а��ȡ���ܷ���
{
	//return true;
	struct tm start = {0, 0, 10, 26, 2, 2013 - 1900, 0, 0, 0};// ��������·ݣ�0-11
	struct tm end = {0, 0, 8, 30, 3, 2013 - 1900, 0, 0, 0};// �콱����ʱ��
	return _CheckTime(start, end);
}


bool TopFlowerManager::CheckOpenTop()// ���а���ܺͷ����ʻ����ܷ���
{
	//return true;
	struct tm start = {0, 0, 10, 26, 2, 2013 - 1900, 0, 0, 0};// ��������·ݣ�0-11
	struct tm end = {0, 0, 8, 2, 3, 2013 - 1900, 0, 0, 0};
	return _CheckTime(start, end);
}

bool TopFlowerManager::CheckOpenGift()// ��ȡ�������ܷ���
{
	//return false;
	struct tm start = {0, 0, 10, 9, 3, 2013 - 1900, 0, 0, 0};// ��������·ݣ�0-11
	struct tm end = {0, 0, 8, 30, 3, 2020 - 1900, 0, 0, 0};
	return _CheckTime(start, end);
}


bool TopFlowerManager::Update()
{
	if(status == TOPFLOWER_READY)
        {
                return false;// �Ѿ���ʼ����ϣ�����
        }

	if(CheckGetTop())
	{
		OnDBConnect();
	}
/*
	if(CheckOpenGift())
	{
		// 
	}
*/
	return true;
}

void TopFlowerManager::OnDBConnect()
{

	DBGetTopFlowerData* recv_rpc = (DBGetTopFlowerData*) Rpc::Call(RPC_DBGETTOPFLOWERDATA, DBGetTopFlowerDataArg(0, TOPFLOWER_ALL_COUNT, true));// ȡ���ջ��İ�
        GameDBClient::GetInstance()->SendProtocol(recv_rpc);
	
	DBGetTopFlowerData* send_rpc = (DBGetTopFlowerData*) Rpc::Call(RPC_DBGETTOPFLOWERDATA, DBGetTopFlowerDataArg(0, TOPFLOWER_ALL_COUNT, false));// ȡ���ͻ��İ�
        GameDBClient::GetInstance()->SendProtocol(send_rpc);

}

void TopFlowerManager::Open()
{
        status = TOPFLOWER_READY;
}

ETopFlowerStatus TopFlowerManager::UpdateTop(bool isRecv, int roleid, int userid, int add_count, int gender, const Octets& rolename)
{
        if(status != TOPFLOWER_READY)
        {
                return TOPFLOWER_NOTREADY;
        }

        TMultiMap& _topMap = isRecv ? m_RecvTopMap : m_SendTopMap;
        TMap& _allMap = isRecv ? m_RecvAllMap : m_SendAllMap;

        TMap::iterator _it, _itEnd;
        _it = _allMap.find(roleid);
        _itEnd = _allMap.end();

        if(_it == _itEnd)
        {
                //������û�м�¼�� ��Ҫ����Map����
                GTopFlowerData _newData;
                _newData.roleid = roleid;
		_newData.userid  = userid;
		_newData.rolename = rolename;
                _newData.stamp = (int)_Local_MillSecond();
                _newData.count =  add_count;
                _newData.gender = gender;
		_newData.isGiftTake = 0;
		
		LOG_TRACE("TopFlowerManager::UpdateTop rolename:%s, size=%d.", rolename.begin() ,rolename.size());

                _allMap.insert(std::make_pair(roleid, _newData));

                if(_topMap.size() < TOPFLOWER_ALL_COUNT || _newData.count > _GetTopMinCount(isRecv))
                {
                        // ����Top��
                        _UpdateTop(isRecv, _newData, _newData);
                }
		
		DBUpdateTopFlowerData* rpc = (DBUpdateTopFlowerData*) Rpc::Call(RPC_DBUPDATETOPFLOWERDATA, DBUpdateTopFlowerDataArg(isRecv, 0, _newData));
        	GameDBClient::GetInstance()->SendProtocol(rpc);

        }
        else
        {
                GTopFlowerData _oldData = _it->second;

                // ���������м�¼,�޸�
                _it->second.stamp = (int)_Local_MillSecond();
                _it->second.count += add_count;

                if(_topMap.size() < TOPFLOWER_ALL_COUNT || _it->second.count > _GetTopMinCount(isRecv))
                {
                        // ����Top��
                        _UpdateTop(isRecv, _oldData, _it->second);
                }
		
		DBUpdateTopFlowerData* rpc = (DBUpdateTopFlowerData*) Rpc::Call(RPC_DBUPDATETOPFLOWERDATA, DBUpdateTopFlowerDataArg(isRecv, 0, _it->second));
        	GameDBClient::GetInstance()->SendProtocol(rpc);

        }

        return TOPFLOWER_OPT_OK; 
}
 
bool TopFlowerManager::GetTop(bool isRecv, int page,  std::vector<GTopFlowerData>& data, int& all_count)
{
        if(status != TOPFLOWER_READY)
        {
                return false;
        }

	TMultiMap& _topMap = isRecv ? m_RecvTopMap : m_SendTopMap;
        TMultiMap::iterator _it;

        int _startPos = page*ONE_PAGE_COUNT;
        if(_startPos >= TOPFLOWER_CLIENT_COUNT || _startPos >= (int)_topMap.size() || _startPos<0)
        {
                data.clear();
                all_count = (int)_topMap.size();
                return false;
        }

        int _endPos = (_startPos + ONE_PAGE_COUNT) < (int)_topMap.size() ? (_startPos + ONE_PAGE_COUNT) : (int)_topMap.size();
	if(_endPos > TOPFLOWER_CLIENT_COUNT)
	{
		_endPos = TOPFLOWER_CLIENT_COUNT;
	}

        int _tmpCount = 0;
        for(_it = _topMap.begin(); _it != _topMap.end(); ++_it)
        {
                if(_tmpCount >= _startPos && _tmpCount < _endPos)
                {
			std::string _s((char*)_it->second.rolename.begin() ,(int)_it->second.rolename.size());
                	//LOG_TRACE("GetTop roleid:%d rolename:%s, namesize=%d, count:%d.", _it->second.roleid, _s.c_str(), _it->second.rolename.size(), _it->second.count);
			data.push_back(_it->second);
                }
                _tmpCount++;
        }

        all_count = (int)_topMap.size() > TOPFLOWER_CLIENT_COUNT ? TOPFLOWER_CLIENT_COUNT : (int)_topMap.size(); 

        return true;
}

int TopFlowerManager::GetTopForWeb(int type, std::vector<FlowerTopRecord> & top)
{
	if(status != TOPFLOWER_READY)
        {
                return TOPFLOWER_NOTREADY;
        }

	TMultiMap& _topMap = (type == 1) ? m_RecvTopMap : m_SendTopMap;
	TMultiMap::iterator _it;

	int _nCount = 0;
	for(_it = _topMap.begin(); _it != _topMap.end() && _nCount < TOPFLOWER_CLIENT_COUNT; ++_it)
        {
		GTopFlowerData _data = _it->second;
		
		FlowerTopRecord _record;
		_record.userid = _data.userid;
		_record.rolename = _data.rolename;
		_record.timestamp = _data.stamp;
		_record.number = _data.count;
              	
		top.push_back(_record);
		_nCount++; 
        }

	LOG_TRACE("TopFlowerManager::GetTopForWeb type:%d, top.size=%d.", type, top.size());

	return TOPFLOWER_OPT_OK;


}

bool TopFlowerManager::GetRolePos(bool isRecv, int roleid, int& pos, int& count)
{
         if(status != TOPFLOWER_READY)
        {
                return false;
        }

	GTopFlowerData* _pFlowerData = _GetKey(isRecv, roleid);
        if(!_pFlowerData)
        {
                pos = -1;// -1��ʾδ���
                count = 0;

                return false;
        }

        STopFlowerID _oldKey;
        _oldKey.count = _pFlowerData->count;
        _oldKey.stamp = _pFlowerData->stamp;

        TMultiMap& _topMap = isRecv ? m_RecvTopMap : m_SendTopMap;
        TMultiMap::iterator _it, _itBegin, _itEnd;

	_itBegin = _topMap.begin();
        _itEnd = _topMap.end();

	int _tmpPos = 0;
        for(_it=_itBegin; _it!=_itEnd && _tmpPos < TOPFLOWER_CLIENT_COUNT; ++_it)
	{
		if(roleid == _it->second.roleid)
		{
			pos = _tmpPos;
			count = _it->second.count;
			return true;
		}
		_tmpPos++;
	}
        
	pos = -1;
  	count = _pFlowerData->count;

        return false;
}

bool TopFlowerManager::IsGiftTake(bool isRecv, int roleid)
{
	 if(status != TOPFLOWER_READY)
        {
                return false;
        }

	if(!CheckOpenGift())
	{
		return false;
	}

        GTopFlowerData* _pFlowerData = _GetKey(isRecv, roleid);
        if(!_pFlowerData)
        {
                return false;
        }

	return  0 == _pFlowerData->isGiftTake;
}

bool TopFlowerManager::UpdateGiftTake(bool isRecv, int roleid)
{
        if(status != TOPFLOWER_READY)
        {
                return false;
        }

	 GTopFlowerData* _pFlowerData = _GetKey(isRecv, roleid);
        if(!_pFlowerData)
	{
		return false;
	}
	
	_pFlowerData->isGiftTake = 1;// 1��־���Ѿ��콱��
 	
	GTopFlowerData _newData; 
	_newData.roleid = roleid;
	
	DBUpdateTopFlowerData* rpc = (DBUpdateTopFlowerData*) Rpc::Call(RPC_DBUPDATETOPFLOWERDATA, DBUpdateTopFlowerDataArg(isRecv, 2, _newData));// 2��ʾ��ʶ�콱��_newDataֻ��roleid�͹���
        GameDBClient::GetInstance()->SendProtocol(rpc);

	return true;
}

bool TopFlowerManager::DelRoleTop(bool isRecv, int roleid)
{
	if(status != TOPFLOWER_READY)
        {
                return false;
        }

	GTopFlowerData* _pFlowerData = _GetKey(isRecv, roleid);
        if(!_pFlowerData)
        {
		// û���ʻ������������
                return false;
        }

	STopFlowerID _oldKey;
        _oldKey.count = _pFlowerData->count;
        _oldKey.stamp = _pFlowerData->stamp;

	TMultiMap& _topMap = isRecv ? m_RecvTopMap : m_SendTopMap;
        TMap& _allMap = isRecv ? m_RecvAllMap : m_SendAllMap;
	
	_allMap.erase(roleid);
	_topMap.erase(_oldKey);

	GTopFlowerData _newData;
	_newData.roleid = roleid;

	DBUpdateTopFlowerData* rpc = (DBUpdateTopFlowerData*) Rpc::Call(RPC_DBUPDATETOPFLOWERDATA, DBUpdateTopFlowerDataArg(isRecv, 1, _newData));// 1��ʾɾ��������_newDataֻҪroleid������
        GameDBClient::GetInstance()->SendProtocol(rpc);

	return true;
}

bool TopFlowerManager::OnRolenameChange(bool isRecv, int roleid, const Octets& oldname, const Octets& newname)
{
	if(status != TOPFLOWER_READY)
        {
                return false;
        }

	GTopFlowerData* _pFlowerData = _GetKey(isRecv, roleid);
        if(!_pFlowerData)
        {
		// û���ʻ������������
                return false;
        }

	STopFlowerID _oldKey;
        _oldKey.count = _pFlowerData->count;
        _oldKey.stamp = _pFlowerData->stamp;

	TMultiMap& _topMap = isRecv ? m_RecvTopMap : m_SendTopMap;
        TMap& _allMap = isRecv ? m_RecvAllMap : m_SendAllMap;

	TMap::iterator _it_all = _allMap.find(roleid);
	if(_it_all != _allMap.end())
	{
		_it_all->second.rolename = newname;
	}

	TMultiMap::iterator _it_top = _topMap.find(_oldKey);
	if(_it_top != _topMap.end())
	{
		for(_it_top=_topMap.begin(); _it_top!= _topMap.end(); ++_it_top)
		{
			if(_it_top->second.rolename == oldname)
			{
				std::string _sold((char*)oldname.begin() ,(int)oldname.size());
				std::string _snew((char*)newname.begin() ,(int)newname.size());
				LOG_TRACE("TopFlowerManager::OnRolenameChange old_rolename:%s, new_rolename=%s.", _sold.c_str(),_snew.c_str());

				_it_top->second.rolename = newname;
			}
		}

	}

	GTopFlowerData _newData;
	_newData.roleid = roleid;
	_newData.rolename = newname;

	DBUpdateTopFlowerData* rpc = (DBUpdateTopFlowerData*) Rpc::Call(RPC_DBUPDATETOPFLOWERDATA, DBUpdateTopFlowerDataArg(isRecv, 3, _newData));
        GameDBClient::GetInstance()->SendProtocol(rpc);


	return true;
}

int TopFlowerManager::_GetTopMinCount(bool isRecv)
{
        TMultiMap& _topMap = isRecv ? m_RecvTopMap : m_SendTopMap;

        TMultiMap::reverse_iterator _rIt;
        if(_topMap.empty())
        {
                return 0;
        }
        _rIt = _topMap.rbegin();

        return _rIt->first.count;
}

GTopFlowerData* TopFlowerManager::_GetKey(bool isRecv, int roleid)
{
        TMap& _allMap = isRecv ? m_RecvAllMap : m_SendAllMap;

        // �����ϵ�TopFlowerID
        TMap::iterator _it, _itEnd;
        _it = _allMap.find(roleid);
        _itEnd = _allMap.end();

        if(_it == _itEnd)
        {
                return NULL;
        }
        else
        {
                return &_it->second;
        }


}
 
bool TopFlowerManager::_UpdateTop(bool isRecv, GTopFlowerData& oldData, GTopFlowerData& newData)
{
	STopFlowerID _oldKey;
	_oldKey.count = oldData.count;
	_oldKey.stamp = oldData.stamp;

	STopFlowerID _newKey;
	_newKey.count = newData.count;
	_newKey.stamp = newData.stamp;

	TMultiMap::iterator _it, _itEnd;
	TMultiMap& _topMap = isRecv ? m_RecvTopMap : m_SendTopMap;

	_it = _topMap.find(_oldKey);
	_itEnd = _topMap.end();

	if(_it != _itEnd)
	{
		//����м�¼,��ʱ����ԭֵ��ɾ����¼�������¼�¼
		_topMap.erase(_it->first);
		_topMap.insert(std::make_pair(_newKey, newData));
	}
	else
	{
		//���û�м�¼��ֱ�Ӳ����¼�¼
		_topMap.insert(std::make_pair(_newKey, newData));

		// �������TOPFLOWER_COUNT������С��ɾ��
		if(_topMap.size() > TOPFLOWER_ALL_COUNT)
		{
			_topMap.erase(_topMap.rbegin()->first);
		}
	}

	return true;
}
