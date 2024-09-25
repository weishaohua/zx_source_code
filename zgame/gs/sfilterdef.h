#ifndef __SKILL_SFILTERDEF_H__
#define __SKILL_SFILTERDEF_H__

enum filter_class_id{
	CLASS_DIZZY          = 4096,   // ����
	CLASS_SLEEP          = 4097,   // ˯��
	CLASS_WEAK           = 4098,   // ����
	CLASS_SILENT         = 4099,   // ��Ĭ
	CLASS_WRAP           = 4100,   // ����
	CLASS_SLOW           = 4101,   // ����
	CLASS_INCCRITRATE    = 4102,   // ��ǿ������
	CLASS_HPLEAK         = 4103,   // ����������Ѫ
	CLASS_DECATTACK      = 4104,   // ���͹����������ͱ���
	CLASS_DECDEFENCE     = 4105,   // ���������������ͱ���
	CLASS_SUBATTACK      = 4106,   // ���ͷ��������������ͱ���
	CLASS_DECDODGE       = 4107,   // ���Ͷ���ʣ�������ֵ
	CLASS_INCHPGEN       = 4108,   // MP�ָ��ӿ죬���� ������/ÿ�룩
	CLASS_INCMPGEN       = 4109,   // HP�ָ��ӿ죬���� ������/ÿ�룩
	CLASS_INCHP          = 4110,   // ����HP���ֵ������
	CLASS_INCMP          = 4111,   // ����MP���ֵ������
	CLASS_INCATTACK      = 4112,   // ��ǿ������, ���ӱ���
	CLASS_INCDEFENCE     = 4113,   // ��ǿ������, ���ӱ���
	CLASS_ADDDODGE       = 4114,   // �����ǿ������
	CLASS_DODGE          = 4115,   // �Ṧ
	CLASS_MAGICSHIELD    = 4116,   // ħ����
	CLASS_DECMP          = 4117,   // ����MP����
	CLASS_CRAZY          = 4118,   // ��������
	CLASS_DECHURT        = 4119,   // �����˺�����
	CLASS_HPGEN          = 4120,   // �����ָ�HP
	CLASS_MPGEN          = 4121,   // �����ָ�MP
	CLASS_BAREHANDED     = 4122,   // ����
	CLASS_ADDHP          = 4123,   // ������Ѫ���޹̶�ֵ
	CLASS_ADDMP          = 4124,   // �����������޹̶�ֵ
	CLASS_ADDSPEED       = 4125,   // ����
	CLASS_ADDATTACK      = 4126,   // ���ӹ����̶�ֵ
	CLASS_ADDANTIDIZZY   = 4127,    
	CLASS_ADDANTISLEEP   = 4128,   
	CLASS_ADDANTIWEAK    = 4129,  
	CLASS_ADDANTISILENT  = 4130, 
	CLASS_ADDANTIWRAP    = 4131, 
	CLASS_ADDANTISLOW    = 4132, 
	CLASS_SUBANTIDIZZY   = 4133, 
	CLASS_SUBANTISLEEP   = 4134, 
	CLASS_SUBANTIWEAK    = 4135, 
	CLASS_SUBANTISILENT  = 4136, 
	CLASS_SUBANTIWRAP    = 4137, 
	CLASS_SUBANTISLOW    = 4138, 
	CLASS_ADDDEFENCE     = 4139, 
	CLASS_SUBDEFENCE     = 4140,    // ���ͷ����̶�ֵ
	CLASS_DECDAMAGE      = 4141,    // �����С�˺��ٷֱ�
	CLASS_BLOODSHIELD    = 4142,    // �ܹ���������ӷ���
	CLASS_DIET           = 4143,    // ��ʳ
	CLASS_IMMUNEDIZZY    = 4144,   // ���߻���
	CLASS_IMMUNESLEEP    = 4145,   // ����˯��
	CLASS_IMMUNESILENT   = 4146,   // ���߳�Ĭ
	CLASS_IMMUNEWEAK     = 4147,   // ��������
	CLASS_IMMUNEWRAP     = 4148,   // ���߲���
	CLASS_IMMUNESLOW     = 4149,   // ���߼���
	CLASS_INCHURT        = 4150,   // �˺����Ӱٷֱ�
	CLASS_BLESSED        = 4151,   // ���ߺ�������
	CLASS_CURSED         = 4152,   // �ܹ��������˺�
	CLASS_POWERUP        = 4153,   // ������ǿ��������
	CLASS_SHOUYI         = 4154,   // �������ӹ�������
	CLASS_NINGJIN        = 4155,   // 
	CLASS_ICON           = 4156,   // 
	CLASS_MPLEAK         = 4157,   // 
	CLASS_INCANTI        = 4158,   // 
	CLASS_ADDANTI        = 4159,   // 
	CLASS_DECANTI        = 4160,   // 
	CLASS_SUBANTI        = 4161,   // 
	CLASS_RETORT         = 4162,   // 
	CLASS_INCSKILLDAMAGE = 4163,   // 
	CLASS_DECSKILLDAMAGE = 4164,   // 
	CLASS_DRUNK          = 4165,   // 
	CLASS_SPICY          = 4166,   // 
	CLASS_SUBHP	     = 4167,   // 
	CLASS_SUBMP	     = 4168,   // 
	CLASS_FAMILYINCATTACK= 4169,   // 
	CLASS_FAMILYINCHP    = 4170,   // 
	CLASS_FAMILYINCMP    = 4171,   // 
	CLASS_CYCSUBDEFENCE  = 4172,   //
	CLASS_CYCSUBATTACK   = 4173,   //
	CLASS_CYCDECDEFENCE  = 4174,   //
	CLASS_CYCDECATTACK   = 4175,   //
	CLASS_DECCRITRATE    = 4176,   //
	CLASS_FRENZIED       = 4177,   // 
	CLASS_EVILAURA       = 4178,   // а��⻷
	CLASS_HOLYAURA       = 4179,   // ��ʥ�⻷
	CLASS_DECCRITHURT    = 4180,   //
	CLASS_ICESHIELD      = 4181,   //
	CLASS_FOCUSANTI      = 4182,   //
	CLASS_DISPERSEANTI   = 4183,   //
	CLASS_INCHEAL        = 4184,   //
	CLASS_DECACCURACY    = 4185,   //
	CLASS_ATTACK2HP      = 4186,   //
	CLASS_ATTACK2MP      = 4187,   //
	CLASS_HPLEAK1        = 4188,   //
	CLASS_HPLEAK2        = 4189,   //
	CLASS_HPLEAK3        = 4190,   //
	CLASS_BLEEDING       = 4191,   //
	CLASS_POWERLESS      = 4192,   //
	CLASS_ALOOF          = 4193,   //
	CLASS_DIZZYTIMER     = 4194,   //
	CLASS_SLEEPTIMER     = 4195,   //
	CLASS_WRAPTIMER      = 4196,   //
	CLASS_SILENTTIMER    = 4197,   //
	CLASS_GHOSTFORM      = 4198,   //
	CLASS_INSANITYFORM   = 4199,   //
	CLASS_DODGEREGAIN    = 4200,   //
	CLASS_HPREGAIN       = 4201,   //
	CLASS_DEADLYBLESS    = 4202,   //
	CLASS_HURTSCATTER    = 4203,   //
	CLASS_DEATHSCATTER   = 4204,   //
	CLASS_REJECTDEBUFF   = 4205,   //
	CLASS_INCSKILLACCU   = 4206,   //
	CLASS_DECSKILLACCU   = 4207,   //
	CLASS_INCSKILLDODGE  = 4208,   //
	CLASS_DECSKILLDODGE  = 4209,   //
	CLASS_INCFATALRATIO  = 4210,   //
	CLASS_DECFATALRATIO  = 4211,   //
	CLASS_INCFATALHURT   = 4212,   //
	CLASS_DECFATALHURT   = 4213,   //
	CLASS_EXPBOOST       = 4214,   //
	CLASS_CREDITBOOST    = 4215,   //
	CLASS_ADDATTACK2     = 4216,   // ����HP���ֵ������
	CLASS_YANYU		     = 4217,   //
	CLASS_HPLEAK4		 = 4218,
	CLASS_INCATTACK2	 = 4219,
	CLASS_HPLEAK5		 = 4220,
	CLASS_ADDHP2		 = 4221,
	CLASS_INCATTACK3	 = 4222,
	CLASS_INCHP2         = 4223,   // ����HP���ֵ������
	CLASS_HPLEAK6		 = 4224,
	CLASS_ZHAOQI		 = 4225,
	CLASS_FASHEN		 = 4226,
	CLASS_CIBEI			 = 4227,
	CLASS_INCDEFENCE2    = 4228,   // ��ǿ������, ���ӱ���
	CLASS_ADDATTACK3     = 4229,   // ����HP���ֵ������
	CLASS_TIANGU		 = 4230,   // ����HP���ֵ������
	CLASS_BANRUO		 = 4231,
	CLASS_XINMO			 = 4232,
	CLASS_AOSHI			 = 4233,
	CLASS_HUPO			 = 4234,
	CLASS_GUILIN		 = 4235,
	CLASS_ADDATTACK4	 = 4236,
	CLASS_ADDANTI2		 = 4237,
	CLASS_ADDDODGE2		 = 4238,
	CLASS_TONGXIN		 = 4239,
	CLASS_SLOW2 		 = 4240,
	CLASS_BAREXPBOOST    = 4241,   //
	CLASS_BARHPGEN	     = 4242,   //
	CLASS_BARMPGEN	     = 4243,   //
	CLASS_BARADDDOGEACCU = 4244,
	CLASS_CATCHPET		 = 4245,
	CLASS_VIPHUPO		 = 4246,
	CLASS_VIPGUILIN		 = 4247,
	CLASS_FEAR		 	 = 4248,
	CLASS_LONGXIANG	 	 = 4249,
	CLASS_HUJU		 	 = 4250,
	CLASS_DOUHUN		 = 4251,
	CLASS_GANGDAN		 = 4252,
	CLASS_ASWIND		 = 4253,
	CLASS_ASFOREST		 = 4254,
	CLASS_ASFIRE		 = 4255,
	CLASS_ASHILL		 = 4256,
	CLASS_MAKECRIT		 = 4257,
	CLASS_FOXSTATE		 = 4258,
	CLASS_CHIHUN		 = 4259,
	CLASS_INCCRITHURT    	 = 4260,   //
	CLASS_IMMUNETRANSFORM	 = 4261,
	CLASS_ADDANTITRANSFORM   = 4262,
	CLASS_SUBANTITRANSFORM   = 4263,
	CLASS_DIMMING			 = 4264,
	CLASS_SHADOWHIDE		 = 4265,
	CLASS_SHADOWDANCE		 = 4266,
	CLASS_POSEXCHANGEWITHCLONE 		= 4267,
	CLASS_STATEEXCHANGEWITHCLONE 	= 4268,
	CLASS_DRAWCLONELIFE		 = 4269,
	CLASS_SHARELIFEWITHCLONE = 4270,
	CLASS_SHARESTATEWITHCLONE= 4271,
	CLASS_BACKSTAB			 = 4272,
	CLASS_BLINK				 = 4273,
	CLASS_CRITCURSE			 = 4274,
	CLASS_INCDRUGEFFECT		 = 4275,
	CLASS_DESDRGUEFFECT		 = 4276,
	CLASS_SKILLMIRROR		 = 4277,
	CLASS_ADDSPOT			 = 4278,
	CLASS_SUBSPOT			 = 4279,
	CLASS_ADDHIDE			 = 4280,
	CLASS_SUBHIDE			 = 4281,
	CLASS_CRAZYCUESE		 = 4282,
	CLASS_FOGSTATE			 = 4283,

	CLASS_MIRACLEBURSTXIAN	 = 4284,
	CLASS_MIRACLEBURSTFO	 = 4285,
	CLASS_MIRACLEBURSTMO	 = 4286,
	CLASS_DARKNESS			 = 4287,
	CLASS_IGNITE			 = 4288,
	CLASS_SKILLREFLECT		 = 4289,
	CLASS_DIVINITYBURST		 = 4290,
	CLASS_DIVINITYFURY		 = 4291,
	CLASS_COLDINJURE		 = 4292,
	CLASS_FROZEN			 = 4293,
	CLASS_GOBACK			 = 4294,
	CLASS_SCOPEDAMAGE		 = 4295,
	CLASS_DAMAGEMOVE		 = 4296,
	CLASS_HUMANBOMB			 = 4297,

	CLASS_SPIRITDRAG		 = 4298,
	CLASS_FIRING			 = 4299,
	CLASS_CYCLE				 = 4300,
	CLASS_ADDWEAK			 = 4301,
	CLASS_ADDWRAP			 = 4302,
	CLASS_ADDCHARM			 = 4303,
	CLASS_ADDSLOW			 = 4304,
	CLASS_DELAYCAST			 = 4305,
	CLASS_TURNBUFF			 = 4306,
	CLASS_TURNDEBUFF		 = 4307,		

	CLASS_PARALYSIS			 = 4308,
	CLASS_SWIFT				 = 4309,
	CLASS_TRIGGERSKILL		 = 4310,
	CLASS_ACTIVEONFILTERADD	 = 4311,
	CLASS_CRIPPLED			 = 4312,
	CLASS_GUISHEN1			 = 4313,
	CLASS_TUOLING1			 = 4314,
	CLASS_PULL				 = 4315,
	CLASS_BLOODTHIRSTY		 = 4316,	
	CLASS_ACTIVECRIT		 = 4317,
	CLASS_ACTIVEBECRIT		 = 4318,

	CLASS_ADDCOMMON			 = 4319,
	CLASS_INCSKILLLEVEL		 = 4320,
	CLASS_SETSKILLLEVEL		 = 4321,
	CLASS_INCMOUNTSPEED		 = 4322,

	CLASS_GUISHEN2			 = 4323,
	CLASS_TUOLING2			 = 4324,
	CLASS_LIMITCYCLEAREA	 = 4325,
	CLASS_PERMITCYCLEAREA	 = 4326,
	CLASS_JAIL				 = 4327,

	CLASS_PULLING			 = 4328,
	CLASS_YUANLING			 = 4329,
	CLASS_BEFIRED			 = 4330,

	CLASS_GUISHEN1DEBUFF	 = 4331,
	CLASS_TUOLING1DEBUFF	 = 4332,
	CLASS_GUISHEN2DEBUFF	 = 4333,
	CLASS_TUOLING2DEBUFF	 = 4334,
	CLASS_CRIPPLEDDEBUFF	 = 4335,

	CLASS_PZMAXSPEED		 = 4336,
	CLASS_PZHALFSPEED		 = 4337,
	CLASS_PZHILL			 = 4338,
	CLASS_PZNOMOVE			 = 4339,
	CLASS_PZCHAOS			 = 4340,
	CLASS_PZVOID			 = 4341,

	CLASS_JUNIORDARKFORM 	 = 4342,
	CLASS_JUNIORLIGHTFORM	 = 4343,
	CLASS_SENIORDARKFORM	 = 4344,
	CLASS_SENIORLIGHTFORM	 = 4345,
	CLASS_DARKUNIFORM		 = 4346,
	CLASS_LIGHTUNIFORM		 = 4347,
	CLASS_DECHURT2			 = 4348,
	CLASS_RECORDPOS			 = 4349,
	CLASS_RETURNPOS			 = 4350,
	CLASS_FLY				 = 4351,
	CLASS_ACTIVEBEATTACKED	 = 4352,
	CLASS_MIRRORIMAGE		 = 4353,
	CLASS_INSTANTSKILL		 = 4354,
	CLASS_SUMMONSKILL		 = 4355,

	CLASS_BLOODTHIRSTY2		 = 4356,
	CLASS_TALISMANEFFECTBLESS= 4357,
	CLASS_DARKLASER			 = 4358,
	CLASS_AUTORESURRECT		 = 4359,
	CLASS_ADDSPIRIT			 = 4360,
	CLASS_DIRSTATE			 = 4361,
	CLASS_TEAMENCHANT		 = 4362,
	CLASS_GTHPGEN			 = 4363,
	CLASS_GTMPGEN			 = 4364,
	CLASS_GTADDATTACKDEFENSE = 4365,
	CLASS_SHOWYB			 = 4366,
	CLASS_RECOVERYHP		 = 4367,
	CLASS_EQUIPEFFECT		 = 4368,
	CLASS_SPEEDTO			 = 4369,
	CLASS_PUPPETFORM		 = 4370,
	CLASS_PUPPETSKILL		 = 4371,
	CLASS_SUBANTIDIZZY2		 = 4372,
	CLASS_SUBANTISLEEP2		 = 4373,
	CLASS_SUBANTIWEAK2		 = 4374,
	CLASS_SUBANTISILENT2		 = 4375,
	CLASS_SUBANTIWRAP2		 = 4376,
	CLASS_SUBANTISLOW2		 = 4377,
	CLASS_SUBHP2			 = 4378,
	CLASS_SUBMP2			 = 4379,
	CLASS_SEED			 = 4380,
	CLASS_QILINFORM			 = 4381,
	CLASS_RMHUMANFORM		 = 4382,
	CLASS_CYCLESKILL		 = 4383,
	CLASS_BUXIU		 	 = 4384,
	CLASS_BETAUNTED2		 = 4385,
	CLASS_ADDRAGE1			 = 4386,
	CLASS_ADDRAGE2			 = 4387,
	CLASS_RENMA1			 = 4388,
	CLASS_RENMA2			 = 4389,
	CLASS_RENMA3			 = 4390,
	CLASS_RENMA4			 = 4391,
	CLASS_SHARELIFEWITHSUMMON	 = 4392, 
	CLASS_ACCUMDAMAGE		 = 4393, 
	CLASS_BLOODPOOL			 = 4394,
	CLASS_GUIWANG_ADDMAXHP		 = 4395,
	CLASS_GUIWANG_ADDDAMAGE		 = 4396,
	CLASS_GUIWANG_ADDTENACIY	 = 4397,
	CLASS_GUIWANG_ADDSKILLATTACK	 = 4398,
	CLASS_GUIWANG_ADDANTICRIT	 = 4399,
	CLASS_DSLEEP          		 = 4400,   //���˯��
};

#define NORMAL_FILTER_BEGIN      4096
enum filter_id{
	FILTER_DIZZY           = 4096,   // ѣ��
	FILTER_SLEEP           = 4097,   // ˯��
	FILTER_SILENT          = 4098,   // ��Ĭ
	FILTER_WRAP            = 4099,   // ����
	FILTER_CRAZY           = 4100,   // ����Ѫħ��ǿ����
	FILTER_MAGICSHIELD     = 4101,   // ħ������
	FILTER_DECHURT         = 4102,   // ��������
	FILTER_INCCRITRATE     = 4103,   // ���ӱ�����
	FILTER_DECRESIST       = 4105,   // ���ͷ��������������ͱ��� �Ƿ�����id��? ly ask
	FILTER_DECMP           = 4106,   // ����MP����
	FILTER_BAREHANDED      = 4107,   // ����
	FILTER_DECDAMAGE       = 4108,   // �����С�˺��ٷֱ�
	FILTER_BLOODSHIELD     = 4109,   // �ܹ���������ӷ���
	FILTER_DIET            = 4111,   // ��ʳ
	FILTER_IMMUNEDIZZY     = 4112,   // ���߻���
	FILTER_IMMUNESLEEP     = 4113,   // ����˯��
	FILTER_IMMUNESILENT    = 4114,   // ���߳�Ĭ
	FILTER_IMMUNEWEAK      = 4115,   // ��������
	FILTER_IMMUNEWRAP      = 4116,   // ���߲���
	FILTER_IMMUNESLOW      = 4117,   // ���߼���
	FILTER_INCHURT         = 4118,   // �˺����Ӱٷֱ�
	FILTER_BLESSED         = 4119,   // ���ߺ�������
	FILTER_CURSED          = 4120,   // �ܹ��������˺�
	FILTER_POWERUP         = 4121,   // ������ǿ��������
	FILTER_SHOUYI          = 4122,   // �������ӹ�������
	FILTER_NINGJIN         = 4123,   // 
	FILTER_ICON            = 4124,   // 
	FILTER_INCANTI         = 4125,   // 
	FILTER_DECANTI         = 4126,   // 
	FILTER_ADDANTI         = 4127,   // 
	//FILTER_SUBANTI         = 4128,   //�ĳɿɵ���
	FILTER_RETORT          = 4129,   // ����
	//FILTER_INCSKILLDAMAGE  = 4130,   //�ĳɿɵ���
	//FILTER_DECSKILLDAMAGE  = 4131,   //�ĳɿɵ���
	FILTER_DRUNK           = 4132,   // 
	FILTER_SPICY           = 4133,   // 
	FILTER_FAMILYINCHP     = 4134,   // �������Ѫ���ްٷֱ� 
	FILTER_FAMILYINCMP     = 4135,   // ������������ްٷֱ�  
	FILTER_FAMILYINCATTACK = 4136,   // ����ӹ��ٷֱ�  
	FILTER_FRENZIED        = 4137,   // ���˺����ӹ��� 
	FILTER_DODGE           = 4140,   // �Ṧ
	FILTER_EVILAURA        = 4141,   // а��⻷
	FILTER_HOLYAURA        = 4142,   // ��ʥ�⻷
	FILTER_ICESHIELD       = 4143,   //
	FILTER_FOCUSANTI       = 4144,   //
	FILTER_DISPERSEANTI    = 4145,   //
	FILTER_ATTACK2HP       = 4146,   //
	FILTER_ATTACK2MP       = 4147,   //
	FILTER_POWERLESS       = 4148,   //
	FILTER_ALOOF           = 4149,   //
	FILTER_DIZZYTIMER      = 4150,   //
	FILTER_SLEEPTIMER      = 4151,   //
	FILTER_WRAPTIMER       = 4152,   //
	FILTER_SILENTTIMER     = 4153,   //
	FILTER_GHOSTFORM       = 4154,   //
	FILTER_INSANITYFORM    = 4155,   //
	FILTER_DODGEREGAIN     = 4156,   //
	FILTER_HPREGAIN        = 4157,   //
	FILTER_DEADLYBLESS     = 4158,   //
	FILTER_HURTSCATTER     = 4159,   //
	FILTER_DEATHSCATTER    = 4160,   //
	FILTER_REJECTDEBUFF    = 4161,   //
	FILTER_EXPBOOST        = 4162,   //
	FILTER_CREDITBOOST     = 4163,   //
	FILTER_ADDANTI2        = 4164,   // 
	FILTER_CATCHPET        = 4165,   //
	FILTER_FEAR			   = 4166,
	FILTER_LONGXIANG	   = 4167,
	FILTER_HUJU 		   = 4168,
	FILTER_DOUHUN		   = 4169,
	FILTER_GANGDAN		   = 4170,
	FILTER_ASWIND		   = 4171,
	FILTER_ASFOREST		   = 4172,
	FILTER_ASFIRE		   = 4173,
	FILTER_ASHILL		   = 4174,
	FILTER_MAKECRIT		   = 4175,
	FILTER_FOXSTATE		   = 4176,
	FILTER_IMMUNETRANSFORM     = 4177,   //��������
	
	FILTER_SHADOWHIDE		 	= 4178,
	FILTER_SHADOWDANCE		 	= 4179,
	FILTER_POSEXCHANGEWITHCLONE = 4180,
	FILTER_STATEEXCHANGEWITHCLONE 	= 4181,
	FILTER_DRAWCLONELIFE		 	= 4182,
	FILTER_SHARELIFEWITHCLONE 	= 4183,
	FILTER_SHARESTATEWITHCLONE	= 4184,
	FILTER_BACKSTAB			 	= 4185,
	FILTER_BLINK				= 4186,
	FILTER_CRITCURSE			= 4187,
	FILTER_SKILLMIRROR		 	= 4188,

	FILTER_CRAZYCURSE			= 4189,
	FILTER_FOGSTATE				= 4190,

	FILTER_DARKNESS				= 4191,
	FILTER_SKILLREFLECT			= 4192,
	FILTER_GOBACK				= 4193,
	FILTER_SCOPEDAMAGE			= 4194,
	FILTER_PULLBACK				= 4195,
	FILTER_HUMANBOMB			= 4196,

	//�������Ч��
	FILTER_SPIRITDRAG			= 4197,		//����ǣ��
	FILTER_CYCLE				= 4198,		//��
	FILTER_ADDWEAK				= 4199,		//��������
	FILTER_ADDWRAP				= 4200,		//���Ӷ���
	FILTER_ADDCHARM				= 4201, 	//�����Ȼ�
	FILTER_ADDSLOW				= 4202,		//���Ӽ���

	//̫껻���Ч��
	FILTER_PARALYSIS			= 4203,		//���
	FILTER_SWIFT				= 4204,		//Ѹ��
	FILTER_TRIGGERSKILL			= 4205, 	//��������
	//FILTER_ACTIVEONFILTERADD	= 4206,		//������״̬ʱ����
	FILTER_CRIPPLED				= 4207,		//�з�
	FILTER_GUISHEN1				= 4208,		//����1
	FILTER_TUOLING1				= 4209,		//����1
	FILTER_PULL					= 4210,		//����
	FILTER_ACTIVECRIT			= 4211,		//����Ŀ�꼤��
	FILTER_ACTIVEBECRIT			= 4212,		//����������

	FILTER_INCSKILLLEVEL		= 4213,		//���Ӽ��ܵȼ�
	FILTER_SETSKILLLEVEL		= 4214,		//ָ�����ܵȼ�
	FILTER_GUISHEN2				= 4215,		//����2
	FILTER_TUOLING2				= 4216,		//����2

	FILTER_LIMITCYCLEAREA	 	= 4217,		//��ֹ�ƶ�������
	FILTER_PERMITCYCLEAREA		= 4218,		//�����ƶ�������
	FILTER_JAIL					= 4219,

	FILTER_PULLING				= 4220,
	FILTER_YUANLING				= 4221,		//������Ԫ����
	FILTER_BEFIRED				= 4222,

	FILTER_GUISHEN1DEBUFF		= 4223,		//����1 Debuff
	FILTER_TUOLING1DEBUFF		= 4224,		//����1 Debuff
	FILTER_GUISHEN2DEBUFF		= 4225,		//����2 Debuff
	FILTER_TUOLING2DEBUFF		= 4226,		//����2 Debuff
	FILTER_CRIPPLEDDEBUFF		= 4227,		//�з�  Debuff

	FILTER_PZMAXSPEED		   	= 4228,
	FILTER_PZHALFSPEED		 	= 4229,
	FILTER_PZHILL				= 4230,
	FILTER_PZNOMOVE			 	= 4231,
	FILTER_PZCHAOS			 	= 4232,
	FILTER_PZVOID			 	= 4233,

	FILTER_JUNIORDARKFORM		= 4234,		//�͵Ȱ�ʹ
	FILTER_JUNIORLIGHTFORM		= 4235,		//�͵ȹ�ʹ
	FILTER_SENIORDARKFORM		= 4236,		//�ߵȰ�ʹ
	FILTER_SENIORLIGHTFORM		= 4237,		//�ߵȹ�ʹ
	FILTER_DARKUNIFORM			= 4238,		//�ⰵ��һ��
	FILTER_LIGHTUNIFORM			= 4239,		//�ⰵ��һ��
	FILTER_DECHURT2				= 4240,		//������II
	FILTER_RECORDPOS			= 4241,		//��¼λ��
	FILTER_RETURNPOS			= 4242,		//����λ��
	FILTER_FLY					= 4243,		//����
	FILTER_ACTIVEBEATTACKED		= 4244,		//����������
	FILTER_MIRRORIMAGE			= 4245,		//����
	FILTER_INSTANTSKILL			= 4246,		//ʹ����˲��
	FILTER_SUMMONSKILL			= 4247,		//ʹ���ٻ��޼���
	FILTER_BLOODTHIRSTY2		= 4248,		//��Ѫ2
	FILTER_TALISMANEFFECTBLESS	= 4249,		//�ͷŷ���Ч��
	FILTER_DARKLASER			= 4250,
	FILTER_AUTORESURRECT		= 4251,		//�����Զ�����
	FILTER_ADDSPIRIT			= 4252,		//������
	FILTER_DIRSTATE				= 4253,		//����״̬
	FILTER_TEAMENCHANT			= 4254,		//���鸴��
	FILTER_SHOWYB				= 4255,
	FILTER_RECOVERHP			= 4256,
	FILTER_EQUIPEFFECT			= 4257,
	FILTER_PUPPETFORM			= 4258,
	FILTER_PUPPETSKILL			= 4259,
	FILTER_SUBANTIDIZZY2			= 4260,
	FILTER_SUBANTISLEEP2			= 4261,
	FILTER_SUBANTIWEAK2			= 4262,
	FILTER_SUBANTISILENT2			= 4263,
	FILTER_SUBANTIWRAP2			= 4264,
	FILTER_SUBANTISLOW2			= 4265,
	FILTER_SUBHP2				= 4266,
	FILTER_SUBMP2				= 4266,
	FILTER_SEED				= 4267,
	FILTER_QILINFORM			= 4268,
	FILTER_RMHUMANFORM			= 4269,
	FILTER_CYCLESKILL			= 4270,
	FILTER_BUXIU				= 4271,
	FILTER_BETAUNTED2			= 4272,
	FILTER_ADDRAGE1				= 4273,
	FILTER_ADDRAGE2				= 4274,
	FILTER_RENMA1				= 4275,
	FILTER_RENMA2				= 4276,
	FILTER_RENMA3				= 4277,
	FILTER_RENMA4				= 4278,
	FILTER_SHARELIFEWITHSUMMON		= 4279,
	FILTER_ACCUMDAMAGE			= 4280,
	FILTER_BLOODPOOL			= 4281,
	FILTER_GUIWANG_ADDMAXHP			= 4282,
	FILTER_GUIWANG_ADDDAMAGE		= 4283,
	FILTER_GUIWANG_ADDTENACIY		= 4284,
	FILTER_GUIWANG_ADDSKILLATTACK		= 4285,
	FILTER_GUIWANG_ADDANTICRIT		= 4286,
	FILTER_DSLEEP				= 4287,

};

// �ɵ���״̬Ч��
#define MULTI_FILTER_BEGIN      4300
enum multi_filter_id{
	MULTI_FILTER_WEAK            = 1,   // ����
	MULTI_FILTER_SLOW            = 2,   // ����
	MULTI_FILTER_ADDATTACK       = 3,   // ���ӹ�����
	MULTI_FILTER_ADDDEFENCE      = 4,   // ���ӷ�����
	MULTI_FILTER_DECATTACK       = 5,
	MULTI_FILTER_INCHP           = 7,   // ������������
	MULTI_FILTER_INCMP           = 8,   // ������������
	MULTI_FILTER_ADDHP           = 9,   // ������������
	MULTI_FILTER_ADDMP           = 10,   // ������������
	MULTI_FILTER_ADDSPEED        = 11,   // ����
	MULTI_FILTER_ADDDODGE        = 12,   // ���Ӷ���
	MULTI_FILTER_INCHPGEN        = 13,   // ���������ָ��ٶ�
	MULTI_FILTER_HPLEAK          = 14,   // ����������
	MULTI_FILTER_MPLEAK          = 15,   // ����������
	MULTI_FILTER_HPGEN           = 16,   // �����ָ�����
	MULTI_FILTER_MPGEN           = 17,   // �����ָ�����
	MULTI_FILTER_ADDANTIDIZZY    = 18,   // ���ӱ�����
	MULTI_FILTER_ADDANTISLEEP    = 19,   // ���ӱ�����
	MULTI_FILTER_ADDANTIWEAK     = 20,   // ���ӱ�����
	MULTI_FILTER_ADDANTISILENT   = 21,   // ���ӱ�����
	MULTI_FILTER_ADDANTIWRAP     = 22,   // ���ӱ�����
	MULTI_FILTER_ADDANTISLOW     = 23,   // ���ӱ�����
	MULTI_FILTER_SUBANTIDIZZY    = 24,   // ���ӱ�����
	MULTI_FILTER_SUBANTISLEEP    = 25,   // ���ӱ�����
	MULTI_FILTER_SUBANTIWEAK     = 26,   // ���ӱ�����
	MULTI_FILTER_SUBANTISILENT   = 27,   // ���ӱ�����
	MULTI_FILTER_SUBANTIWRAP     = 28,   // ���ӱ�����
	MULTI_FILTER_SUBANTISLOW     = 29,   // ���ӱ�����
	MULTI_FILTER_INCMPGEN        = 30,   // ���ӱ�����
	MULTI_FILTER_SUBHP           = 32,   // ����hp���޹̶�ֵ
	MULTI_FILTER_SUBMP           = 33,   // ����mp���޹̶�ֵ
	MULTI_FILTER_CYCSUBDEFENCE   = 34,
	MULTI_FILTER_CYCSUBATTACK    = 35,
	MULTI_FILTER_CYCDECDEFENCE   = 36,
	MULTI_FILTER_CYCDECATTACK    = 37,
	MULTI_FILTER_DECCRITRATE     = 38,   // ����Ŀ�걬����
	MULTI_FILTER_DECDODGE	     = 39,   // ����Ŀ������ٷֱ� 
	MULTI_FILTER_DECDEFENCE      = 40,   // ���������������ͱ���
	MULTI_FILTER_SUBDEFENCE      = 41,   // ���ͷ����̶�ֵ
	MULTI_FILTER_INCDEFENCE      = 42,   // ��ǿ������
	MULTI_FILTER_INCATTACK       = 43,   // ��ǿ������
	MULTI_FILTER_DECCRITHURT     = 44,   // ����Ŀ�걬����
	MULTI_FILTER_DECACCURACY     = 45,
	MULTI_FILTER_INCHEAL         = 46,
	MULTI_FILTER_SUBATTACK       = 47,   // ���͹�����
	MULTI_FILTER_HPLEAK1         = 48, 
	MULTI_FILTER_HPLEAK2         = 49, 
	MULTI_FILTER_HPLEAK3         = 50,
	MULTI_FILTER_BLEEDING        = 51,
	MULTI_FILTER_INCSKILLACCU    = 52,   //
	MULTI_FILTER_DECSKILLACCU    = 53,   //
	MULTI_FILTER_INCSKILLDODGE   = 54,   //
	MULTI_FILTER_DECSKILLDODGE   = 55,   //
	MULTI_FILTER_INCFATALRATIO   = 56,   //
	MULTI_FILTER_DECFATALRATIO   = 57,   //
	MULTI_FILTER_INCFATALHURT    = 58,   //
	MULTI_FILTER_DECFATALHURT    = 59,   //
	MULTI_FILTER_ADDATTACK2      = 60,   // ������������
	MULTI_FILTER_YANYU      	 = 61,   // ������������
	MULTI_FILTER_HPLEAK4		 = 62,
	MULTI_FILTER_INCATTACK2		 = 63,
	MULTI_FILTER_HPLEAK5		 = 64,
	MULTI_FILTER_ADDHP2		 	 = 65,
	MULTI_FILTER_INCATTACK3		 = 66,
	MULTI_FILTER_INCHP2          = 67,   // ������������
	MULTI_FILTER_HPLEAK6		 = 68,
	MULTI_FILTER_ZHAOQI			 = 69,
	MULTI_FILTER_FASHEN			 = 70,
	MULTI_FILTER_CIBEI			 = 71,
	MULTI_FILTER_INCDEFENCE2     = 72,   // ��ǿ������
	MULTI_FILTER_ADDATTACK3      = 73,   // ���ӹ�����
	MULTI_FILTER_TIANGU		     = 74,   // ���ӹ�����
	MULTI_FILTER_BANRUO		     = 75,   // ���ӹ�����
	MULTI_FILTER_XINMO		     = 76,   // ���ӹ�����
	MULTI_FILTER_AOSHI		     = 77,   // ���ӹ�����
	MULTI_FILTER_HUPO		     = 78,   // ���ӹ�����
	MULTI_FILTER_GUILIN		     = 79,   // ���ӹ�����
	MULTI_FILTER_ADDATTACK4	     = 80,   // ���ӹ�����
	MULTI_FILTER_ADDDODGE2	     = 81,   // ���ӹ�����
	MULTI_FILTER_TONGXIN	     = 82,   // ���ӹ�����
	MULTI_FILTER_SLOW2           = 83,   // ����
	MULTI_FILTER_BARHPGEN		 = 84, 
	MULTI_FILTER_BARMPGEN        = 85,   // �����ָ�����
	MULTI_FILTER_BARADDDOGEACCU  = 86,
	MULTI_FILTER_BAREXPBOOST     = 87,   //
	MULTI_FILTER_VIPHUPO	     = 88,   // ���ӹ�����
	MULTI_FILTER_VIPGUILIN	     = 89,   // ���ӹ�����
	MULTI_FILTER_CHIHUN			 = 90,
	MULTI_FILTER_INCCRITHURT	 = 91,
	MULTI_FILTER_ADDANTITRANSFORM	= 92,
	MULTI_FILTER_SUBANTITRANSFORM	= 93,
	MULTI_FILTER_SUBANTI   		= 94,
	MULTI_FILTER_DIMMING		= 95,
	MULTI_FILTER_INCDRUGEFFECT	= 96,
	MULTI_FILTER_DECDRUGEFFECT	= 97,
	MULTI_FILTER_INCSKILLDAMAGE = 98,
	MULTI_FILTER_DECSKILLDAMAGE = 99, 
	MULTI_FILTER_ADDSPOT 		= 100,
	MULTI_FILTER_SUBSPOT		= 101,
	MULTI_FILTER_ADDHIDE		= 102,
	MULTI_FILTER_SUBHIDE		= 103,
	
	MULTI_FILTER_MIRACLEBURSTXIAN = 104,
	MULTI_FILTER_MIRACLEBURSTFO	  = 105,
	MULTI_FILTER_MIRACLEBURSTMO	  = 106,
	MULTI_FILTER_IGNITE			= 107,
	MULTI_FILTER_DIVINITYBURST	= 108,
	MULTI_FILTER_DIVINITYFURY	= 109,
	MULTI_FILTER_COLDINJURE		= 110,
	MULTI_FILTER_FROZEN			= 111,
	MULTI_FILTER_DAMAGEMOVE		= 112,

	MULTI_FILTER_FIRING			= 113, //����
	MULTI_FILTER_DELAYCAST		= 114, //�ӳ���Ч����
	MULTI_FILTER_TURNBUFF		= 115, //��ת����
	MULTI_FILTER_TURNDEBUFF		= 116, //��ת����
	MULTI_FILTER_BLOODTHIRSTY	= 117, //��Ѫ

	MULTI_FILTER_ADDCOMMON		= 118, //����ͨ��
	MULTI_FILTER_INCMOUNTSPEED	= 119, //��������ٶ�
	MULTI_FILTER_ACTIVEONFILTERADD	= 120,//������״̬ʱ����
	MULTI_FILTER_GTHPGEN		= 121,
	MULTI_FILTER_GTMPGEN		= 122,
	MULTI_FILTER_GTADDATTACKDEFENSE = 123,
	MULTI_FILTER_SPEEDTO		= 124,
};

enum filter_modifier{
	FMID_ANTIDOTE 		= 1,              // �ⶾһ��
	FMID_COMPLETE_TRAVEL= 2,       //��������
	FMID_SPEEDUP_FLY 	= 3,
	FMID_NORMAL_FLY 	= 4,  
	FMID_DUEL_END 		= 5,    
	FMID_CLEAR_AEBF 	= 6,
	FMID_PET_HONOR    	= 7,
	FMID_ADD_AREA     	= 8,
	FMID_REMOVE_AREA  	= 9,
	FMID_TRIGGER_SKILL	= 10,
	FMID_CLEANCOOLTIME 	= 11,
	FMID_OVERLAY 	  	= 12,
	FMID_OVERLAY_CNT  	= 13,
	FMID_BUFFAREA 	  	= 14,
	FMID_GET_BLOODCAPA	= 15,
	FMID_FILL_BLOODPOOL	= 16,
	FMID_SKILLMAX 		= 1024, 
};
#endif