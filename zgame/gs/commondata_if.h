
#ifndef __ONLINEGAME_GAME_GS_COMMON_DATA_IF_H_
#define __ONLINEGAME_GAME_GS_COMMON_DATA_IF_H_

class CommonData;

class CommonDataInterface
{
      CommonData* _pcd;
public:

      class Iterator
      {       
      public: 
	      Iterator(){}
	      virtual ~Iterator(){} 
	      virtual void operator()(int data_type, int key, int value)=0;
      };      
      CommonDataInterface(int data_type=0);

      bool IsConsistent();

      bool GetData(int index, int& val);
      void SetData(int index, int val);

      bool GetUserData(int index, void *data, int *size);
      void SetUserData(int index, void *data, int size);

      void IncData (int index);
      void DecData (int index);
 
      void AddData (int index, int diff_val);
      void SubData (int index, int diff_val);

      void ForEach(CommonDataInterface::Iterator &it);
};


//Rpc���ھ�ȷ����ĳ��value. 
class  CommonDataRpcArg
{
public:
      int _data_type;		//���ݿռ�
      int _index;		//����
      int _diff_val;		//����������Ϊ��ֵ��
      int _range_low;		//����
      int _range_high;		//����,���delivery�ϵ���ֵ�����������󣬳�������ֵ����rpc����ʧ��

      CommonDataRpcArg(){}

      CommonDataRpcArg(int data_type, int index, int diff_val, int range_low, int range_high):
       _data_type(data_type),_index(index), _diff_val(diff_val), 
       _range_low(range_low),_range_high(range_high) { }
};

enum op_mode {
        op_mode_inc,    //����
        op_mode_dec,    //�Լ�
        op_mode_set,    //����
        op_mode_add,    //����ĳֵ
        op_mode_sub,    //��ȥĳֵ
        op_mode_xchg,   //����
};

class CommonDataRpcObject
{
protected:
      int _op;
      CommonDataRpcArg _arg;
public:
      CommonDataRpcObject(int op = op_mode_add): _op(op){}
      virtual ~CommonDataRpcObject() {}

      void SetArg(const CommonDataRpcArg& arg) { _arg = arg;}
      int  GetOp () { return _op;}
      void SetOp (int op) {_op = op;}

      enum {
	      RESULT_OK,		//�ɹ�
	      RESULT_OUT_OF_RANGE, 	//��֮����ֵ�ᳬ���߽�
	      RESULT_TIMEOUT,		//��ʱ
	      RESULT_DEST_IS_LOCAL,	//��������Ŀ�����ڲ��ռ������
      };
      /*
       *  ��ý���Ļص�
       *  @param ret_val  ���ص�ֵ���Խ�����������ԭ�ȵ�ֵ�������������ǵ�ǰ��ֵ
       *  @param result   ���
       *  @note  ���յ�OnGetResult��һ�ε��õĹ��̽����������ٴν����󷢳������������ͷš�
       */
      virtual void OnGetResult(int ret_val,int result) =0;
};

/*
 * ��Rpc���󷢳�
 * @param obj 
 * @param arg ����Ҫ�Ĳ���
 * @note ���յ�����ǰ�������ͷ�obj
 */
void SendCommonDataRpcObject (CommonDataRpcObject *obj, const CommonDataRpcArg& arg);

enum {
	CMN_DATA_LOCAL   = 0,  //�ڲ���ʹ�õģ��ǹ���
	CMN_DATA_SHARE	 = 1,  //��GS�乲��
	CMN_DATA_NEED_DB = 2,  //��Ҫ����
	CMN_DATA_DEFAULT = 3,  //Ĭ�� (CMN_DATA_SHARE | CMN_DATA_NEED_DB)
};
/*
 * �������ݿռ�
 * @param data_type ��Ҫ���������ݿռ�����
 * @param flag  ���ݿռ������, ����һ��local�Ϳ����� CMN_DATA_LOCAL
 */
bool CommonDataSpaceCreate (int data_type, int flag = CMN_DATA_SHARE | CMN_DATA_NEED_DB);

/**
 * �����Ѿ����������ݿռ�
 * @param data_type, ��Ҫɾ�������ݿռ�����
 */
bool CommonDataSpaceDestroy(int data_type);
#endif
