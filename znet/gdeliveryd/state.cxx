#include "callid.hxx"

#ifdef WIN32
#include <winsock2.h>
#include "gnproto.h"
#include "gncompress.h"
#else
#include "protocol.h"
#include "binder.h"
#endif

namespace GNET
{

static GNET::Protocol::Type _state_GDeliverServer[] = 
{
	PROTOCOL_FACBASEDATASEND,
	PROTOCOL_CREATEFACBASE,
	PROTOCOL_GETFACACTTOPTABLE,
	PROTOCOL_GETFACTIONDYNAMIC,
	PROTOCOL_GETKDCTOKEN,
	PROTOCOL_RAIDSTART,
	PROTOCOL_RAIDENTER,
	PROTOCOL_RAIDQUIT,
	PROTOCOL_TEAMRAIDQUIT,
	PROTOCOL_HIDESEEKRAIDQUIT,
	PROTOCOL_RAIDGETLIST,
	PROTOCOL_RAIDAPPOINT,
	PROTOCOL_RAIDKICK,
	PROTOCOL_RAIDGETROOM,
	PROTOCOL_RAIDJOINACCEPT,
	PROTOCOL_RAIDOPENVOTE,
	PROTOCOL_RAIDVOTES,
	PROTOCOL_TEAMRAIDMAPPINGSUCCESS_RE,
	PROTOCOL_HIDESEEKMAPPINGSUCCESS_RE,
	PROTOCOL_TEAMRAIDUPDATECHAR,
	PROTOCOL_FACTIONPKRAIDCHALLENGE,
	PROTOCOL_FACTIONPKRAIDCHALLENGECANCEL,
	PROTOCOL_FACTIONPKRAIDINVITE_RE,
	PROTOCOL_FACTIONPKRAIDGETSCORE,
	PROTOCOL_FACTIONPKRAIDGETLIST,
	PROTOCOL_FACTIONWITHDRAWBONUS,
	PROTOCOL_GETFRIENDGTSTATUS,
	RPC_COMMONDATAQUERY,
	PROTOCOL_GCIRCLECHAT,
	PROTOCOL_CIRCLELIST,
	PROTOCOL_GETCIRCLEBASEINFO,
	PROTOCOL_CLAIMCIRCLEGRADBONUS,
	PROTOCOL_CONSIGNCANCELPOST,
	PROTOCOL_CONSIGNQUERY,
	PROTOCOL_CONSIGNGETITEM,
	PROTOCOL_CONSIGNLISTALL,
	PROTOCOL_CONSIGNLISTROLE,
	PROTOCOL_CONSIGNLISTLARGECATEGORY,
	PROTOCOL_PLAYERLOGIN,
	PROTOCOL_PLAYERSTATUSSYNC,
	PROTOCOL_ENTERWORLD,
	PROTOCOL_STATUSANNOUNCE,
	PROTOCOL_ROLELIST,
	PROTOCOL_CREATEROLE,
	PROTOCOL_DELETEROLE,
	PROTOCOL_UNDODELETEROLE,
	PROTOCOL_PLAYERBASEINFO,
	PROTOCOL_PLAYERBASEINFO2,
	PROTOCOL_PLAYERBASEINFOCRC,
	PROTOCOL_GETPLAYERIDBYNAME,
	PROTOCOL_SETUICONFIG,
	PROTOCOL_GETUICONFIG,
	PROTOCOL_SETHELPSTATES,
	PROTOCOL_GETHELPSTATES,
	PROTOCOL_GETPLAYERBRIEFINFO,
	PROTOCOL_BATTLEFLAGBUFFSCOPE,
	PROTOCOL_GETCOUPONSREQ,
	PROTOCOL_PUBLICCHAT,
	PROTOCOL_PRIVATECHAT,
	PROTOCOL_ADDFRIEND,
	PROTOCOL_GETFRIENDS,
	PROTOCOL_SETGROUPNAME,
	PROTOCOL_SETFRIENDGROUP,
	PROTOCOL_DELFRIEND,
	PROTOCOL_DELFRIEND_RE,
	PROTOCOL_FRIENDSTATUS,
	RPC_ADDFRIENDRQST,
	PROTOCOL_GETENEMIES,
	PROTOCOL_UPDATEENEMY,
	PROTOCOL_GETSAVEDMSG,
	PROTOCOL_CHATROOMCREATE,
	PROTOCOL_CHATROOMINVITE,
	PROTOCOL_CHATROOMINVITE_RE,
	PROTOCOL_CHATROOMJOIN,
	PROTOCOL_CHATROOMLEAVE,
	PROTOCOL_CHATROOMEXPEL,
	PROTOCOL_CHATROOMSPEAK,
	PROTOCOL_CHATROOMLIST,
	PROTOCOL_FRIENDCALLBACKINFO,
	PROTOCOL_FRIENDCALLBACKMAIL,
	PROTOCOL_FRIENDCALLBACKAWARD,
	PROTOCOL_FRIENDCALLBACKSUBSCRIBE,
	PROTOCOL_TRADESTART,
	PROTOCOL_TRADEADDGOODS,
	PROTOCOL_TRADEREMOVEGOODS,
	PROTOCOL_TRADESUBMIT,
	PROTOCOL_TRADEMOVEOBJ,
	PROTOCOL_TRADECONFIRM,
	PROTOCOL_TRADEDISCARD,
	RPC_TRADESTARTRQST,
	RPC_USERLOGIN,
	RPC_USERLOGIN2,
	RPC_GQUERYPASSWD,
	RPC_MATRIXPASSWD,
	RPC_MATRIXPASSWD2,
	RPC_MATRIXTOKEN,
	PROTOCOL_MATRIXFAILURE,
	PROTOCOL_TRYRECONNECT,
	PROTOCOL_GMRESTARTSERVER,
	PROTOCOL_GMSHUTDOWNLINE,
	PROTOCOL_GMONLINENUM,
	PROTOCOL_GMLISTONLINEUSER,
	PROTOCOL_GMKICKOUTUSER,
	PROTOCOL_GMFORBIDSELLPOINT,
	PROTOCOL_GMKICKOUTROLE,
	PROTOCOL_GMSHUTUP,
	PROTOCOL_GMSHUTUPROLE,
	PROTOCOL_GMTOGGLECHAT,
	PROTOCOL_GMFORBIDROLE,
	PROTOCOL_GMPRIVILEGECHANGE,
	PROTOCOL_REPORT2GM,
	PROTOCOL_COMPLAIN2GM,
	PROTOCOL_ANNOUNCELINKTYPE,
	RPC_GMQUERYROLEINFO,
	PROTOCOL_SETMAXONLINENUM,
	RPC_GETMAXONLINENUM,
	RPC_GMGETGAMEATTRI,
	RPC_GMSETGAMEATTRI,
	RPC_GETLINEPLAYERLIMIT,
	RPC_SETLINEPLAYERLIMIT,
	PROTOCOL_VERIFYMASTER,
	PROTOCOL_VERIFYMASTER_RE,
	RPC_DBVERIFYMASTER,
	RPC_FORBIDUSER,
	PROTOCOL_ACREPORT,
	PROTOCOL_ACANSWER,
	PROTOCOL_ACPROTOSTAT,
	PROTOCOL_REPORTIP,
	PROTOCOL_ACACCUSE,
	PROTOCOL_CHECKNEWMAIL,
	PROTOCOL_SYSSENDMAIL,
	PROTOCOL_SYSRECOVEREDOBJMAIL,
	PROTOCOL_GETMAILLIST,
	PROTOCOL_GETMAIL,
	PROTOCOL_GETMAILATTACHOBJ,
	PROTOCOL_DELETEMAIL,
	PROTOCOL_PRESERVEMAIL,
	PROTOCOL_PLAYERSENDMAIL,
	PROTOCOL_AUCTIONOPEN,
	PROTOCOL_AUCTIONBID,
	PROTOCOL_SENDAUCTIONBID,
	PROTOCOL_AUCTIONLIST,
	PROTOCOL_AUCTIONCLOSE,
	PROTOCOL_AUCTIONGET,
	PROTOCOL_AUCTIONATTENDLIST,
	PROTOCOL_AUCTIONEXITBID,
	PROTOCOL_INSTANCINGGETLIST,
	PROTOCOL_INSTANCINGGETFIELD,
	PROTOCOL_INSTANCINGKICK,
	PROTOCOL_INSTANCINGSTART,
	PROTOCOL_INSTANCINGAVAILABLELIST,
	PROTOCOL_INSTANCINGACCEPT,
	PROTOCOL_INSTANCINGLEAVE,
	PROTOCOL_INSTANCINGAPPOINT,
	PROTOCOL_INSTANCINGENTER,
	PROTOCOL_BATTLEGETMAP,
	PROTOCOL_BATTLEGETLIST,
	PROTOCOL_BATTLEGETFIELD,
	PROTOCOL_BATTLEJOIN,
	PROTOCOL_BATTLEENTER,
	PROTOCOL_BATTLELEAVE,
	PROTOCOL_CRSSVRTEAMSGETROLENOTIFY,
	PROTOCOL_CRSSVRTEAMSGETTEAMBASE,
	PROTOCOL_CRSSVRTEAMSGETROLELIST,
	PROTOCOL_CRSSVRTEAMSRENAME,
	PROTOCOL_CRSSVRTEAMSDISMISS,
	PROTOCOL_CRSSVRTEAMSRECRUIT,
	PROTOCOL_CRSSVRTEAMSCHGCAPTAIN,
	PROTOCOL_CRSSVRTEAMSLEAVE,
	PROTOCOL_CRSSVRTEAMSKICK,
	PROTOCOL_CRSSVRTEAMSSEARCH,
	RPC_CRSSVRTEAMSINVITE,
	PROTOCOL_FACTIONCHAT,
	PROTOCOL_FACTIONCREATE,
	PROTOCOL_FACTIONLIST,
	PROTOCOL_FACTIONDISMISS,
	PROTOCOL_FACTIONRECRUIT,
	PROTOCOL_FACTIONEXPEL,
	PROTOCOL_FACTIONANNOUNCE,
	PROTOCOL_FACTIONAPPOINT,
	PROTOCOL_FACTIONRESIGN,
	PROTOCOL_FACTIONLEAVE,
	PROTOCOL_FACTIONUPGRADE,
	PROTOCOL_FACTIONNICKNAME,
	PROTOCOL_GETFACTIONBASEINFO,
	PROTOCOL_GETFACTIONHOSTILE,
	PROTOCOL_HOSTILEDELETE,
	PROTOCOL_HOSTILEADD,
	PROTOCOL_HOSTILEPROTECT,
	RPC_FACTIONINVITE,
	PROTOCOL_SECTRECRUIT,
	PROTOCOL_SECTEXPEL,
	PROTOCOL_SECTLIST,
	RPC_SECTINVITE,
	PROTOCOL_GETTOPTABLE,
	PROTOCOL_COMBATCHALLENGE,
	PROTOCOL_COMBATTOP,
	RPC_COMBATINVITE,
	PROTOCOL_CASHLOCK,
	PROTOCOL_CASHPASSWORDSET,
	PROTOCOL_FAMILYRECORD,
	PROTOCOL_DOMAINLOGIN,
	PROTOCOL_DOMAINVALIDATE,
	PROTOCOL_DOMAINCMD,
	PROTOCOL_AUTOLOCKSET,
	PROTOCOL_SIEGEINFOGET,
	PROTOCOL_SIEGEENTER,
	PROTOCOL_SIEGETOP,
	RPC_QUERYUSERID,
	PROTOCOL_CONTESTINVITE_RE,
	PROTOCOL_CONTESTANSWER,
	PROTOCOL_CONTESTEXIT,
	PROTOCOL_C2SFUNGAMEDATASEND,
	PROTOCOL_C2SHOMETOWNDATASEND,
	PROTOCOL_SNSSETPLAYERINFO,
	PROTOCOL_SNSGETPLAYERINFO,
	PROTOCOL_SNSCANCELMESSAGE,
	PROTOCOL_SNSCANCELLEAVEMESSAGE,
	PROTOCOL_SNSLISTMESSAGE,
	PROTOCOL_SNSGETMESSAGE,
	PROTOCOL_SNSACCEPTAPPLY,
	PROTOCOL_REFLISTREFERRALS,
	PROTOCOL_REFWITHDRAWEXP,
	PROTOCOL_REFWITHDRAWBONUS,
	PROTOCOL_REFGETREFERENCECODE,
	PROTOCOL_GSHOPSETSALESCHEME,
	PROTOCOL_GSHOPSETDISCOUNTSCHEME,
	RPC_GSHOPGETSCHEME,
	PROTOCOL_TERRITORYMAPGET,
	PROTOCOL_UNIQUEBIDHISTORY,
	PROTOCOL_DISABLEAUTOLOCK,
	RPC_PLAYERIDENTITYMATCH,
	PROTOCOL_TRYCHANGEGS,
	PROTOCOL_DOCHANGEGS,
	PROTOCOL_SSOGETTICKETREQ,
	PROTOCOL_FASTPAY,
	PROTOCOL_QUERYROLENAMEHISTORY,
	PROTOCOL_KINGDOMANNOUNCE,
	PROTOCOL_KINGDOMAPPOINT,
	PROTOCOL_KINGDOMDISCHARGE,
	PROTOCOL_KINGDOMSETGAMEATTRI,
	PROTOCOL_KINGDOMGETINFO,
	PROTOCOL_KINGDOMGETTITLE,
	RPC_KINGGUARDINVITE,
	RPC_QUEENOPENBATHINVITE,
	PROTOCOL_QUEENTRYOPENBATH,
	PROTOCOL_GETKINGTASK,
	PROTOCOL_SENDFLOWERREQ,
	PROTOCOL_TOPFLOWERREQ,
	PROTOCOL_TOPFLOWERGETGIFT,
	PROTOCOL_OPENBANQUETCANCEL,
	PROTOCOL_OPENBANQUETQUEUECOUNT,
	PROTOCOL_OPENBANQUETENTER,
	PROTOCOL_OPENBANQUETGETFIELD,
	PROTOCOL_GETTOUCHPOINT,
	PROTOCOL_GETPASSPORTCASHADD,
	RPC_OPERATIONCMD,
};

GNET::Protocol::Manager::Session::State state_GDeliverServer(_state_GDeliverServer,
						sizeof(_state_GDeliverServer)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_GAuthClient[] = 
{
	RPC_PASSPORTGETROLELIST,
	PROTOCOL_KEYEXCHANGE,
	PROTOCOL_STATUSANNOUNCE,
	PROTOCOL_KICKOUTUSER,
	PROTOCOL_ACCOUNTINGRESPONSE,
	PROTOCOL_QUERYUSERPRIVILEGE_RE,
	PROTOCOL_QUERYUSERFORBID_RE,
	PROTOCOL_UPDATEREMAINTIME,
	PROTOCOL_TRANSBUYPOINT_RE,
	PROTOCOL_ADDCASH,
	PROTOCOL_ADDCASH_RE,
	PROTOCOL_GETPLAYERIDBYNAME,
	PROTOCOL_SYSSENDMAIL,
	PROTOCOL_SYSRECOVEREDOBJMAIL,
	PROTOCOL_VERIFYMASTER,
	PROTOCOL_VERIFYMASTER_RE,
	PROTOCOL_CERTANSWER,
	PROTOCOL_CERTFINISH,
	RPC_CERTCHALLENGE,
	RPC_GQUERYPASSWD,
	RPC_MATRIXPASSWD,
	RPC_MATRIXPASSWD2,
	RPC_MATRIXTOKEN,
	PROTOCOL_MATRIXFAILURE,
	RPC_USERLOGIN,
	RPC_USERLOGIN2,
	RPC_USERLOGOUT,
	RPC_GETADDCASHSN,
	RPC_CASHSERIAL,
	PROTOCOL_ADDICTIONCONTROL,
	PROTOCOL_VIPANNOUNCE,
	RPC_QUERYUSERID,
	PROTOCOL_NETBARANNOUNCE,
	PROTOCOL_BILLINGREQUEST,
	PROTOCOL_BILLINGBALANCE,
	PROTOCOL_AUTHDVERSION,
	RPC_NETMARBLELOGIN,
	RPC_NETMARBLEDEDUCT,
	PROTOCOL_SSOGETTICKETREP,
	PROTOCOL_DISCOUNTANNOUNCE,
	PROTOCOL_AU2GAME,
	PROTOCOL_SYSSENDMAIL4,
};

GNET::Protocol::Manager::Session::State state_GAuthClient(_state_GAuthClient,
						sizeof(_state_GAuthClient)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_GProviderDeliveryServer[] = 
{
	PROTOCOL_SYNCPLAYERFACCOUPONADD,
	PROTOCOL_STARTFACTIONMULTIEXP,
	PROTOCOL_FACBASEDATABROADCAST,
	PROTOCOL_FACBASEDATADELIVER,
	PROTOCOL_FACBASEDATASEND,
	PROTOCOL_FACMALLCHANGE,
	PROTOCOL_FACDYNDONATECASH,
	PROTOCOL_FACDYNPUTAUCTION,
	PROTOCOL_FACDYNWINAUCTION,
	PROTOCOL_FACDYNBUYAUCTION,
	PROTOCOL_FACDYNBUILDINGUPGRADE,
	PROTOCOL_FACDYNBUILDINGCOMPLETE,
	PROTOCOL_GFACBASEENTER,
	PROTOCOL_GFACBASELEAVE,
	PROTOCOL_FACBASEPROPCHANGE,
	PROTOCOL_FACBASEBUILDINGPROGRESS,
	PROTOCOL_FACAUCTIONPUT,
	PROTOCOL_STARTFACBASE_RE,
	PROTOCOL_STOPFACBASE_RE,
	PROTOCOL_GUPDATEFACACT,
	PROTOCOL_GFACBASESERVERREGISTER,
	PROTOCOL_GLOBALDROPBROADCAST,
	PROTOCOL_REGISTERGLOBALCOUNTER,
	PROTOCOL_GRAIDSERVERREGISTER,
	PROTOCOL_GRAIDOPEN,
	PROTOCOL_GRAIDJOIN,
	PROTOCOL_GRAIDMEMBERCHANGE,
	PROTOCOL_GRAIDLEAVE,
	PROTOCOL_GRAIDENTERSUCCESS,
	PROTOCOL_GRAIDEND,
	PROTOCOL_SENDRAIDSTART_RE,
	PROTOCOL_SENDRAIDENTER_RE,
	PROTOCOL_GCRSSVRTEAMSGETSCORE,
	PROTOCOL_GRAIDTEAMAPPLY,
	PROTOCOL_GHIDESEEKRAIDAPPLY,
	PROTOCOL_GFENGSHENRAIDAPPLY,
	PROTOCOL_GFACTIONPKRAIDKILL,
	PROTOCOL_GFACTIONPKRAIDDEPOSIT,
	PROTOCOL_GFACTIONPKRAIDFIGHTNOTIFY,
	PROTOCOL_GCIRCLECHAT,
	PROTOCOL_ADDCIRCLEPOINT,
	RPC_CIRCLEGETASYNCDATA,
	PROTOCOL_ANNOUNCEPROVIDERID,
	PROTOCOL_PLAYERLOGIN_RE,
	PROTOCOL_PLAYERKICKOUT_RE,
	PROTOCOL_PLAYERLOGOUT,
	PROTOCOL_PLAYEROFFLINE_RE,
	PROTOCOL_PLAYERRECONNECT_RE,
	PROTOCOL_QUERYPLAYERSTATUS,
	PROTOCOL_GETTASKDATA,
	PROTOCOL_SETTASKDATA,
	PROTOCOL_SETCHATEMOTION,
	PROTOCOL_BATTLEFLAGSTART,
	PROTOCOL_QUERYBATTLEFLAGBUFF,
	PROTOCOL_GETCOUPONSREP,
	PROTOCOL_GTRADESTART_RE,
	PROTOCOL_GTRADEDISCARD,
	PROTOCOL_KEEPALIVE,
	PROTOCOL_DISCONNECTPLAYER,
	PROTOCOL_GMRESTARTSERVER_RE,
	PROTOCOL_COMMONDATASYNC,
	RPC_COMMONDATACHANGE,
	PROTOCOL_GETMAILLIST,
	PROTOCOL_GETMAIL,
	PROTOCOL_GETMAILATTACHOBJ,
	PROTOCOL_DELETEMAIL,
	PROTOCOL_PRESERVEMAIL,
	PROTOCOL_PLAYERSENDMAIL,
	PROTOCOL_AUCTIONOPEN,
	PROTOCOL_AUCTIONBID,
	PROTOCOL_SENDAUCTIONBID,
	PROTOCOL_AUCTIONLIST,
	PROTOCOL_AUCTIONCLOSE,
	PROTOCOL_AUCTIONGET,
	PROTOCOL_AUCTIONGETITEM,
	PROTOCOL_AUCTIONATTENDLIST,
	PROTOCOL_AUCTIONEXITBID,
	PROTOCOL_QUERYREWARDTYPE,
	PROTOCOL_QUERYGAMESERVERATTR,
	PROTOCOL_ACREPORTCHEATER,
	PROTOCOL_ACTRIGGERQUESTION,
	PROTOCOL_BATTLEENTER,
	PROTOCOL_BATTLESERVERREGISTER,
	PROTOCOL_BATTLEENTERFAIL,
	PROTOCOL_GBATTLEEND,
	PROTOCOL_BATTLESTART_RE,
	PROTOCOL_GBATTLELEAVE,
	PROTOCOL_DEBUGCOMMAND,
	PROTOCOL_GOPENBANQUETLEAVE,
	PROTOCOL_GOPENBANQUETJOIN,
	PROTOCOL_SENDINSTANCINGJOIN,
	PROTOCOL_INSTANCINGREGISTER,
	PROTOCOL_INSTANCINGENTERFAIL,
	PROTOCOL_GINSTANCINGEND,
	PROTOCOL_SENDINSTANCINGSTART_RE,
	PROTOCOL_GINSTANCINGLEAVE,
	PROTOCOL_GBATTLEJOIN,
	PROTOCOL_CHATBROADCAST,
	PROTOCOL_PRIVATECHAT,
	PROTOCOL_FACTIONCHAT,
	PROTOCOL_SENDFACTIONCREATE,
	PROTOCOL_PLAYERFACTIONINFO,
	PROTOCOL_SYNCPLAYERFACTION,
	PROTOCOL_FACTIONDISMISS,
	PROTOCOL_FACTIONUPGRADE,
	PROTOCOL_GHOSTILEPROTECT,
	PROTOCOL_SENDCRSSVRTEAMSCREATE,
	PROTOCOL_SYNCROLEDATA,
	PROTOCOL_UPDATEENEMY,
	RPC_GETWEEKLYTOP,
	RPC_GETDAILYTOP,
	RPC_PUTSPOUSE,
	PROTOCOL_STOCKCOMMISSION,
	PROTOCOL_STOCKACCOUNT,
	PROTOCOL_STOCKTRANSACTION,
	PROTOCOL_STOCKBILL,
	PROTOCOL_STOCKCANCEL,
	PROTOCOL_COMBATKILL,
	PROTOCOL_PLAYERCHANGEGS_RE,
	RPC_TASKASYNCDATA,
	PROTOCOL_FAMILYUSESKILL,
	PROTOCOL_FAMILYEXPSHARE,
	RPC_CERTCHALLENGE,
	PROTOCOL_GSIEGESERVERREGISTER,
	PROTOCOL_GSIEGEEND,
	PROTOCOL_SENDSIEGESTART_RE,
	PROTOCOL_SENDSIEGECHALLENGE,
	PROTOCOL_SIEGESETASSISTANT,
	PROTOCOL_SIEGEKILL,
	PROTOCOL_SIEGEBROADCAT,
	PROTOCOL_SECTUPDATE,
	PROTOCOL_SECTQUIT,
	PROTOCOL_SENDSNSPRESSMESSAGE,
	PROTOCOL_SENDSNSAPPLY,
	PROTOCOL_SENDSNSVOTE,
	PROTOCOL_BILLINGREQUEST2,
	PROTOCOL_BILLINGBALANCE,
	PROTOCOL_SENDREFCASHUSED,
	PROTOCOL_SENDREFLEVELUP,
	PROTOCOL_SENDCHANGEPROFILE,
	PROTOCOL_QUERYNETBARREWARD,
	PROTOCOL_ACHIEVEMENTMESSAGE,
	PROTOCOL_PKMESSAGE,
	PROTOCOL_REFINEMESSAGE,
	PROTOCOL_GINSTANCINGSTATUS,
	PROTOCOL_SENDHOMETOWNEXCHGMONEY,
	PROTOCOL_GTERRITORYCHALLENGE,
	PROTOCOL_TERRITORYSERVERREGISTER,
	PROTOCOL_SENDTERRITORYSTART_RE,
	PROTOCOL_GTERRITORYEND,
	PROTOCOL_GTERRITORYENTER,
	PROTOCOL_GTERRITORYLEAVE,
	RPC_GTERRITORYITEMGET,
	RPC_GUNIQUEBID,
	RPC_GUNIQUEGETITEM,
	RPC_GETFRIENDNUMBER,
	PROTOCOL_TRYCHANGEDS,
	PROTOCOL_PLAYERCHANGEDS_RE,
	PROTOCOL_GCONSIGNSTART,
	PROTOCOL_GCONSIGNSTARTROLE,
	PROTOCOL_PLAYERCONSIGNOFFLINE_RE,
	PROTOCOL_GCHANGEROLENAME,
	PROTOCOL_GCHANGEFACTIONNAME,
	PROTOCOL_GKINGDOMBATTLEREGISTER,
	PROTOCOL_KINGDOMBATTLESTART_RE,
	PROTOCOL_GKINGDOMBATTLEEND,
	PROTOCOL_GKINGDOMBATTLEHALF,
	PROTOCOL_GKINGDOMATTACKERFAIL,
	PROTOCOL_GKINGDOMBATTLEENTER,
	PROTOCOL_GKINGDOMBATTLELEAVE,
	PROTOCOL_GKINGDOMPOINTCHANGE,
	PROTOCOL_GKINGTRYCALLGUARDS,
	PROTOCOL_GKINGISSUETASK,
	PROTOCOL_GKINGGETREWARD,
	PROTOCOL_SYNCBATHTIMES,
	PROTOCOL_ISCONNALIVE,
	PROTOCOL_S2CLINEBROADCAST,
	RPC_FLOWERGIFTTAKE,
	RPC_FLOWERTAKEOFF,
	PROTOCOL_GTOUCHPOINTEXCHANGE,
	RPC_GETCASHAVAIL,
};

GNET::Protocol::Manager::Session::State state_GProviderDeliveryServer(_state_GProviderDeliveryServer,
						sizeof(_state_GProviderDeliveryServer)/sizeof(GNET::Protocol::Type), 120);

static GNET::Protocol::Type _state_GameDBClient[] = 
{
	RPC_PASSPORTGETROLELIST,
	PROTOCOL_DELROLEANNOUNCE,
	RPC_DBCREATEFACBASE,
	RPC_GETFACBASE,
	RPC_PUTFACBASE,
	RPC_DBSAVEWEBORDER,
	RPC_GETCASHAVAIL,
	RPC_DBCREATEROLE,
	RPC_DBDELETEROLE,
	RPC_DBUNDODELETEROLE,
	RPC_DBDELETEFACTION,
	RPC_DBUNDELETEFACTION,
	RPC_DBDELETEFAMILY,
	RPC_DBUNDELETEFAMILY,
	RPC_PUTUSER,
	RPC_GETUSER,
	RPC_DELUSER,
	RPC_GETROLE,
	RPC_DELROLE,
	RPC_DELROLE,
	RPC_PUTROLEBASE,
	RPC_UPDATEROLEBASE,
	RPC_GETROLEBASE,
	RPC_GETROLEINFO,
	RPC_PUTROLESTATUS,
	RPC_GETROLESTATUS,
	RPC_PUTROLEPOCKET,
	RPC_GETROLEPOCKET,
	RPC_PUTROLETASK,
	RPC_GETROLETASK,
	RPC_TRADEINVENTORY,
	RPC_TRADESAVE,
	RPC_PUTROLE,
	RPC_GETMONEYINVENTORY,
	RPC_PUTMONEYINVENTORY,
	RPC_GETROLEBASESTATUS,
	RPC_PUTROLESTOREHOUSE,
	RPC_GETROLESTOREHOUSE,
	RPC_PUTROLEFORBID,
	RPC_GETROLEFORBID,
	RPC_GETROLEID,
	RPC_GETFRIENDLIST,
	RPC_PUTFRIENDLIST,
	RPC_PUTMESSAGE,
	RPC_GETMESSAGE,
	RPC_DBFRIENDCALLBACKINFO,
	RPC_DBFRIENDCALLBACKMAIL,
	RPC_DBUPDATECALLBACKINFO,
	RPC_DBCHECKAWARD,
	RPC_DBFRIENDCALLBACKSUBSCRIBE,
	RPC_GETUSERROLES,
	RPC_CANCHANGEROLENAME,
	RPC_CLEARSTOREHOUSEPASSWD,
	RPC_RENAMEROLE,
	RPC_UID2LOGICUID,
	RPC_ROLEID2UID,
	RPC_GETTASKDATARPC,
	RPC_DBSETCASHPASSWORD,
	RPC_PUTTASKDATARPC,
	RPC_TRANSACTIONACQUIRE,
	RPC_TRANSACTIONABORT,
	RPC_TRANSACTIONCOMMIT,
	RPC_DBVERIFYMASTER,
	RPC_DBGETMAILLIST,
	RPC_DBGETMAIL,
	RPC_DBGETMAILATTACH,
	RPC_DBSETMAILATTR,
	RPC_DBSENDMAIL,
	RPC_DBDELETEMAIL,
	RPC_DBGETMAILALL,
	RPC_DBAUCTIONOPEN,
	RPC_DBAUCTIONBID,
	RPC_DBAUCTIONGET,
	RPC_DBAUCTIONCLOSE,
	RPC_DBAUCTIONLIST,
	RPC_DBAUCTIONTIMEOUT,
	RPC_GETADDCASHSN,
	RPC_CASHSERIAL,
	PROTOCOL_ADDCASH,
	PROTOCOL_ADDCASH_RE,
	RPC_DBCRSSVRTEAMSGET,
	RPC_DBCRSSVRTEAMSCREATE,
	RPC_DBCRSSVRTEAMSRENAME,
	RPC_DBCRSSVRTEAMSDEL,
	RPC_DBCRSSVRTEAMSJOIN,
	RPC_DBCRSSVRTEAMSCHGCAPTAIN,
	RPC_DBCRSSVRTEAMSLEAVE,
	RPC_DBCRSSVRTEAMSROLEPUNISHSCORE,
	RPC_DBCRSSVRTEAMSGETSEASONTOP,
	RPC_DBCRSSVRTEAMSSEASONTOPSAVE,
	RPC_DBCROSSCRSSVRTEAMS,
	RPC_DBCROSSCRSSVRTEAMSDISMISS,
	RPC_DBCROSSCRSSVRTEAMSRENAME,
	RPC_DBCROSSCRSSVRTEAMSSEARCH,
	RPC_SYNCCRSSVRTEAMSSCORE,
	RPC_SYNCCRSSVRTEAMSROLE,
	RPC_SYNCCRSSVRTEAMS,
	RPC_DBFACTIONADD,
	RPC_DBFACTIONDEL,
	RPC_DBFACTIONJOIN,
	RPC_DBFACTIONGET,
	RPC_DBFACTIONLEAVE,
	RPC_DBFACTIONSYNC,
	RPC_DBFACTIONUPDATE,
	RPC_DBFACTIONBRIEF,
	RPC_DBFACTIONAPPOINT,
	RPC_DBFACTIONRENAME,
	RPC_DBSECTJOIN,
	RPC_DBSECTQUIT,
	RPC_DBSECTGET,
	RPC_DBSECTUPDATE,
	RPC_DBFAMILYADD,
	RPC_DBFAMILYGET,
	RPC_DBFAMILYDEL,
	RPC_DBFAMILYJOIN,
	RPC_DBFAMILYLEAVE,
	RPC_DBFAMILYUPDATE,
	RPC_DBFAMILYAPPOINT,
	RPC_DBFAMILYSYNC,
	RPC_DBHOSTILEADD,
	RPC_DBHOSTILEDELETE,
	RPC_DBHOSTILEUPDATE,
	RPC_DBHOSTILEPROTECT,
	RPC_DBFACTIONADDMONEY,
	RPC_DBFACTIONWITHDRAWMONEY,
	RPC_DBPUTTOPTABLE,
	RPC_DBGETTOPTABLE,
	PROTOCOL_UPDATETOPTABLE,
	RPC_PUTSPOUSE,
	RPC_DBSTOCKLOAD,
	RPC_DBSTOCKTRANSACTION,
	RPC_DBSTOCKBALANCE,
	RPC_DBSTOCKCOMMISSION,
	RPC_DBSTOCKCANCEL,
	RPC_DBAUTOLOCKSET,
	RPC_DBSIEGELOAD,
	RPC_DBSIEGEEND,
	RPC_DBSIEGESETASSISTANT,
	RPC_DBSIEGECHALLENGE,
	RPC_DBINSTANCINGACTIVATE,
	RPC_DBHOMETOWNEXCHGMONEY,
	RPC_DBSIEGEGET,
	RPC_DBSIEGEPUTDETAIL,
	RPC_DBBATTLEJOIN,
	PROTOCOL_DOMAINCMD_RE,
	RPC_QUERYUSERID,
	RPC_FORBIDUSER,
	RPC_DBFORBIDUSER,
	RPC_DBGETCOMMONDATA,
	RPC_DBPUTCOMMONDATA,
	RPC_DBSNSGETPLAYERINFO,
	RPC_DBSNSGETPLAYERINFOCACHE,
	RPC_DBSNSUPDATEROLEINFO,
	RPC_DBSNSGETROLEINFO,
	RPC_DBSNSUPDATEPLAYERINFO,
	RPC_DBSNSGETMESSAGE,
	RPC_DBSNSUPDATEMESSAGE,
	RPC_DBREFGETREFERRAL,
	RPC_DBREFGETREFERRER,
	RPC_DBREFUPDATEREFERRER,
	RPC_DBREFUPDATEREFERRAL,
	RPC_DBREFWITHDRAWTRANS,
	RPC_DBCHANGEPROFILE,
	RPC_DBCLEARCONSUMABLE,
	RPC_DBHOMETOWNGETINFO,
	RPC_DBHOMETOWNPUTINFO,
	RPC_DBHOMETOWNPUTCOMPOINFO,
	RPC_DBFUNGAMEGETINFO,
	RPC_DBFUNGAMEGETSCORE,
	RPC_DBFUNGAMEPUTINFO,
	RPC_DBFUNGAMEPUTSCORE,
	RPC_DBTERRITORYLISTLOAD,
	RPC_DBTERRITORYLISTSAVE,
	RPC_DBTERRITORYCHALLENGE,
	RPC_DBTERRITORYSAVE,
	RPC_DBUNIQUEAUCTIONLOAD,
	RPC_DBUNIQUEAUCTIONSAVE,
	RPC_MODIFYROLECREATETIME,
	RPC_DBCIRCLEGET,
	RPC_DBCIRCLEHISGET,
	RPC_DBCIRCLEJOIN,
	RPC_DBCIRCLEAPPOINT,
	RPC_DBCIRCLESYNC,
	RPC_DBCONSIGNTABLELOAD,
	RPC_DBCONSIGNPREPOST,
	RPC_DBCONSIGNQUERY,
	RPC_DBCONSIGNFAIL,
	RPC_DBCONSIGNPOST,
	RPC_DBCONSIGNPRECANCEL,
	RPC_DBCONSIGNMAIL,
	RPC_DBCONSIGNGETROLESIMPLEINFO,
	RPC_DBCONSIGNSHELFCANCEL,
	RPC_DBCONSIGNSHELF,
	RPC_DBCONSIGNSOLD,
	RPC_DBCONSIGNQUERYBACKUP,
	RPC_DBCONSIGNPREPOSTROLE,
	RPC_DBCONSIGNSOLDROLE,
	RPC_DBCONSIGNGETROLE,
	RPC_DBCONSIGNROLEFAIL,
	RPC_FETCHPLAYERDATA,
	RPC_ACTIVATEPLAYERDATA,
	RPC_TOUCHPLAYERDATA,
	RPC_SAVEPLAYERDATA,
	RPC_FREEZEPLAYERDATA,
	RPC_PLAYERIDENTITYMATCH,
	RPC_DELPLAYERDATA,
	RPC_GETCASHTOTAL,
	RPC_DBRAIDOPEN,
	RPC_ADDGLOBALCOUNTER,
	RPC_GETGLOBALCOUNTER,
	RPC_DBCHANGEROLENAME,
	RPC_DBROLENAMELIST,
	RPC_DBQUERYROLENAME,
	RPC_DBCHANGEFACTIONNAME,
	RPC_DBKINGDOMLOAD,
	RPC_DBKINGDOMSETWINNER,
	RPC_DBKINGDOMSAVE,
	RPC_DBKINGDOMAPPOINT,
	RPC_DBKINGDOMDISCHARGE,
	PROTOCOL_UPDATEQUEEN,
	RPC_DBKINGDOMTASKISSUE,
	RPC_DBKINGDOMTASKCLOSE,
	RPC_DBKINGDOMTASKEND,
	RPC_DBKINGGETREWARD,
	RPC_DBGETTOPFLOWERDATA,
	RPC_DBUPDATETOPFLOWERDATA,
	RPC_DBFLOWERGETROLEID,
	RPC_DBFLOWERGETROLEEXIST,
	RPC_DBGETTOUCHORDER,
	RPC_OPERATIONCMD,
	RPC_DBGETSERVICEFORBIDCMD,
	RPC_DEBUGOPERATIONCMD,
};

GNET::Protocol::Manager::Session::State state_GameDBClient(_state_GameDBClient,
						sizeof(_state_GameDBClient)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_UniqueNameClient[] = 
{
	RPC_PRECREATEROLE,
	RPC_POSTCREATEROLE,
	RPC_POSTDELETEROLE,
	RPC_PRECREATEFACTION,
	RPC_POSTCRSSVRTEAMSCREATE,
	RPC_POSTCRSSVRTEAMSRENAME,
	RPC_POSTCRSSVRTEAMSDEL,
	RPC_PRECRSSVRTEAMSCREATE,
	RPC_PRECRSSVRTEAMSRENAME,
	RPC_POSTCREATEFACTION,
	RPC_POSTDELETEFACTION,
	RPC_PRECREATEFAMILY,
	RPC_POSTCREATEFAMILY,
	RPC_POSTDELETEFAMILY,
	RPC_PRECHANGEROLENAME,
	RPC_POSTCHANGEROLENAME,
	RPC_PRECHANGEFACTIONNAME,
	RPC_POSTCHANGEFACTIONNAME,
};

GNET::Protocol::Manager::Session::State state_UniqueNameClient(_state_UniqueNameClient,
						sizeof(_state_UniqueNameClient)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_GRoleDBClient[] = 
{
	RPC_ACCOUNTADDROLE,
	RPC_ACCOUNTDELROLE,
};

GNET::Protocol::Manager::Session::State state_GRoleDBClient(_state_GRoleDBClient,
						sizeof(_state_GRoleDBClient)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_ACClient[] = 
{
	PROTOCOL_ACREMOTECODE,
	PROTOCOL_ACQUESTION,
	PROTOCOL_GMKICKOUTUSER,
	PROTOCOL_ACKICKOUTUSER,
	PROTOCOL_ACREPORTCHEATER,
	PROTOCOL_ACFORBIDUSER,
	PROTOCOL_ACACCUSE_RE,
};

GNET::Protocol::Manager::Session::State state_ACClient(_state_ACClient,
						sizeof(_state_ACClient)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_GWebTradeClient[] = 
{
	PROTOCOL_POST_RE,
	PROTOCOL_GAMEPOSTCANCEL_RE,
	PROTOCOL_WEBPOSTCANCEL,
	PROTOCOL_SHELF,
	PROTOCOL_SHELFCANCEL,
	PROTOCOL_SOLD,
	PROTOCOL_POSTEXPIRE,
	PROTOCOL_WEBGETROLELIST,
	RPC_QUERYUSERID,
};

GNET::Protocol::Manager::Session::State state_GWebTradeClient(_state_GWebTradeClient,
						sizeof(_state_GWebTradeClient)/sizeof(GNET::Protocol::Type), 86400);

static GNET::Protocol::Type _state_CentralDeliveryClient[] = 
{
	PROTOCOL_LOADEXCHANGE,
	PROTOCOL_DSANNOUNCEIDENTITY,
	PROTOCOL_SENDDATAANDIDENTITY,
	PROTOCOL_SENDDATAANDIDENTITY_RE,
	PROTOCOL_REMOTELOGINQUERY,
	PROTOCOL_REMOTELOGOUT,
	PROTOCOL_KICKOUTREMOTEUSER_RE,
	PROTOCOL_ACKICKOUTUSER,
	PROTOCOL_ACFORBIDUSER,
	PROTOCOL_GMKICKOUTUSER,
	PROTOCOL_SENDREFLEVELUP,
	PROTOCOL_GMSHUTUP,
	PROTOCOL_GETREMOTEROLEINFO_RE,
	PROTOCOL_BATTLEGETFIELD_RE,
	PROTOCOL_BATTLEENTER_RE,
	PROTOCOL_BATTLELEAVE_RE,
	PROTOCOL_BATTLESTARTNOTIFY,
	PROTOCOL_OPENBANQUETDRAG_RE,
	PROTOCOL_OPENBANQUETJOIN_RE,
	PROTOCOL_OPENBANQUETQUEUECOUNT_RE,
	PROTOCOL_CROSSBATTLEJOINTEAM,
	PROTOCOL_GETTOPTABLE_RE,
	PROTOCOL_RAIDENTER_RE,
	PROTOCOL_CROSSRAIDENTER_RE,
	PROTOCOL_RAIDSTART_RE,
	PROTOCOL_RAIDSTARTNOTIFY,
	PROTOCOL_TEAMRAIDMAPPINGSUCCESS,
	PROTOCOL_HIDESEEKMAPPINGSUCCESS,
	PROTOCOL_TEAMRAIDAPPLYNOTICE,
	PROTOCOL_TEAMRAIDACTIVEAPPLYNOTICE,
	PROTOCOL_RAIDKICKNOTIFY,
	PROTOCOL_TEAMRAIDAPPLY_RE,
	PROTOCOL_RAIDQUIT_RE,
	PROTOCOL_CROSSCRSSVRTEAMSONLINESTATUS,
	PROTOCOL_CROSSCRSSVRTEAMSGETREQ,
	PROTOCOL_CRSSVRTEAMSSEARCH_RE,
	RPC_SYNCCROSSCRSSVRTEAMSSCORE,
	RPC_SYNCCROSSCRSSVRTEAMSTOP,
	RPC_SYNCCROSSCRSSVRTEAMSENDBRIEF,
	RPC_SYNCCROSSCRSSVRTEAMS,
	RPC_SYNCCROSSCRSSVRTEAMSUPDATE,
	RPC_SYNCCROSSCRSSVRTEAMSADD,
	RPC_SYNCCROSSCRSSVRTEAMSDEL,
	RPC_SYNCCROSSCRSSVRTEAMSRENAME,
	RPC_SYNCCROSSCRSSVRTEAMSCHGCAPTAIN,
	RPC_SYNCCROSSCRSSVRTEAMSDISMISS,
	RPC_CROSSCRSSVRTEAMSGETTOPTABLE,
};

GNET::Protocol::Manager::Session::State state_CentralDeliveryClient(_state_CentralDeliveryClient,
						sizeof(_state_CentralDeliveryClient)/sizeof(GNET::Protocol::Type), 120);

static GNET::Protocol::Type _state_CentralDeliveryServer[] = 
{
	PROTOCOL_LOADEXCHANGE,
	PROTOCOL_DSANNOUNCEIDENTITY,
	PROTOCOL_SENDDATAANDIDENTITY,
	PROTOCOL_SENDDATAANDIDENTITY_RE,
	PROTOCOL_REMOTELOGINQUERY_RE,
	PROTOCOL_KICKOUTREMOTEUSER,
	PROTOCOL_GETREMOTEROLEINFO,
	PROTOCOL_ADDICTIONCONTROL,
	PROTOCOL_VIPANNOUNCE,
	PROTOCOL_BATTLEGETFIELD,
	PROTOCOL_CROSSBATTLEJOINTEAM,
	PROTOCOL_BATTLEENTER,
	PROTOCOL_COPENBANQUETJOIN,
	PROTOCOL_OPENBANQUETENTER,
	PROTOCOL_OPENBANQUETLOGOUT,
	PROTOCOL_OPENBANQUETCANCEL,
	PROTOCOL_OPENBANQUETQUEUECOUNT,
	PROTOCOL_BATTLELEAVE,
	PROTOCOL_TEAMRAIDMAPPINGSUCCESS_RE,
	PROTOCOL_HIDESEEKMAPPINGSUCCESS_RE,
	PROTOCOL_GRAIDTEAMAPPLY,
	PROTOCOL_GHIDESEEKRAIDAPPLY,
	PROTOCOL_GFENGSHENRAIDAPPLY,
	PROTOCOL_CRSSVRTEAMSSEARCH,
	PROTOCOL_TEAMRAIDQUIT,
	PROTOCOL_HIDESEEKRAIDQUIT,
	RPC_SYNCCROSSCRSSVRTEAMSSCORE,
	RPC_SYNCCROSSCRSSVRTEAMSTOP,
	RPC_SYNCCROSSCRSSVRTEAMSENDBRIEF,
	RPC_SYNCCROSSCRSSVRTEAMS,
	RPC_SYNCCROSSCRSSVRTEAMSUPDATE,
	RPC_SYNCCROSSCRSSVRTEAMSADD,
	RPC_SYNCCROSSCRSSVRTEAMSDEL,
	RPC_SYNCCROSSCRSSVRTEAMSRENAME,
	RPC_SYNCCROSSCRSSVRTEAMSCHGCAPTAIN,
	RPC_CROSSCRSSVRTEAMSGETTOPTABLE,
	RPC_SYNCCROSSCRSSVRTEAMSDISMISS,
};

GNET::Protocol::Manager::Session::State state_CentralDeliveryServer(_state_CentralDeliveryServer,
						sizeof(_state_CentralDeliveryServer)/sizeof(GNET::Protocol::Type), 120);

static GNET::Protocol::Type _state_GameGateServer[] = 
{
	PROTOCOL_GETKDCTOKEN_RE,
	PROTOCOL_GATEONLINELIST_RE,
	PROTOCOL_GATEGETGTROLESTATUS_RE,
	PROTOCOL_GATEUPDATESTATUS,
	PROTOCOL_GATEOFFLINECHAT,
	PROTOCOL_PRIVATECHAT,
	PROTOCOL_GATEFACTIONCHAT,
	RPC_GATEGETROLERELATION,
};

GNET::Protocol::Manager::Session::State state_GameGateServer(_state_GameGateServer,
						sizeof(_state_GameGateServer)/sizeof(GNET::Protocol::Type), 86400);


};

