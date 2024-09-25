#include "mount_filter.h"
#include "clstab.h"
#include "arandomgen.h"
#include "actobject.h"
#include "world.h"
#include "item.h"

DEFINE_SUBSTANCE(mount_filter,filter,CLS_FILTER_MOUNT)

void 
mount_filter::AdjustDamage(damage_entry&, const XID &, const attack_msg&)
{
	//$$$$ �����Ѿ�����ʹ��
	//ÿ�α����ж���һ�����ʵ�����
	if(abase::Rand(0.f,1.f) < _drop_rate)
	{
		//����������ٻس������Ϣ
		//_is_deleted = true;
		_parent.GetImpl()->SendTo<0>(GM_MSG_PLAYER_RECALL_PET,_parent.GetSelfID(),0);
	}
}

void 
mount_filter::OnAttach()
{
	//1:�������״̬ 
	_parent.ActiveMountState(_mount_id,_mount_lvl,_no_broadcast, _mount_type);

	//2:�����ƶ��ٶ�
	_parent.EnhanceOverrideSpeed(_speedup);

	//��Ч��������
	gactive_imp * pImp = _parent.GetImpl();
	for(size_t i = 0; i < 4; i ++)
	{
		if(_addons[i].id <= 0) continue;

		DATA_TYPE dt;
		const EQUIPMENT_ADDON & addon= *(const EQUIPMENT_ADDON*)gmatrix::GetDataMan().get_data_ptr(_addons[i].id ,ID_SPACE_ADDON,dt);
		if(&addon == NULL  || dt != DT_EQUIPMENT_ADDON) continue;

		addon_data data = {addon.type, {addon.param1, addon.param2, addon.param3}};
		if(!addon_manager::TransformData(data)) continue;

		addon_handler * handler = addon_manager::QueryHandler(addon.type);
		if(!handler) continue;

		_addons[i].data = data;
		_addons[i].handler = handler;
		handler->Activate(data, NULL, pImp, NULL);
	}

	_parent.UpdateAllProp();
	_parent.SendClientCurSpeed();
}

void 
mount_filter::OnDetach()
{
	//1:�뿪���״̬
	_parent.DeactiveMountState();

	//2:�޸��ƶ��ٶ�
	_parent.ImpairOverrideSpeed(_speedup);

	//��Ч��������
	gactive_imp * pImp = _parent.GetImpl();
	for(size_t i = 0; i < 4; i ++)
	{
		if(!_addons[i].handler) continue;
		_addons[i].handler->Deactivate(_addons[i].data, NULL, pImp, NULL);
	}

	_parent.UpdateAllProp();
	_parent.SendClientCurSpeed();
}
	
bool 
mount_filter::Save(archive & ar)
{
	filter::Save(ar);
	ar << _mount_id << _mount_lvl << _speedup << _drop_rate;
	return true;
}

bool 
mount_filter::Load(archive & ar)
{
	filter::Load(ar);
	ar >> _mount_id >> _mount_lvl >> _speedup >> _drop_rate;
	return true;
}

void 
mount_filter::Merge(filter * f)
{
	mount_filter * pFilter = substance::DynamicCast<mount_filter>(f);
	ASSERT(pFilter);
	if(!pFilter) return ;
	
	//��������
	_mount_id = pFilter->_mount_id;
	_mount_lvl = pFilter->_mount_lvl;

	//�ٶ��и��ģ����½��м���
	if(_speedup != pFilter->_speedup)
	{
		_parent.ImpairOverrideSpeed(_speedup);
		_speedup = pFilter->_speedup;
		_parent.EnhanceOverrideSpeed(_speedup);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	_drop_rate = pFilter->_drop_rate;
}

