-------------------------------------------------------------------------------------------------------------
��־�����Ͳ�ѯ�����ӿ�˵��
-------------------------------------------------------------------------------------------------------------
(1)��������
	1.�̳г���Index,ʵ���Լ�����������(index.hpp).
	2.����IndexMaker ���� add(), void IndexMaker::add(Index * idx);
	3.����IndexMaker ���� make().
	3.�������
	4.�����в�������˵��:
		demoindexmaker application.xml file_list mail|tail
		application.xml	Ӧ��xml�����ļ�(��Ĭ��Ŀ¼)
		file_list	Դ��־�ļ��б�(tailʱֻ��Ӧһ��), file_listһ����""������,
				���file_listΪ��,�������application.xml����,������ӽ���index��type�ҵ���Ӧ��logname�ļ���,
					��������ļ�����ת��Ŀ¼��������ƥ�����־�ļ�.
		mail		�����������˳�(Դ�ļ�Ϊת��Ŀ¼�µ���־�ļ�),����ļ��б��ʾ: "file1 file2 file3"
		tail		����ʵʱ����,ѭ���ȴ�(Դ�ļ�Ϊlogserver���ڼ�¼����־�ļ�)
	
	5.�����ļ�������:
		A.ִ��ת�������־�ļ�������	serial_logname.startime.type.keyname.idx
			����:��־�ļ�00005_login.20070126160342.log
			��������:Э������(type)Ϊ2004, �ؼ�������(keyname)Ϊlogintime
			����Ӧ����־�����ļ���:
				00005_login.20070126160342.2004.logintime.idx
		B.��logserver��ǰ���ڶ�д���ļ���������(ʵʱ����)	logname.starttime.type.keyname.idx
			����:��־�ļ�login.log
			��������:Э������(type)Ϊ2004, �ؼ�������(keyname)Ϊlogintime
			����Ӧ����־�����ļ���:
				login.20070126160342.2004.logintime.idx
	6.ʵʱ��������:
		�����в���Ϊtail��Ϊʵʱ����,��ʱ���ļ��б�Ϊһ��logserver��ǰ���ڼ�¼��һ���ļ�
		�����������ļ�β���˳�,ʵʱ�����־�ļ�
		�������յ�ת�������͵��ź�ʱ,����ǰѭ������,�������û���,���´�ʵʱ��־�ļ�
		ʵʱ����ʹ��ʱ���Կ�����logserver�ķ�ʽ��̨����,��/etc/rc.d/ �¼�Kxx ��Sxx ������ֹͣ����.ֹͣ�����ź�2.(���Ϊtail,��ע��SIGINT�ź�)
			
(2)������ѯ
	1.�̳г���query,ʵ���Լ��Ĳ�ѯ����(query.hpp).
	2.����IndexQuery�෽�� add,void IndexQuery::add(Query * query);
	3.����IndexQuery�෽�� go().
	3.�������
	4.�����в�������˵��:
		demoindexquery application.xml 
	5.������ѯ����:
		����query���ṩ��type��keyname,timerang,������Ŀ¼���ҵ�����ƥ���idx�ļ�,�ٸ���key,����Ӧ��keyvalue,�����û��ṩ�Ļص�process_protocol����.
		�����û���ҵ�,ȷ��query���enable_scan_if_not_found_idx�����Ƿ�Ϊtrue,
			����ṩĿ����־�ļ���,��������ļ�scan�ļ�;
			���û���ṩ,����������ļ�ȡһ����query->get_type()��ƥ�����־�ļ��б�,������־ת��Ŀ¼��ʵʱ��¼Ŀ¼����־�ļ�
		����и�����־�ļ���˳��ɨ���¼,�����û��ṩ�Ļص�process_protocol����.
	6.�������ṩͬʱ�Զ��query��ѯ�Ĺ���,	����һ����ѯ��Ӧһ��query

(3)��Ҫϵͳ����lsof��logserverִ�г����ļ���

ndex,ʵ���Լ�����������(index.hpp).
	2.����IndexMaker ���� add(), void IndexMaker::add(Index * idx);
	3.����IndexMaker ���� make().
	3.�������
	4.�����в�������˵��:
		demoindexmaker application.xml file_list mail|tail
		application.xml	Ӧ��xml�����ļ�(��Ĭ��Ŀ¼)
		file_list	Դ��־�ļ��б�(tailʱֻ��Ӧһ��), file_listһ����""������,
				���file_listΪ��,�������application.xml����,������ӽ���index��type�ҵ���Ӧ��logname�ļ���,
					��������ļ�����ת��Ŀ¼��������ƥ�����־�ļ�.
		mail		�����������˳�(Դ�ļ�Ϊת��Ŀ¼�µ���־�ļ�),����ļ��б��ʾ: "file1 file2 file3"
		tail		����ʵʱ����,ѭ���ȴ�(Դ�ļ�Ϊlogserver���ڼ�¼����־�ļ�)
	
	5.�����ļ�������:
		A.ִ��ת�������־�ļ�������	serial_logname.startime.type.keyname.idx
			����:��־�ļ�00005_login.20070126160342.log
			��������:Э������(type)Ϊ2004, �ؼ�������(keyname)Ϊlogintime
			����Ӧ����־�����ļ���:
				00005_login.20070126160342.2004.logintime.idx
		B.��logserver��ǰ���ڶ�д���ļ���������(ʵʱ����)	logname.starttime.type.keyname.idx
			����:��־�ļ�login.log
			��������:Э������(type)Ϊ2004, �ؼ�������(keyname)Ϊlogintime
			����Ӧ����־�����ļ���:
				login.20070126160342.2004.logintime.idx
	6.ʵʱ��������:
		�����в���Ϊtail��Ϊʵʱ����,��ʱ���ļ��б�Ϊһ��logserver��ǰ���ڼ�¼��һ���ļ�
		�����������ļ�β���˳�,ʵʱ�����־�ļ�
		�������յ�ת�������͵��ź�ʱ,����ǰѭ������,�������û���,���´�ʵʱ��־�ļ�
		ʵʱ����ʹ��ʱ���Կ�����logserver�ķ�ʽ��̨����,��/etc/rc.d/ �¼�Kxx ��Sxx ������ֹͣ����.ֹͣ�����ź�2.(���Ϊtail,��ע��SIGINT�ź�)
			
(2)������ѯ
	1.�̳г���query,ʵ���Լ��Ĳ�ѯ����(query.hpp).
	2.����IndexQuery�෽�� add,void IndexQuery::add(Query * query);
	3.����IndexQuery�෽�� go().
	3.�������
	4.�����в�������˵��:
		demoindexquery application.xml 
	5.������ѯ����:
		����query���ṩ��type��keyname,timerang,������Ŀ¼���ҵ�����ƥ���idx�ļ�,�ٸ���key,����Ӧ��keyvalue,�����û��ṩ�Ļص�process_protocol����.
		�����û���ҵ�,ȷ��query���enable_scan_if_not_found_idx�����Ƿ�Ϊtrue,
			����ṩĿ����־�ļ���,��������ļ�scan�ļ�;
			���û���ṩ,����������ļ�ȡһ����query->get_type()��ƥ�����־�ļ��б�,������־ת��Ŀ¼��ʵʱ��¼Ŀ¼����־�ļ�
		����и�����־�ļ���˳��ɨ���¼,�����û��ṩ�Ļص�process_protocol����.
	6.�������ṩͬʱ�Զ��query��ѯ�Ĺ���,	����һ����ѯ��Ӧһ��query

(3)��Ҫϵͳ����lsof��logserverִ�г����ļ���

(4)����ṩ�ļ��б���д���,�ļ���Ŀ¼�Ĺ���һ���þ���·��(�ļ���)

-------------------------------------------------------------------------------------------------------------
io�����˵��
-------------------------------------------------------------------------------------------------------------
	1.io/protocol.h
	���� private: ��Ϊprotected;	
		 protected:
                        struct Stream : public Marshal::OctetsStream
                        {
                                Session *session;
                                mutable bool check_policy;
                                Stream(Session *s) : session(s), check_policy(true) { }
                        };      
                        Session(Manager *m) : sid(session_id++), is(this), os_bytes(0), manager(m), state(m->GetInitState()){ }                     
                        Session(const Session &rhs) : NetSession(rhs), sid(session_id++), is(this), os_bytes(rhs.os_bytes), 
					manager(rhs.manager), state(rhs.state) { }
                        typedef gnet_map<ID, Session *> Map;
                        static ID session_id;
                        ID sid;
                        Stream is;
                        std::deque<Octets>os;
                        size_t os_bytes;
                        Manager *manager;
                        const State *state;
                        Timer timer;
                        NetSession *Clone () const      { return new Session(*this); }
	
