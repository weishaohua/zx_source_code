local lottery_template = {};

function AddLotteryTemplate(id, s1, c1)
	lottery_template[tonumber(id)] = {};
	lottery_template[tonumber(id)].Select = s1;
	lottery_template[tonumber(id)].Cashing = c1;
end

function LotteryType3613SelectNumber(list, max_count)
	return math.random(1,6); 
end

function LotteryType3613Cashing(cur_list)
	-- ��ֵ����Ϊ cur_list[1] .. cur_list[n] 
	local i,j
	local t={1,1,1}
	for i=1,3 do
	for j=i+1,6 do
	if (cur_list[i] == cur_list[j]) then
	t[i] = t[i]+1  	   
	end
	end	
	end
	if (cur_list[1]..cur_list[2]..cur_list[3]..cur_list[4]..cur_list[5]..cur_list[6]=="123456") then
	return 1,0,28,10,0,0;	
	end
	for i=1,3 do
	if (t[i] == 6) then
	return 1,100000,0,0,0,0;
	end
	if (t[i] == 5) then
	return 2,10000,0,0,0,0;
	end
	if (t[i] == 4) then
	return 3,100,0,0,0,0;
	end
	end		
	return 3,100,0,0,0,0;
end

--------------------------------------------------------------------------------------------------------------
--
--      ��Ʊ�ҽ������ӿ� ��Щ�ӿڻᱻ�����ŵĺ������ã�����ʵ�ֲ�Ʊ��ع��ܣ�
--      ��Щ�ӿڵľ���ʵ�ֿ��Ա����ģ����Ǳ�ע�ͺͽӿڵ����ڱ��ֲ�Ӧ���޸ġ�
--   
--      �ӿں���LotterySelectNumber 
--	˵������ʾ����һ����Ʊ�е�ĳλ����
--      ������
--           id : ��Ʊ����Ʒ id�� ��ͬ�Ĳ�Ʊ�����в�ͬ�ı���
--           max_count: ���Ų�Ʊ�ܹ����м����������
--           cur_list: ��ǰ�Ѿ����ɵĲ�Ʊ���� #cur_list�Ϳ��Եõ���ǰ���ɵĺ������� 
--                     cur_list[1] �� cur_list[#cur_list] ���ǵ�ǰ�Ѿ����ɵ����к���
--                     ��׼�ĸ������ɿ���������Щ����
--      ���أ��������Ӧ������һ��1~255����ֵ��ʾ�������ɵĵ�����Ʊ���룬0����Ϊ
--            ����Ĳ�Ʊ���룬�������Ϊ����������з��أ�
--
--      ************************************************************************
--
--      �ӿں���LotteryCashing
--      ˵������һ���Ѿ�������ϵĲ�Ʊ���жҽ�����
--      ������
--           id : ��Ʊ����Ʒ id�� ��ͬ�Ĳ�Ʊ�����в�ͬ�ı���
--           cur_list: ��ǰ�Ѿ����ɵĲ�Ʊ���� #cur_list�Ϳ��Եõ���ǰ���еĺ���
--                     cur_list[1] �� cur_list[#cur_list] ��ʾ����������˳��ĺ���
--      ���أ��ҽ���Ľ���ɴ˽ӿڷ��أ��ҽ��ܹ������������Ͳ������з��ء�
--          ���صĸ�ʽΪ return b_level b_money b_item b_count b_bind b_period;
--          ��������ֵ�������£�
--           b_level: ���β�Ʊ���н��ȼ����ȼ���һ�����ڵ���0������
--           b_money: ���β�Ʊ���н����𣬽�����һ�����ڵ���0������
--           b_item:�����β�Ʊ�Ľ�����Ʒid�����Ϊ0��-1���ʾ�޽�Ʒ
--           b_count: ���β�Ʊ�Ľ�����Ʒ��Ŀ�����ֵӦ�����ڵ���0�������ڶѵ�����
--           b_bind: �����н����ŵ���Ʒ�����Ƿ�󶨣���ЧֵΪ0����1��ֻ�жѵ�����Ϊ1���������������
--           b_period:�����н�������Ʒ�����Ƿ�����Чʱ�䣬����һ�����ڵ���0������������0��ʾ������Чֵ��
--                    ֻ�жѵ�����Ϊ1��������������塣
--         �����ز�������������Ҫ�󣬻��߲����ķ�Χ������Ҫ��ʱ����������ķ��ر���Ϊ�Ǵ���ġ�
-------------------------------------------------------------------------------------------------------------

--
--   �йؽӿ�ʵ�֡�����������ӿ���һ���ο�ʵ�֡
--�� ʹ��if�������жϲ�ͬ�Ĳ�Ʊ��Ȼ����ò�ͬ��Ʊ�����ɺ��жϺ���
--

function LotterySelectNumber(id, max_count, cur_list)
	if(id == 3613) then
		return LotteryType3613SelectNumber(cur_list,max_count);
	elseif(id == 3614) then 
		-- .... ���3614 Ҳ��ĳ�ֲ�Ʊ�Ļ�������ĳЩ����
	end
	return 0;	-- ���ش���
end

function LotteryCashing(id , cur_list)
	if(id == 3613) then
		return LotteryType3613Cashing(cur_list);
	elseif(id == 3614) then 
		-- .... ���3614 Ҳ��ĳ�ֲ�Ʊ�Ļ�������ĳЩ����
	end
	return 0,0,0,0,0,0;	-- ���ش���
end

-------------------------------------------------------------------------------------------------
--
--      ����һ����ѡʵ�֡�ʹ��table�������еĲ�Ʊ�ӿڡ�����ʱֱ�Ӳ�ѯ������֮
--	�ź���Ҫ��AddLotteryTemplate����ÿ�ֲ�Ʊ��ӳ����뵽����.
--	��ֻ��һ����ʾ�Դ��룬�ӿڵ����ֱ��޸�ΪXXXX_aux�������򲢲������������ƵĽӿں���
--

function LotterySelectNumber_aux(id, max_count, cur_list)
	local m = lottery_template[id];
	if(m) then
	return m.Select(cur_list, max_count);
	end
	return 0; 	-- ���ش���
end

function LotteryCashing_aux(id , cur_list)
	local m = lottery_template[id];
	if(m) then
	return m.Cashing(cur_list);
	end
	return 0;	-- ���ش���
end

AddLotteryTemplate(3613, LotteryType3613SelectNumber,LotteryType3613Cashing);

