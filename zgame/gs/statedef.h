#ifndef __SKILL_STATEDEF_H__
#define __SKILL_STATEDEF_H__

namespace GNET
{
enum visible_state{
	// ͨ��״̬
	VSTATE_DECHURT       = 1,  // ������
	VSTATE_MAGICSHIELD   = 2,  // ħ����
	VSTATE_SLOW          = 3,  // ����
	VSTATE_WRAP          = 4,  // 
	VSTATE_BLESSED       = 5,  // 
	VSTATE_SLEEP         = 6,  // 
	VSTATE_SILENT        = 7,  // 
	VSTATE_HPLEAK        = 8,  // 
	VSTATE_HPGEN         = 9,  // 
	VSTATE_INVINCIBLE    = 10, // 
	VSTATE_WEAK          = 11, // 
	VSTATE_MPLEAK        = 12, // 
	VSTATE_MPGEN         = 13, // 
	VSTATE_DIZZY         = 14, // 
	VSTATE_TASK          = 15, // 15-16
	VSTATE_ICESHIELD     = 17, 
	VSTATE_UNMOVED       = 18, 
	VSTATE_SECRET        = 19, 
	VSTATE_DISASTER      = 20, 
	VSTATE_GRACE         = 21, 
	VSTATE_CATCHPET		 = 25,
	VSTATE_FEAR			 = 26,

	VSTATE_ABSOLUTEAREA	 = 27, //����Ч��

	VSTATE_MIRACLEBURSTXIAN = 33, //�񼣱�����
	VSTATE_MIRACLEBURSTFO = 34, //�񼣱�����
	VSTATE_MIRACLEBURSTMO = 35, //�񼣱���ħ
	VSTATE_DARKNESS		 = 36, //�ڰ���
	VSTATE_IGNITE		 = 37, //ȼ��
	VSTATE_FROZEN		 = 38, //����Ч��
	VSTATE_COLDINJURE	 = 39, //����Ч��
	VSTATE_DAMAGEMOVE	 = 40, //λ���˺�
	VSTATE_TURNBUFF		 = 41, //��ת����Ч��
	VSTATE_TURNDEBUFF	 = 42, //��ת����Ч��
	VSTATE_ADDWEAK		 = 43, //��������
	VSTATE_ADDWRAP		 = 44, //���Ӷ���
	VSTATE_ADDCHARM		 = 45, //�����Ȼ�
	VSTATE_ADDSLOW		 = 46, //���Ӽ���
	VSTATE_ADDCOMMON	 = 47, //����ͨ��
	VSTATE_FIRING		 = 48, //��������
	VSTATE_HUOZHU		 = 49, //��������
	VSTATE_DECHURT2 	 = 50, //����2
	VSTATE_SENIORDARK	 = 51, //�ߵȰ�ʹ
	VSTATE_SENIORLIGHT	 = 52, //�ߵȹ�ʹ
	//VSTATE_IMAGEMIRROR	 = 53, //����
	VSTATE_SHOWYB	 	 = 54, //����
	VSTATE_RECOVERYHP	 = 55, //�ָ���Ѫ
};

enum hidden_state{
	HSTATE_DIZZY         = 1,    // ѣ��
	HSTATE_SLEEP         = 2,    // ˯��
	HSTATE_WEAK          = 3,    // ����
	HSTATE_SILENT        = 4,    // ��Ĭ
	HSTATE_WRAP          = 5,    // ����
	HSTATE_SLOW          = 6,    // �ٻ�
	HSTATE_ADDATTACK     = 7,    // ���ӹ���
	HSTATE_ADDDEFENCE    = 8,    // ���ӷ���
	HSTATE_INCATTACK     = 9,    // ��ȭ
	HSTATE_INCDEFENCE    = 10,   // ӲƤ

	HSTATE_ADDANTIDIZZY  = 11,   // ���ӻ��Կ���
	HSTATE_ADDANTISLEEP  = 12,   // 
	HSTATE_ADDANTIWEAK   = 13,   // ������������
	HSTATE_ADDANTISILENT = 14,   // ���ӳ�Ĭ����
	HSTATE_ADDANTIWRAP   = 15,   // ���Ӳ��ƿ���

	HSTATE_SUBANTIDIZZY  = 16,   // ���ͻ��Կ���
	HSTATE_SUBANTISLEEP  = 17,   // ����˯�߿���
	HSTATE_SUBANTIWEAK   = 18,   // ������������
	HSTATE_SUBANTISILENT = 19,   // ���ͳ�Ĭ����
	HSTATE_SUBANTIWRAP   = 20,   // ���Ͳ��ƿ���

	HSTATE_INCHP          = 21,   // ����HP����
	HSTATE_INCMP          = 22,   // ����MP����
	HSTATE_ADDHP          = 23,   // ����HP����
	HSTATE_ADDMP          = 24,   // ����HP����
	HSTATE_ADDSPEED       = 25,   // ����
	HSTATE_ADDDODGE       = 26,   // ����
	HSTATE_INCHPGEN       = 27,   // �����ָ��ӿ�
	HSTATE_CRAZY          = 28,   // ����ȼ��
	HSTATE_HPLEAK         = 29,   // �ж�
	HSTATE_MPLEAK         = 30,   // �ж�
	HSTATE_HPGEN          = 31,   // �ָ�����
	HSTATE_MPGEN          = 32,   // �ָ�ħ��
	HSTATE_INCMPGEN       = 33,   // ħ���ָ��ӿ�
	HSTATE_SUBDEFENCE     = 34,   // ���ͷ���
	HSTATE_DECDEFENCE     = 35,   // ���ͷ����ٷֱ�
	HSTATE_IMMUNEDIZZY    = 36,   // ���߻���
	HSTATE_IMMUNESLEEP    = 37,   // ���߻���
	HSTATE_IMMUNESILENT   = 38,   // ���߻���
	HSTATE_IMMUNEWEAK     = 39,   // ���߻���
	HSTATE_IMMUNEWRAP     = 40,   // ���߲���
	HSTATE_IMMUNESLOW     = 41,   // ���߻���
	HSTATE_CURSED         = 42,   // �ܹ����ۼ��˺�
	HSTATE_POWERUP        = 43,   // ������ǿ��������
	HSTATE_BLOODSHIELD    = 44,   // ���˺����ӷ���
	HSTATE_BLESSED        = 45,   // 
	HSTATE_SHOUYI         = 46,   // ���������̶�ֵ
	HSTATE_INVINCIBLE     = 47,   // �޵�
	HSTATE_DRUNK          = 48,   // ���
	HSTATE_SPICY          = 49,   // ��

	HSTATE_SUBHP	       = 50,   // ����hp���޹̶�ֵ 
	HSTATE_SUBMP	       = 51,   // ����mp���޹̶�ֵ
	HSTATE_SUBATTACK       = 52,   // ���ٹ����̶�ֵ 
	HSTATE_DECCRITRATE     = 53,   // ����Ŀ�걬���� 
	HSTATE_DECDODGE        = 54,   // ���Ͷ����ٷֱ�
	HSTATE_FRENZIED        = 55,   // ���˺����ӹ�����
	HSTATE_INCHURT         = 56,   // �˺����Ӱٷֱ�
	HSTATE_NINGJIN         = 57,   // ����֮��
	HSTATE_INCANTI         = 58,   // �������п��԰ٷֱ�
	HSTATE_ADDANTI         = 59,   // �������п��Թ̶�ֵ
	HSTATE_DECANTI         = 60,   // �������п��԰ٷֱ�
	HSTATE_SUBANTI         = 61,   // �������п��Թ̶�ֵ
	HSTATE_INCSKILLDAMAGE  = 62,   // �����˺����Ӱٷֱ�
	HSTATE_DECSKILLDAMAGE  = 63,   // �����˺����ٰٷֱ�
	HSTATE_RETORT          = 64,   // ����
	HSTATE_INCCRITHURT     = 65,   
	HSTATE_HPSUCK          = 66,  
	HSTATE_MPSUCK          = 67, 
	HSTATE_INCHEAL         = 68, 
	HSTATE_CYCSUBDEFENCE   = 69, 
	HSTATE_CYCSUBATTACK    = 70, 
	HSTATE_CYCDECDEFENCE   = 71, 
	HSTATE_CYCDECATTACK    = 72, 
	HSTATE_FAMILYINCHP     = 73, 
	HSTATE_FAMILYINCATTACK = 74, 
	HSTATE_FAMILYINCMP     = 75, 
	HSTATE_DECACCURACY     = 76, 
	HSTATE_DECCRITHURT     = 77,
	HSTATE_ICESHIELD       = 78,
	HSTATE_FOCUSANTI       = 79,
	HSTATE_DISPERSEANTI    = 80,
	HSTATE_BAREHANDED      = 81,
	HSTATE_MAGICSHIELD     = 82,
	HSTATE_DECHURT         = 83,
	HSTATE_DECDAMAGE       = 84,
	HSTATE_DIET            = 85,
	HSTATE_INCCRITRATE     = 86,
	HSTATE_POWERLESS       = 87,
	HSTATE_ALOOF           = 88,
	HSTATE_BLEEDING        = 89,
	HSTATE_DIZZYTIMER      = 90,
	HSTATE_WRAPTIMER       = 91,
	HSTATE_SILENTTIMER     = 92,
	HSTATE_SLEEPTIMER      = 93,
	HSTATE_HPLEAK1         = 94,
	HSTATE_HPLEAK2         = 95,
	HSTATE_HPLEAK3         = 96,
	HSTATE_GHOSTFORM       = 97,
	HSTATE_INSANITYFORM    = 98,
	HSTATE_DODGEREGAIN     = 99,
	HSTATE_HPREGAIN        = 100,
	HSTATE_DEADLYBLESS     = 101,
	HSTATE_HURTSCATTER     = 102,
	HSTATE_DEATHSCATTER    = 103,
	HSTATE_INCSKILLACCU    = 104,
	HSTATE_DECSKILLACCU    = 105,
	HSTATE_INCSKILLDODGE   = 106,
	HSTATE_DECSKILLDODGE   = 107,
	HSTATE_INCFATALRATIO   = 108,
	HSTATE_DECFATALRATIO   = 109,
	HSTATE_INCFATALHURT    = 110,
	HSTATE_DECFATALHURT    = 111,
	HSTATE_EXPBOOST        = 112,
	HSTATE_CREDITBOOST     = 113,
	HSTATE_REJECTDEBUFF    = 114,
	HSTATE_ADDATTACK2	   = 115,
	HSTATE_YANYU           = 116,
	HSTATE_HPLEAK4	       = 117,
	HSTATE_INCATTACK2	   = 118,
	HSTATE_HPLEAK5	   	   = 119,
	HSTATE_ADDHP2	   	   = 120,
	HSTATE_INCATTACK3	   = 121,
	HSTATE_INCHP2	       = 122,
	HSTATE_HPLEAK6	   	   = 123,
	HSTATE_ZHAOQI	   	   = 124,
	HSTATE_FASHEN	   	   = 125,
	HSTATE_CIBEI	   	   = 126,
	HSTATE_INCDEFENCE2	   = 127,
	HSTATE_ADDATTACK3	   = 128,
	HSTATE_TIANGU	   	   = 129,
	HSTATE_BANRUO	   	   = 130,
	HSTATE_XINMO	   	   = 131,
	HSTATE_AOSHI	   	   = 132,
	HSTATE_HUPO	   		   = 133,
	HSTATE_GUILIN	   	   = 134,
	HSTATE_ADDATTACK4	   = 135,
	HSTATE_ADDANTI2	   	   = 136,
	HSTATE_ADDDODGE2	   = 137,
	HSTATE_TONGXIN	   	   = 138,
	HSTATE_SLOW2           = 139,    // �ٻ�
	HSTATE_BAREXPBOOST     = 140,
	HSTATE_BARHPGEN        = 141,   // �ָ�����
	HSTATE_BARMPGEN        = 142,   // �ָ�����
	HSTATE_BARADDDOGEACCU  = 143,
	HSTATE_VIPHUPO	   	   = 144,
	HSTATE_VIPGUILIN	   = 145,
	HSTATE_FEAR			   = 146,
	HSTATE_LONGXIANG	   = 147,
	HSTATE_HUJU			   = 148,
	HSTATE_DOUHUN		   = 149,
	HSTATE_GANGDAN		   = 150,
	HSTATE_ASWIND		   = 151,
	HSTATE_ASFOREST		   = 152,
	HSTATE_ASFIRE		   = 153,
	HSTATE_ASHILL		   = 154,
	HSTATE_MAKECRIT		   = 155,
	HSTATE_FOXSTATE		   = 156,
	HSTATE_CHANGEFORM	   = 157,	// ����gs�����ʱ��,����Ӧfilter
	HSTATE_CHIHUN		   = 158,
	HSTATE_EVILAURA		   = 159,
	HSTATE_HOLYAURA		   = 160,
	HSTATE_TRANSFORM	   = 161,       //���ܱ���
	HSTATE_TRANSFORM_TASK	   	= 162,       //�������
	HSTATE_IMMUNETRANSFORM     	= 163,       //��������
	HSTATE_ADDANTITRANSFORM    	= 164,       //���ӱ�����
	HSTATE_SUBANTITRANSFORM    	= 165,       //���ͱ�����
	HSTATE_DIMMING		   	  	= 166,	//���� 
	HSTATE_SHADOWHIDE	   		= 167,   //Ӱ��
	HSTATE_SHADOWDANCE	   		= 168,	//Ӱ��
	HSTATE_SHARELIFEWITHCLONE  	= 169,	//�����������
	HSTATE_SHARESTATEWITHCLONE 	= 170,	//�������״̬
	HSTATE_BACKSTAB			   	= 171,	//����
	HSTATE_CRITCURSE		   	= 172, 	//��������
	HSTATE_INCDRUGEFFECT	   	= 173, 	//��ǿ��ҩЧ��
	HSTATE_DECDRUGEFFECT	   	= 174,	//������ҩЧ��
	HSTATE_SKILLMIRROR		   	= 175,  //���ܾ�
	HSTATE_DRAWCLONELIFE		= 176, 	//��ȡ����������������������ֵ
	HSTATE_ABSOLUTEAREA			= 177, 	//����
	HSTATE_ADDSPOT				= 178, 	//���ӷ�������
	HSTATE_SUBSPOT				= 179,  //���ͷ�������
	HSTATE_ADDHIDE				= 180,  //������������
	HSTATE_SUBHIDE				= 181, 	//������������
	HSTATE_FOGSTATE				= 182,  //���ж�
	
	HSTATE_MIRACLEBURSTXIAN		= 183,	//�񼣱�����
	HSTATE_MIRACLEBURSTFO		= 184,	//�񼣱�����
	HSTATE_MIRACLEBURSTMO		= 185,	//�񼣱���ħ
	HSTATE_DARKNESS				= 186,	//�ڰ���
	HSTATE_IGNITE				= 187,	//ȼ��
	HSTATE_SKILLREFLECT			= 188,	//���ܷ���
	HSTATE_DIVINITYBURST		= 189,	//��ʥ����
	HSTATE_DIVINITYFURY			= 190,	//��ʥ֮ŭ
	HSTATE_COLDINJURE			= 191,	//����
	HSTATE_FROZEN				= 192,	//������
	HSTATE_GOBACK				= 193,	//����ԭ����λ��
	HSTATE_SCOPEDAMAGE			= 194,	//��Χ�����������˺�
	HSTATE_PULLBACK				= 195,	//����λ�������
	HSTATE_DAMAGEMOVE			= 196,	//λ���˺�
	HSTATE_HUMANBOMB			= 197,	//����ը��
	HSTATE_DPGEN				= 198,  //�ָ�����

	HSTATE_SPIRITDRAG			= 199,	//����ǣ��	
	HSTATE_FIRING				= 200, 	//����
	HSTATE_DELAYCAST			= 201,	//�ӳ���Ч
	HSTATE_TURNBUFF				= 202,  //��ת����
	HSTATE_TURNDEBUFF			= 203, 	//��ת����
	HSTATE_ADDWEAK				= 204,	//��������
	HSTATE_ADDWRAP				= 205,  //���Ӳ���
	HSTATE_ADDCHARM				= 206,	//�����Ȼ�
	HSTATE_ADDSLOW				= 207,	//���Ӽ���
	HSTATE_PARALYSIS			= 208,	//���
	HSTATE_SWIFT				= 209,	//Ѹ��
	HSTATE_TRIGGERSKILL			= 210,	//��������
	HSTATE_ACTIVE				= 211,	//����
	HSTATE_CRIPPLED				= 212, 	//�з�
	HSTATE_GUISHEN1				= 213,	//����1
	HSTATE_TUOLING1				= 214,  //����1
	HSTATE_PULL					= 215,  //����
	HSTATE_BLOODTHIRSTY			= 216,	//��Ѫ
	HSTATE_CYCLE				= 217,	//��
	HSTATE_ADDCOMMON			= 218,	//������ͨ
	HSTATE_INCSKILLLEVEL		= 219,	//���Ӽ��ܵȼ�
	HSTATE_SETSKILLLEVEL		= 220,	//�趨���ܵȼ�
	HSTATE_INCMOUNTSPEED		= 221, 	//��������ٶ�
	HSTATE_GUISHEN2				= 222,	//����2
	HSTATE_TUOLING2				= 223,  //����2
	HSTATE_JAILPERMITCYCLEAREA	= 224,
	HSTATE_JAILLIMITCYCLEAREA	= 225,
	HSTATE_PULLING				= 226,
	HSTATE_YUANLING				= 227,

	HSTATE_JUNIORDARK			= 228,
	HSTATE_JUNIORLIGHT			= 229,
	HSTATE_SENIORDARK			= 230,
	HSTATE_SENIORLIGHT			= 231,
	HSTATE_DARKUNIFORM			= 232,
	HSTATE_LIGHTUNIFORM			= 233,
	
	HSTATE_DECHURT2				= 234,
	HSTATE_RECORDPOS			= 235,
	HSTATE_FLY					= 236,
	HSTATE_MIRRORIMAGE			= 237,
	HSTATE_INSTANTSKILL			= 238,
	HSTATE_PZHILL				= 239,
	HSTATE_PZNOMOVE				= 240,
	HSTATE_PZCHAOS				= 241,
	HSTATE_PZVOID				= 242,
	HSTATE_BATTLEFLAG			= 243,
	HSTATE_AUTORES			    = 244,
	HSTATE_GTHPGEN				= 245,
	HSTATE_GTMPGEN				= 246,
	HSTATE_GTADDATTACKDEFENSE 	= 247,
	HSTATE_RECOVERHP			= 248,
	HSTATE_EQUIPEFFECT			= 249,
	HSTATE_KINDOMEXP            = 250,
	HSTATE_SPEEDTO		    = 251,
	HSTATE_PUPPETFORM		    = 252,
	HSTATE_PUPPETSKILL		= 253,
	HSTATE_SUBANTIDIZZY2		= 254,
	HSTATE_SUBANTISLEEP2		= 255,
	HSTATE_SUBANTIWEAK2		= 256,
	HSTATE_SUBANTISILENT2		= 257,
	HSTATE_SUBANTIWRAP2		= 258,
	HSTATE_SUBANTISLOW2		= 259,
	HSTATE_SUBHP2			= 260,
	HSTATE_SUBMP2			= 261,
	HSTATE_SEED			= 262,
	HSTATE_QILINFORM		= 263,
	HSTATE_RMHUMANFORM		= 264,
	HSTATE_CYCLESKILL		= 265,
	HSTATE_BUXIU			= 266,
	HSTATE_BETAUNTED2		= 267,
	HSTATE_ADDRAGE1			= 268,
	HSTATE_ADDRAGE2			= 269,
	HSTATE_SHARELIFEWITHSUMMON	= 270,
	HSTATE_PET_COMBINE1		= 271,
	HSTATE_PET_COMBINE2		= 272,
	HSTATE_ACCUMDAMAGE		= 273,
	HSTATE_BLOODPOOL		= 274,
	HSTATE_COD_COOLDOWN		= 275,
	HSTATE_GUIWANG_ADDMAXHP		= 276,
	HSTATE_GUIWANG_ADDDAMAGE	= 277,
	HSTATE_GUIWANG_ADDTENACIY	= 278,
	HSTATE_GUIWANG_ADDSKILLATTACK	= 279,
	HSTATE_GUIWANG_ADDANTICRIT	= 280,
	HSTATE_DSLEEP      	 	= 281,    // ���˯��

	
	
	HSTATE_SYSTEMSTART     		= 1000,
};

#define IMMUNEOVERAWE      0x00000001   //��������
#define IMMUNEBLIND        0x00000002   //������ä
#define IMMUNEREPEL        0x00000004   //���߻���
#define IMMUNEDRAINMAGIC   0x00000008   //��������
#define IMMUNEDECDEFENCE   0x00000010   //���߻���
#define IMMUNESUBDEFENCE   0x00000020   //���߻���
#define IMMUNEDIZZY        0x00000040   //���߻���
#define IMMUNEWEAK         0x00000080   //��������
#define IMMUNESLOW         0x00000100   //���߻���
#define IMMUNESILENT       0x00000200   //���߳�Ĭ
#define IMMUNESLEEP        0x00000400   //����˯��
#define IMMUNEWRAP         0x00000800   //���߲���
#define IMMUNEDIET         0x00001000   //���߲���
#define IMMUNEBAREHANDED   0x00002000   //�����˺��ӱ�
#define IMMUNEHPLEAK       0x00004000   //���߳����˺�
#define IMMUNETAUNT        0x00008000   //���߳���
#define IMMUNEMPDISPERSE   0x00010000   //����Ԫ���ɢ
#define IMMUNEMPLEAK	   0x00020000	//������ħ
#define IMMUNETRANSFORM	   0x00040000	//���߱���
#define IMMUNEFIRING       0x00080000   //��������
#define IMMUNEIGNITE	   0x00100000	//����Ѫ��
#define IMMUNEDECDEFSCALE  0x00200000	//���߽��ͷ����ٷֱ�

//����Ϊ�������У�����Ϊ����ģ�涨�������
#define IMMUNEFROZE 	   0x20000000   //���߱�����
#define IMMUNEPULL	   0x40000000   //�������� ���ε�
#define IMMUNEDAMAGE	   0x80000000   //�����˺�

#define SKILL_IMMUNEATTACK 0x80

}

#endif
