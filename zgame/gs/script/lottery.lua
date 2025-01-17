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
	-- 数值区间为 cur_list[1] .. cur_list[n] 
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
--      彩票兑奖函数接口 这些接口会被程序部门的函数调用，用于实现彩票相关功能，
--      这些接口的具体实现可以被更改，但是本注释和接口的外在表现不应被修改。
--   
--      接口函数LotterySelectNumber 
--	说明：表示生成一个彩票中的某位数字
--      参数：
--           id : 彩票的物品 id， 不同的彩票可能有不同的表现
--           max_count: 这张彩票总共会有几个数字组成
--           cur_list: 当前已经生成的彩票号码 #cur_list就可以得到当前生成的号码数量 
--                     cur_list[1] 到 cur_list[#cur_list] 就是当前已经生成的所有号码
--                     标准的概率生成可能无需这些参数
--      返回：这个函数应当返回一个1~255的数值表示本次生成的单个彩票号码，0被认为
--            错误的彩票号码，因而被作为错误情况进行返回．
--
--      ************************************************************************
--
--      接口函数LotteryCashing
--      说明：对一张已经生成完毕的彩票进行兑奖操作
--      参数：
--           id : 彩票的物品 id， 不同的彩票可能有不同的表现
--           cur_list: 当前已经生成的彩票号码 #cur_list就可以得到当前所有的号码
--                     cur_list[1] 到 cur_list[#cur_list] 表示了依照生成顺序的号码
--      返回：兑奖后的结果由此接口返回，兑奖总共有六个整数型参数进行返回。
--          返回的格式为 return b_level b_money b_item b_count b_bind b_period;
--          各个返回值解释如下：
--           b_level: 本次彩票的中奖等级，等级是一个大于等于0的整数
--           b_money: 本次彩票的中奖奖金，奖金是一个大于等于0的整数
--           b_item:　本次彩票的奖励物品id，如果为0或-1则表示无奖品
--           b_count: 本次彩票的奖励物品数目，这个值应当大于等于0并不大于堆叠上限
--           b_bind: 本次中奖发放的物品奖励是否绑定，有效值为0或者1。只有堆叠上限为1这个参数才有意义
--           b_period:本次中奖发放物品奖励是否有有效时间，这是一个大于等于0的整数，大于0表示存在有效值。
--                    只有堆叠上限为1这个参数才有意义。
--         当返回参数个数不满足要求，或者参数的范围不满足要求时，这个函数的返回被认为是错误的。
-------------------------------------------------------------------------------------------------------------

--
--   有关接口实现　后面的两个接口是一个参考实现�
--　 使用if语句进行判断不同的彩票　然后调用不同彩票的生成和判断函数
--

function LotterySelectNumber(id, max_count, cur_list)
	if(id == 3613) then
		return LotteryType3613SelectNumber(cur_list,max_count);
	elseif(id == 3614) then 
		-- .... 如果3614 也是某种彩票的话可以做某些事情
	end
	return 0;	-- 返回错误
end

function LotteryCashing(id , cur_list)
	if(id == 3613) then
		return LotteryType3613Cashing(cur_list);
	elseif(id == 3614) then 
		-- .... 如果3614 也是某种彩票的话可以做某些事情
	end
	return 0,0,0,0,0,0;	-- 返回错误
end

-------------------------------------------------------------------------------------------------
--
--      另外一个备选实现　使用table保存所有的彩票接口　调用时直接查询并调用之
--	才后面要用AddLotteryTemplate　将每种彩票的映射加入到其中.
--	这只是一个提示性代码，接口的名字被修改为XXXX_aux，主程序并不会调用这个名称的接口函数
--

function LotterySelectNumber_aux(id, max_count, cur_list)
	local m = lottery_template[id];
	if(m) then
	return m.Select(cur_list, max_count);
	end
	return 0; 	-- 返回错误
end

function LotteryCashing_aux(id , cur_list)
	local m = lottery_template[id];
	if(m) then
	return m.Cashing(cur_list);
	end
	return 0;	-- 返回错误
end

AddLotteryTemplate(3613, LotteryType3613SelectNumber,LotteryType3613Cashing);

