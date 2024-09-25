
#ifndef __ONLINEGAME_GAME_GS_COMMON_DATA_H_
#define __ONLINEGAME_GAME_GS_COMMON_DATA_H_

#include "commonvalue.h"
#include <map>
#include <set>
#include "rwlock.h"
#include "commondata_if.h"	//for CMN_DATA_SHARE


class CommonData	//gs��delivery�乫�������ݣ�������key-value��Ӧ������
{
	friend class CommonDataMan;
	friend class SyncTransaction;
public:
	RWLock _lock;

	void ReadLock() {_lock.ReadLock();}
	void ReadUnlock() {_lock.ReadUnlock();}
	void WriteLock() {_lock.WriteLock();}
	void WriteUnlock() {_lock.WriteUnlock();}

	//���ĳ��KEY��Ӧ��Value. ����ǰ�����
	bool GetValue (int key, Value& value);

	//�޸���������SyncTransaction

	int GetDataType() const {return _data_type;}
	int GetFlag() const {return _flag;}
	bool IsLocal() const {return !(_flag & CMN_DATA_SHARE);}//�Ƿ�ֻ��GS�ֲ�ʹ�ã���������GS����
	bool IsNeedDB() const {return (_flag & CMN_DATA_NEED_DB);}//��Ҫ���ݿ⣿
	void SetConsistent() {if (!_consistent) _consistent=true;}
	bool IsConsistent() const {return _consistent;}

	void ForEach (CommonDataInterface::Iterator &it);
private:
	int _data_type;
	typedef std::map<int, Value> DATA_MAP;
	DATA_MAP _map;

	int _flag;	//���ݿռ��һЩ����
	bool _consistent;  //�����Ƿ�����

	CommonData(int data_type, int flag):_data_type(data_type),_flag(flag),_consistent(false) {}
	~CommonData() {}

	void DataHandler (const Octets &data);

	//�޸�ĳ��KEY��Ӧ��Value. ����ǰ�����
	void ChangeValue (int key, int op, Value& value);

};

class CommonDataRpcObject;
class CommonDataMan
{
	//��д������������_map�ṹ�Ķ��̷߳���(�������Զ�̬������������һ����Ҫ������)
	RWLock _map_lock;
	std::map<int, CommonData*> _map;
	int _session_key;

	CommonDataMan();
	~CommonDataMan();

	static CommonDataMan Instance;
public:
	static CommonDataMan& GetInstance()
	{
		return Instance;
	}

	//���������ٺͻ�ȡ
	CommonData* Create(int data_type, int flag);
	//void Destroy(CommonData *);
	void Destroy(int data_type);
	CommonData* Get(int data_type);

	//��������������ݣ����зַ�
	void DispatchData (int session_key, int data_type, const Octets& data);
	//�����緢�ͣ�ͬ������
	void SyncDelivery (int data_type, Octets& data);

	void OnDeliveryDisConnect();

	void SendRpcCall(int data_type, int key, const Value& diff_value,
			 const Value& range_low, const Value& range_high, int op, CommonDataRpcObject *obj);
        void OnRecvRpcCallResult(int session_key, void *data,size_t size, int result, void *user_data);

private:
        void UpdateSessionKey (int session_key);
};

//һ��Transaction���޸Ĳ���ԭ�ӵģ����ɴ�ϵġ�
class SyncTransaction
{
	CommonData* _assoc_data;
	std::vector<data_atomic_action> _queue;
	bool _update_local;
	bool _submitted;
public:
	explicit SyncTransaction (int data_type);
	SyncTransaction (CommonData*);
	~SyncTransaction () { if (!_submitted) Submit();}

	//���ݵ���ز���
	int ValueInc (int key, Value::Type t = Value::TYPE_INT);	
	int ValueDec (int key, Value::Type t = Value::TYPE_INT);	

	int ValueAdd (int key, const Value& value);  //��������
	int ValueSub (int key, const Value& value);
	int ValueSet (int key, const Value& value);  //����

	void Cancel();
	void Submit();
};

#endif
