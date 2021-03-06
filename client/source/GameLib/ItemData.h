#pragma once

// NOTE : Item의 통합 관리 클래스다.
//        Icon, Model (droped on ground), Game Data

#include "../eterLib/GrpSubImage.h"
#include "../eterGrnLib/Thing.h"

class CItemData
{
	public:
		enum
		{
			ITEM_NAME_MAX_LEN = 24,
			ITEM_LIMIT_MAX_NUM = 2,
			ITEM_VALUES_MAX_NUM = 6,
			ITEM_SMALL_DESCR_MAX_LEN = 256,
			ITEM_APPLY_MAX_NUM = 3,
			ITEM_SOCKET_MAX_NUM = 3,
		};

		enum EItemType
		{
			ITEM_TYPE_NONE,					//0
			ITEM_TYPE_WEAPON,				//1//무기
			ITEM_TYPE_ARMOR,				//2//갑옷
			ITEM_TYPE_USE,					//3//아이템 사용
			ITEM_TYPE_AUTOUSE,				//4
			ITEM_TYPE_MATERIAL,				//5
			ITEM_TYPE_SPECIAL,				//6 //스페셜 아이템
			ITEM_TYPE_TOOL,					//7
			ITEM_TYPE_LOTTERY,				//8//복권
			ITEM_TYPE_ELK,					//9//돈
			ITEM_TYPE_METIN,				//10
			ITEM_TYPE_CONTAINER,			//11
			ITEM_TYPE_FISH,					//12//낚시
			ITEM_TYPE_ROD,					//13
			ITEM_TYPE_RESOURCE,				//14
			ITEM_TYPE_CAMPFIRE,				//15
			ITEM_TYPE_UNIQUE,				//16
			ITEM_TYPE_SKILLBOOK,			//17
			ITEM_TYPE_QUEST,				//18
			ITEM_TYPE_POLYMORPH,			//19
			ITEM_TYPE_TREASURE_BOX,			//20//보물상자
			ITEM_TYPE_TREASURE_KEY,			//21//보물상자 열쇠
			ITEM_TYPE_SKILLFORGET,			//22
			ITEM_TYPE_GIFTBOX,				//23
			ITEM_TYPE_PICK,					//24
			ITEM_TYPE_HAIR,					//25//머리
			ITEM_TYPE_TOTEM,				//26//토템
			ITEM_TYPE_BLEND,				//27//생성될때 랜덤하게 속성이 붙는 약물
			ITEM_TYPE_COSTUME,				//28//코스츔 아이템 (2011년 8월 추가된 코스츔 시스템용 아이템)
			ITEM_TYPE_DS,					//29 //용혼석
			ITEM_TYPE_SPECIAL_DS,			//30 // 특수한 용혼석 (DS_SLOT에 착용하는 UNIQUE 아이템이라 생각하면 됨)
			ITEM_TYPE_EXTRACT,					//31 추출도구.
			ITEM_TYPE_SECONDARY_COIN,			//32 명도전.
			ITEM_TYPE_RING,						//33 반지 (유니크 슬롯이 아닌 순수 반지 슬롯)
			ITEM_TYPE_BELT,						//34 벨트

			ITEM_TYPE_MAX_NUM,				
		};

		enum EWeaponSubTypes
		{
			WEAPON_SWORD,
			WEAPON_DAGGER,
			WEAPON_BOW,
			WEAPON_TWO_HANDED,
			WEAPON_BELL,
			WEAPON_FAN,
			WEAPON_ARROW,
			WEAPON_MOUNT_SPEAR,
			WEAPON_CLAW,
			WEAPON_NUM_TYPES,
			WEAPON_NONE = WEAPON_NUM_TYPES+1,
		};

		enum EMaterialSubTypes
		{
			MATERIAL_LEATHER,
			MATERIAL_BLOOD,
			MATERIAL_ROOT,
			MATERIAL_NEEDLE,
			MATERIAL_JEWEL,
			MATERIAL_DS_REFINE_NORMAL, 
			MATERIAL_DS_REFINE_BLESSED, 
			MATERIAL_DS_REFINE_HOLLY,
		};

		enum EArmorSubTypes
		{
			ARMOR_BODY,
			ARMOR_HEAD,
			ARMOR_SHIELD,
			ARMOR_WRIST,
			ARMOR_FOOTS,
		    ARMOR_NECK,
			ARMOR_EAR,
			ARMOR_NUM_TYPES
		};

		enum ECostumeSubTypes
		{
			COSTUME_BODY,				//0	갑옷(main look)
			COSTUME_HAIR,				//1	헤어(탈착가능)
			COSTUME_NUM_TYPES,
		};

		enum EUseSubTypes
		{
			USE_POTION,					// 0
			USE_TALISMAN,
			USE_TUNING,
			USE_MOVE,
			USE_TREASURE_BOX,
			USE_MONEYBAG,
			USE_BAIT,
			USE_ABILITY_UP,
			USE_AFFECT,
			USE_CREATE_STONE,
			USE_SPECIAL,				// 10
			USE_POTION_NODELAY,
			USE_CLEAR,
			USE_INVISIBILITY,
			USE_DETACHMENT,
			USE_BUCKET,
			USE_POTION_CONTINUE,
			USE_CLEAN_SOCKET,
			USE_CHANGE_ATTRIBUTE,
			USE_ADD_ATTRIBUTE,
			USE_ADD_ACCESSORY_SOCKET,	// 20
			USE_PUT_INTO_ACCESSORY_SOCKET,
			USE_ADD_ATTRIBUTE2,
			USE_RECIPE,
			USE_CHANGE_ATTRIBUTE2,
			USE_BIND,
			USE_UNBIND,
			USE_TIME_CHARGE_PER,
			USE_TIME_CHARGE_FIX,				// 28
			USE_PUT_INTO_BELT_SOCKET,			// 29 벨트 소켓에 사용할 수 있는 아이템 
			USE_PUT_INTO_RING_SOCKET,			// 30 반지 소켓에 사용할 수 있는 아이템 (유니크 반지 말고, 새로 추가된 반지 슬롯)
		};

		enum EDragonSoulSubType
		{
			DS_SLOT1,
			DS_SLOT2,
			DS_SLOT3,
			DS_SLOT4,
			DS_SLOT5,
			DS_SLOT6,
			DS_SLOT_NUM_TYPES = 6,
		};

		enum EMetinSubTypes
		{
			METIN_NORMAL,
			METIN_GOLD,
		};

		enum ELimitTypes
		{
			LIMIT_NONE,

			LIMIT_LEVEL,
			LIMIT_STR,
			LIMIT_DEX,
			LIMIT_INT,
			LIMIT_CON,
			LIMIT_PCBANG,

			/// 착용 여부와 상관 없이 실시간으로 시간 차감 (socket0에 소멸 시간이 박힘: unix_timestamp 타입)
			LIMIT_REAL_TIME,						

			/// 아이템을 맨 처음 사용(혹은 착용) 한 순간부터 리얼타임 타이머 시작 
			/// 최초 사용 전에는 socket0에 사용가능시간(초단위, 0이면 프로토의 limit value값 사용) 값이 쓰여있다가 
			/// 아이템 사용시 socket1에 사용 횟수가 박히고 socket0에 unix_timestamp 타입의 소멸시간이 박힘.
			LIMIT_REAL_TIME_START_FIRST_USE,

			/// 아이템을 착용 중일 때만 사용 시간이 차감되는 아이템
			/// socket0에 남은 시간이 초단위로 박힘. (아이템 최초 사용시 해당 값이 0이면 프로토의 limit value값을 socket0에 복사)
			LIMIT_TIMER_BASED_ON_WEAR,

			LIMIT_MAX_NUM
		};

		enum EItemAntiFlag
		{
			ITEM_ANTIFLAG_FEMALE = (1 << 0),
			ITEM_ANTIFLAG_MALE = (1 << 1),
			ITEM_ANTIFLAG_WARRIOR = (1 << 2),
			ITEM_ANTIFLAG_ASSASSIN = (1 << 3),
			ITEM_ANTIFLAG_SURA = (1 << 4),
			ITEM_ANTIFLAG_SHAMAN = (1 << 5),
			ITEM_ANTIFLAG_GET = (1 << 6),
			ITEM_ANTIFLAG_DROP = (1 << 7),
			ITEM_ANTIFLAG_SELL = (1 << 8),
			ITEM_ANTIFLAG_EMPIRE_A = (1 << 9),
			ITEM_ANTIFLAG_EMPIRE_B = (1 << 10),
			ITEM_ANTIFLAG_EMPIRE_R = (1 << 11),
			ITEM_ANTIFLAG_SAVE = (1 << 12),
			ITEM_ANTIFLAG_GIVE = (1 << 13),
			ITEM_ANTIFLAG_PKDROP = (1 << 14),
			ITEM_ANTIFLAG_STACK = (1 << 15),
			ITEM_ANTIFLAG_MYSHOP = (1 << 16),
			ITEM_ANTIFLAG_SAFEBOX = (1 << 17),
			ITEM_ANTIFLAG_WOLFMAN = (1 << 18),
		};

		enum EItemFlag
		{
			ITEM_FLAG_REFINEABLE        = (1 << 0),		// 개량 가능
			ITEM_FLAG_SAVE              = (1 << 1),
			ITEM_FLAG_STACKABLE         = (1 << 2),     // 여러개 합칠 수 있음
			ITEM_FLAG_COUNT_PER_1GOLD   = (1 << 3),		// 가격이 개수 / 가격으로 변함
			ITEM_FLAG_SLOW_QUERY        = (1 << 4),		// 게임 종료시에만 SQL에 쿼리함
			ITEM_FLAG_RARE              = (1 << 5),
			ITEM_FLAG_UNIQUE            = (1 << 6),
			ITEM_FLAG_MAKECOUNT			= (1 << 7),
			ITEM_FLAG_IRREMOVABLE		= (1 << 8),
			ITEM_FLAG_CONFIRM_WHEN_USE	= (1 << 9),
			ITEM_FLAG_QUEST_USE         = (1 << 10),    // 퀘스트 스크립트 돌리는지?
			ITEM_FLAG_QUEST_USE_MULTIPLE= (1 << 11),    // 퀘스트 스크립트 돌리는지?
			ITEM_FLAG_UNUSED03          = (1 << 12),    // UNUSED03
			ITEM_FLAG_LOG               = (1 << 13),    // 사용시 로그를 남기는 아이템인가?
			ITEM_FLAG_APPLICABLE		= (1 << 14),
		};

		enum EWearPositions
		{
			WEAR_BODY,          // 0
			WEAR_HEAD,          // 1
			WEAR_FOOTS,         // 2
			WEAR_WRIST,         // 3
			WEAR_WEAPON,        // 4
			WEAR_NECK,          // 5
			WEAR_EAR,           // 6
			WEAR_UNIQUE1,       // 7
			WEAR_UNIQUE2,       // 8
			WEAR_ARROW,         // 9
			WEAR_SHIELD,        // 10
			WEAR_MAX_NUM,
		};

		enum EItemWearableFlag
		{
			WEARABLE_BODY       = (1 << 0),
			WEARABLE_HEAD       = (1 << 1),
			WEARABLE_FOOTS      = (1 << 2),
			WEARABLE_WRIST      = (1 << 3),
			WEARABLE_WEAPON     = (1 << 4),
			WEARABLE_NECK       = (1 << 5),
			WEARABLE_EAR        = (1 << 6),
			WEARABLE_UNIQUE     = (1 << 7),
			WEARABLE_SHIELD     = (1 << 8),
			WEARABLE_ARROW      = (1 << 9),
		};

		enum EApplyTypes
		{
			APPLY_NONE,
			APPLY_MAX_HP,
			APPLY_MAX_SP,
			APPLY_CON,
			APPLY_INT,
			APPLY_STR,
			APPLY_DEX,
			APPLY_ATT_SPEED,
			APPLY_MOV_SPEED,
			APPLY_CAST_SPEED,
			APPLY_HP_REGEN,
			APPLY_SP_REGEN,
			APPLY_POISON_PCT,
			APPLY_STUN_PCT,
			APPLY_SLOW_PCT,
			APPLY_CRITICAL_PCT,
			APPLY_PENETRATE_PCT,
			APPLY_ATTBONUS_HUMAN,
			APPLY_ATTBONUS_ANIMAL,
			APPLY_ATTBONUS_ORC,
			APPLY_ATTBONUS_MILGYO,
			APPLY_ATTBONUS_UNDEAD,
			APPLY_ATTBONUS_DEVIL,
			APPLY_STEAL_HP,
			APPLY_STEAL_SP,
			APPLY_MANA_BURN_PCT,
			APPLY_DAMAGE_SP_RECOVER,
			APPLY_BLOCK,
			APPLY_DODGE,
			APPLY_RESIST_SWORD,
			APPLY_RESIST_TWOHAND,
			APPLY_RESIST_DAGGER,
			APPLY_RESIST_BELL,
			APPLY_RESIST_FAN,
			APPLY_RESIST_BOW,
			APPLY_RESIST_FIRE,
			APPLY_RESIST_ELEC,
			APPLY_RESIST_MAGIC,
			APPLY_RESIST_WIND,
			APPLY_REFLECT_MELEE,
			APPLY_REFLECT_CURSE,
			APPLY_POISON_REDUCE,
			APPLY_KILL_SP_RECOVER,
			APPLY_EXP_DOUBLE_BONUS,
			APPLY_GOLD_DOUBLE_BONUS,
			APPLY_ITEM_DROP_BONUS,
			APPLY_POTION_BONUS,
			APPLY_KILL_HP_RECOVER,
			APPLY_IMMUNE_STUN,
			APPLY_IMMUNE_SLOW,
			APPLY_IMMUNE_FALL,
			APPLY_SKILL,
			APPLY_BOW_DISTANCE,
			APPLY_ATT_GRADE_BONUS,
			APPLY_DEF_GRADE_BONUS,
			APPLY_MAGIC_ATT_GRADE,
			APPLY_MAGIC_DEF_GRADE,
			APPLY_CURSE_PCT,
			APPLY_MAX_STAMINA,
			APPLY_ATT_BONUS_TO_WARRIOR,
			APPLY_ATT_BONUS_TO_ASSASSIN,
			APPLY_ATT_BONUS_TO_SURA,
			APPLY_ATT_BONUS_TO_SHAMAN,
			APPLY_ATT_BONUS_TO_MONSTER,
			APPLY_MALL_ATTBONUS,
			APPLY_MALL_DEFBONUS,
			APPLY_MALL_EXPBONUS,
			APPLY_MALL_ITEMBONUS,
			APPLY_MALL_GOLDBONUS,
			APPLY_MAX_HP_PCT,
			APPLY_MAX_SP_PCT,
			APPLY_SKILL_DAMAGE_BONUS,
			APPLY_NORMAL_HIT_DAMAGE_BONUS,
			APPLY_SKILL_DEFEND_BONUS,
			APPLY_NORMAL_HIT_DEFEND_BONUS,
			APPLY_EXTRACT_HP_PCT,
			APPLY_PC_BANG_EXP_BONUS,
			APPLY_PC_BANG_DROP_BONUS,
			APPLY_RESIST_WARRIOR,
			APPLY_RESIST_ASSASSIN,
			APPLY_RESIST_SURA,
			APPLY_RESIST_SHAMAN,
			APPLY_ENERGY,
			APPLY_DEF_GRADE,
			APPLY_COSTUME_ATTR_BONUS,
			APPLY_MAGIC_ATTBONUS_PER,
			APPLY_MELEE_MAGIC_ATTBONUS_PER,
			APPLY_RESIST_ICE,
			APPLY_RESIST_EARTH,
			APPLY_RESIST_DARK,
			APPLY_ANTI_CRITICAL_PCT,
			APPLY_ANTI_PENETRATE_PCT,
			APPLY_BLEEDING_PCT,
			APPLY_BLEEDING_REDUCE,
			APPLY_ATT_BONUS_TO_WOLFMAN,
			APPLY_RESIST_WOLFMAN,
			APPLY_RESIST_CLAW,
			MAX_APPLY_NUM,
		};

		enum EImmuneFlags
		{
			IMMUNE_PARA         = (1 << 0),
			IMMUNE_CURSE        = (1 << 1),
			IMMUNE_STUN         = (1 << 2),
			IMMUNE_SLEEP        = (1 << 3),
			IMMUNE_SLOW         = (1 << 4),
			IMMUNE_POISON       = (1 << 5),
			IMMUNE_TERROR       = (1 << 6),
		};

#pragma pack(push)
#pragma pack(1)
		typedef struct SItemLimit
		{
			BYTE        bType;
			long        lValue;
		} TItemLimit;

		typedef struct SItemApply
		{
			BYTE        bType;
			long        lValue;
		} TItemApply;

		typedef struct SItemTable
		{
			DWORD       dwVnum;
			DWORD       dwVnumRange;
			char        szName[ITEM_NAME_MAX_LEN + 1];
			char        szLocaleName[ITEM_NAME_MAX_LEN + 1];
			BYTE        bType;
			BYTE        bSubType;
			
			BYTE        bWeight;
			BYTE        bSize;
			
			DWORD       dwAntiFlags;
			DWORD       dwFlags;
			DWORD       dwWearFlags;
			DWORD       dwImmuneFlag;
						
			DWORD       dwIBuyItemPrice;			
			DWORD		dwISellItemPrice;
			
			TItemLimit  aLimits[ITEM_LIMIT_MAX_NUM];
			TItemApply  aApplies[ITEM_APPLY_MAX_NUM];
			long        alValues[ITEM_VALUES_MAX_NUM];
			long        alSockets[ITEM_SOCKET_MAX_NUM];
			DWORD       dwRefinedVnum;
			WORD		wRefineSet;
			BYTE        bAlterToMagicItemPct;
			BYTE		bSpecular;
			BYTE        bGainSocketPct;
		} TItemTable;

//		typedef struct SItemTable
//		{
//			DWORD       dwVnum;
//			char        szItemName[ITEM_NAME_MAX_LEN + 1];
//			BYTE        bType;
//			BYTE        bSubType;
//			BYTE        bSize;
//			DWORD       dwAntiFlags;
//			DWORD       dwFlags;
//			DWORD       dwWearFlags;
//			DWORD       dwIBuyItemPrice;
//			DWORD		dwISellItemPrice;
//			TItemLimit  aLimits[ITEM_LIMIT_MAX_NUM];
//			TItemApply  aApplies[ITEM_APPLY_MAX_NUM];
//			long        alValues[ITEM_VALUES_MAX_NUM];
//			long        alSockets[ITEM_SOCKET_MAX_NUM];
//			DWORD       dwRefinedVnum;
//			BYTE		bSpecular;
//			DWORD		dwIconNumber;
//		} TItemTable;
#pragma pack(pop)

	public:
		CItemData();
		virtual ~CItemData();

		void Clear();
		void SetSummary(const std::string& c_rstSumm);
		void SetDescription(const std::string& c_rstDesc);

		CGraphicThing * GetModelThing();
		CGraphicThing * GetSubModelThing();
		CGraphicThing * GetDropModelThing();
		CGraphicSubImage * GetIconImage();

		DWORD GetLODModelThingCount();
		BOOL GetLODModelThingPointer(DWORD dwIndex, CGraphicThing ** ppModelThing);

		DWORD GetAttachingDataCount();
		BOOL GetCollisionDataPointer(DWORD dwIndex, const NRaceData::TAttachingData ** c_ppAttachingData);
		BOOL GetAttachingDataPointer(DWORD dwIndex, const NRaceData::TAttachingData ** c_ppAttachingData);

		/////
		const TItemTable*	GetTable() const;
		DWORD GetIndex() const;
		const char * GetName() const;
		const char * GetDescription() const;
		const char * GetSummary() const;
		BYTE GetType() const;
		BYTE GetSubType() const;
		UINT GetRefine() const;
		const char* GetUseTypeString() const;
		DWORD GetWeaponType() const;
		BYTE GetSize() const;
		BOOL IsAntiFlag(DWORD dwFlag) const;
		BOOL IsFlag(DWORD dwFlag) const;
		BOOL IsWearableFlag(DWORD dwFlag) const;
		BOOL HasNextGrade() const;
		DWORD GetWearFlags() const;
		DWORD GetIBuyItemPrice() const;
		DWORD GetISellItemPrice() const;
		BOOL GetLimit(BYTE byIndex, TItemLimit * pItemLimit) const;
		BOOL GetApply(BYTE byIndex, TItemApply * pItemApply) const;
		long GetValue(BYTE byIndex) const;
		long GetSocket(BYTE byIndex) const;
		long SetSocket(BYTE byIndex,DWORD value);
		int GetSocketCount() const;
		DWORD GetIconNumber() const;

		UINT	GetSpecularPoweru() const;
		float	GetSpecularPowerf() const;
	
		/////

		BOOL IsEquipment() const;

		/////

		//BOOL LoadItemData(const char * c_szFileName);
		void SetDefaultItemData(const char * c_szIconFileName, const char * c_szModelFileName  = NULL);
		void SetItemTableData(TItemTable * pItemTable);

	protected:
		void __LoadFiles();
		void __SetIconImage(const char * c_szFileName);

	protected:
		std::string m_strModelFileName;
		std::string m_strSubModelFileName;
		std::string m_strDropModelFileName;
		std::string m_strIconFileName;
		std::string m_strDescription;
		std::string m_strSummary;
		std::vector<std::string> m_strLODModelFileNameVector;

		CGraphicThing * m_pModelThing;
		CGraphicThing * m_pSubModelThing;
		CGraphicThing * m_pDropModelThing;
		CGraphicSubImage * m_pIconImage;
		std::vector<CGraphicThing *> m_pLODModelThingVector;

		NRaceData::TAttachingDataVector m_AttachingDataVector;
		DWORD		m_dwVnum;
		TItemTable m_ItemTable;
		
	public:
		static void DestroySystem();

		static CItemData* New();
		static void Delete(CItemData* pkItemData);

		static CDynamicPool<CItemData>		ms_kPool;
};
