int generate_equipment(unsigned int id, ID_SPACE idspace, char ** data, size_t& size, const void * tag,size_t tag_size)
{
	DATA_TYPE datatype;
	EQUIPMENT_ESSENCE * ess = (EQUIPMENT_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_EQUIPMENT_ESSENCE)	return -1;
		
	//size = sizeof(item_data) + sizeof(_equipment_essence);
	size = sizeof(item_data);
	
	//����tag size
//	ASSERT(tag_size >= sizeof(short));
//	size += tag_size;
	
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//��Ʒ��ģ��ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);			//��Ʒ������
	*(size_t*)buf = ess->pile_num_max;		buf += sizeof(size_t);			//��Ʒ�Ķѵ�����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);			//��Ʒ�Ĵ���ʽ
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = ess->price;			buf += sizeof(int);			//��Ʒ�ļ۸�
	*(int*)buf = 0;					buf += sizeof(int);			//��Ʒ�ĵ���ʱ��
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;

	//*(int*)buf = 0;					buf += sizeof(int);			//װ������Ƕ�ı�ʯ����
	//*(int*)buf = 0;					buf += sizeof(int);			//װ������Ƕ�ı�ʯ����
	//*(int*)buf = 0;					buf += sizeof(int);			//װ������Ƕ�ı�ʯ����	
	ASSERT( (size_t)(buf - *data)== size);

	return 0;
}

int generate_vehicle(unsigned int id, ID_SPACE idspace, char ** data, size_t& size, const void * tag,size_t tag_size)
{
	DATA_TYPE datatype;
	VEHICLE_ESSENCE * ess = (VEHICLE_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_VEHICLE_ESSENCE)	return -1;
	
	int addon_count = abase::RandSelect(ess->addon_prop,sizeof(float), 5);
	if(!ess->init_upgrade) 	addon_count = 0;

	size = sizeof(item_data) + sizeof(int) + addon_count*sizeof(int);
	
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//��Ʒ��ģ��ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);			//��Ʒ������
	*(size_t*)buf = ess->pile_num_max;		buf += sizeof(size_t);			//��Ʒ�Ķѵ�����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);			//��Ʒ�Ĵ���ʽ
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = ess->price;			buf += sizeof(int);			//��Ʒ�ļ۸�
	*(int*)buf = 0;					buf += sizeof(int);			//��Ʒ�ĵ���ʱ��
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;
	
	*(int*)buf = addon_count;			buf += sizeof(int);			//װ������Ƕ�ı�ʯ����
	for(int i = 0; i < addon_count; i ++)
	{
		int index= abase::RandSelect(&(ess->addon[0].prop),sizeof(ess->addon[0]),20);
		int addon = 0;
		if(index >= 0 && index < 24) addon = ess->addon[index].id;
		*(int*)buf = addon;					buf += sizeof(int);			//װ������Ƕ�ı�ʯ����	
	}
	
	
	ASSERT( (size_t)(buf - *data)== size);	
	return 0;
}


template <typename ESSENCE, typename ESSENCE_2>
int generate_normal_item(unsigned int id, ID_SPACE idspace, char ** data, size_t& size, ESSENCE * ,ESSENCE_2 *,DATA_TYPE odatatype )
{
	DATA_TYPE datatype;
	ESSENCE * ess_0 = (ESSENCE *)get_data_ptr(id, idspace, datatype);
	if( ess_0 == NULL || datatype != odatatype)	return -1;
	ASSERT(sizeof(ESSENCE_2) >= sizeof(ESSENCE));
	
	ESSENCE_2 ess;
	memset(&ess,0,sizeof(ess));
	memcpy(&ess,ess_0, sizeof(ESSENCE));

	ess_0 = &ess;
	
	size = sizeof(item_data);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//��Ʒ��ģ��ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);			//��Ʒ������
	*(size_t*)buf = ess.pile_num_max;		buf += sizeof(size_t);			//��Ʒ�Ķѵ�����
	*(int*)buf = ess.proc_type;			buf += sizeof(int);			//��Ʒ�Ĵ���ʽ
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = ess.price;				buf += sizeof(int);			//��Ʒ�ļ۸�
	*(int*)buf = 0;					buf += sizeof(int);			//��Ʒ�ĵ���ʱ��
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;

	ASSERT( (size_t)(buf - *data)== size);
	return 0;
}

int generate_medicine(unsigned int id, ID_SPACE idspace, char ** data, size_t& size)
{
	DATA_TYPE datatype;
	MEDICINE_ESSENCE * ess = (MEDICINE_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_MEDICINE_ESSENCE)	return -1;
		
	size = sizeof(item_data);
	int type = ess->type;
	switch(type)
	{
		case 0:
			size += sizeof(int);
			break;
		case 1:
			size += sizeof(int);
			break;
		case 2:
			size += sizeof(int) + sizeof(int);
			break;
		case 6:
			size += sizeof(int);
			break;
	}

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//��Ʒ��ģ��ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);			//��Ʒ������
	*(size_t*)buf = ess->pile_num_max;		buf += sizeof(size_t);			//��Ʒ�Ķѵ�����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);			//��Ʒ�Ĵ���ʽ
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = 0;					buf += sizeof(int);			//GUID	
	*(int*)buf = ess->price;			buf += sizeof(int);			//��Ʒ�ļ۸�
	*(int*)buf = 0;					buf += sizeof(int);			//��Ʒ�ĵ���ʱ��
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;

	switch(type)
	{
		case 0:
			*(int*)buf = ess->hp[0];buf += sizeof(int);
			break;
		case 1:
			*(int*)buf = ess->mp[0];buf += sizeof(int);
			break;
		case 2:
			*(int*)buf = ess->hp[0];buf += sizeof(int);
			*(int*)buf = ess->mp[0];buf += sizeof(int);
			break;
		case 6:
			*(int*)buf = ess->dp[0];buf += sizeof(int);
			break;

	}
	ASSERT( (size_t)(buf - *data)== size);

	return 0;
}

int generate_potion_bottle(unsigned int id, ID_SPACE idspace, char ** data, size_t& size)
{
	DATA_TYPE datatype;
	CASH_MEDIC_MERGE_ITEM_ESSENCE * ess = (CASH_MEDIC_MERGE_ITEM_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_CASH_MEDIC_MERGE_ITEM_ESSENCE)	return -1;
		
	size = sizeof(item_data) + sizeof(int);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//��Ʒ��ģ��ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);			//��Ʒ������
	*(size_t*)buf = ess->pile_num_max;		buf += sizeof(size_t);			//��Ʒ�Ķѵ�����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);			//��Ʒ�Ĵ���ʽ
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = 0;					buf += sizeof(int);			//GUID	
	*(int*)buf = ess->price;			buf += sizeof(int);			//��Ʒ�ļ۸�
	*(int*)buf = 0;					buf += sizeof(int);			//��Ʒ�ĵ���ʱ��
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;

	*(int*)buf = 0; buf += sizeof(int);
	ASSERT( (size_t)(buf - *data)== size);

	return 0;
}


int generate_aircraft_stamina_potion(unsigned int id, ID_SPACE idspace, char ** data, size_t& size)
{
	DATA_TYPE datatype;
	FLY_ENERGYFOOD_ESSENCE * ess = (FLY_ENERGYFOOD_ESSENCE *)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_FLY_ENERGYFOOD_ESSENCE)	return -1;
	
	size = sizeof(item_data) + sizeof(int);
	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//��Ʒ��ģ��ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);					//��Ʒ������
	*(size_t*)buf = ess->pile_num_max;		buf += sizeof(size_t);			//��Ʒ�Ķѵ�����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ���ʽ
	*(int*)buf = 0;					buf += sizeof(int);						//GUID
	*(int*)buf = 0;					buf += sizeof(int);						//GUID	
	*(int*)buf = ess->price;			buf += sizeof(int);					//��Ʒ�ļ۸�
	*(int*)buf = 0;					buf += sizeof(int);						//��Ʒ�ĵ���ʱ��
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);		//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);		//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;
	
	*(int*)buf = ess->energy_total; buf += sizeof(int);
	ASSERT( (size_t)(buf - *data)== size);
	
	return 0;
}

int generate_special_name_item(unsigned int id, ID_SPACE idspace, char **data, size_t & size, int name_id)
{
	DATA_TYPE datatype;
	SPECIAL_NAME_ITEM_ESSENCE* ess = (SPECIAL_NAME_ITEM_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_SPECIAL_NAME_ITEM_ESSENCE) return -1;

	if(ess->pile_num_max != 1) return -1;	

//	if(name_id == 0) return -1;

	size = sizeof(item_data) + sizeof(int);
	*data = (char*)abase::fastalloc(size);
	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//��Ʒ��ģ��ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);					//��Ʒ������
	*(size_t*)buf = ess->pile_num_max;		buf += sizeof(size_t);			//��Ʒ�Ķѵ�����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);				//��Ʒ�Ĵ���ʽ
	*(int*)buf = 0;					buf += sizeof(int);						//GUID
	*(int*)buf = 0;					buf += sizeof(int);						//GUID	
	*(int*)buf = ess->price;			buf += sizeof(int);					//��Ʒ�ļ۸�
	*(int*)buf = 0;					buf += sizeof(int);						//��Ʒ�ĵ���ʱ��
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);		//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);		//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;
	
	*(int*)buf = name_id; buf += sizeof(int);
	ASSERT( (size_t)(buf - *data)== size);
	
	return 0;
	

}

int generate_giftbox_delivery(unsigned int id, ID_SPACE idspace, char ** data, size_t& size)
{
	DATA_TYPE datatype;
	GIFT_BAG_LOTTERY_DELIVER_ESSENCE* ess = (GIFT_BAG_LOTTERY_DELIVER_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE)	return -1;
		
	size = sizeof(item_data) + sizeof(int) * 3 + sizeof(bool);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//��Ʒ��ģ��ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);			//��Ʒ������
	*(size_t*)buf = ess->pile_num_max;		buf += sizeof(size_t);			//��Ʒ�Ķѵ�����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);			//��Ʒ�Ĵ���ʽ
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = 0;					buf += sizeof(int);			//GUID	
	*(int*)buf = ess->price;			buf += sizeof(int);			//��Ʒ�ļ۸�
	*(int*)buf = 0;					buf += sizeof(int);			//��Ʒ�ĵ���ʱ��
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;

	*(int*)buf = 0; buf += sizeof(int);
	*(int*)buf = 0; buf += sizeof(int);
	*(bool*)buf = 0; buf += sizeof(bool);
	*(int*)buf = 0; buf += sizeof(int);
	ASSERT( (size_t)(buf - *data)== size);

	return 0;
}

int generate_lottery_ty(unsigned int id, ID_SPACE idspace, char ** data, size_t& size)
{
	DATA_TYPE datatype;
	LOTTERY_TANGYUAN_ITEM_ESSENCE* ess = (LOTTERY_TANGYUAN_ITEM_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_LOTTERY_TANGYUAN_ITEM_ESSENCE)	return -1;
		
	size = sizeof(item_data) + sizeof(int) + sizeof(char);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//��Ʒ��ģ��ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);			//��Ʒ������
	*(size_t*)buf = ess->pile_num_max;		buf += sizeof(size_t);			//��Ʒ�Ķѵ�����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);			//��Ʒ�Ĵ���ʽ
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = 0;					buf += sizeof(int);			//GUID	
	*(int*)buf = ess->price;			buf += sizeof(int);			//��Ʒ�ļ۸�
	*(int*)buf = 0;					buf += sizeof(int);			//��Ʒ�ĵ���ʱ��
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;

	*(int*)buf = 0; buf += sizeof(int);
	*(char*)buf = 0; buf += sizeof(char);
	ASSERT( (size_t)(buf - *data)== size);

	return 0;
}

int generate_giftbox_set(unsigned int id, ID_SPACE idspace, char ** data, size_t& size)
{
	DATA_TYPE datatype;
	GIFT_PACK_ITEM_ESSENCE* ess = (GIFT_PACK_ITEM_ESSENCE*)get_data_ptr(id, idspace, datatype);
	if( ess == NULL || datatype != DT_GIFT_PACK_ITEM_ESSENCE)	return -1;
		
	size = sizeof(item_data) + sizeof(int);

	// allocate the buffer with exact length
	*data = (char *)abase::fastalloc(size);	
	char * buf = *data;
	
	*(unsigned int*)buf = id;			buf += sizeof(unsigned int);		//��Ʒ��ģ��ID
	*(size_t*)buf = 1;				buf += sizeof(size_t);			//��Ʒ������
	*(size_t*)buf = ess->pile_num_max;		buf += sizeof(size_t);			//��Ʒ�Ķѵ�����
	*(int*)buf = ess->proc_type;			buf += sizeof(int);			//��Ʒ�Ĵ���ʽ
	*(int*)buf = 0;					buf += sizeof(int);			//GUID
	*(int*)buf = 0;					buf += sizeof(int);			//GUID	
	*(int*)buf = ess->price;			buf += sizeof(int);			//��Ʒ�ļ۸�
	*(int*)buf = 0;					buf += sizeof(int);			//��Ʒ�ĵ���ʱ��
	size_t* content_length = (size_t*)buf;		buf += sizeof(size_t);			//��סbuf��ָ�룬�Ժ�����
	char ** item_content = (char **)buf;		buf += sizeof(char *);			//��סbuf��ָ�룬�Ժ�����
	*content_length = (char *)(*data)+size-buf;
	*item_content = buf;

	*(int*)buf = 0; buf += sizeof(int);
	ASSERT( (size_t)(buf - *data)== size);

	return 0;
}
