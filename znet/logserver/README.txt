-------------------------------------------------------------------------------------------------------------
��־�����ʹ��˵��
-------------------------------------------------------------------------------------------------------------
	1.�����Ǽ���־����������: logserver logger.conf application.xml
	2.����ת������������: logrotate logger.conf application.xml
		ת���ļ�������:
			serial_logname.starttime.log
			����: ��־�ļ�login.log,ִ��ת����Ϊ
				00005_login.20070126160342.log
				00005				��ʾ���ļ���ת��Ŀ¼�е�serial
				20070126160342	��ʾ���ļ����ݵĿ�ʼʱ��
			
		����־�ļ��ﵽת��������,������:
			1.ת��������ҵ�����־�ļ��򿪵�pid(��logserver��)
			2.��һ������Э���logserver,֪ͨlogserver,��logsrever�½�һ����־�ļ�
			3.����pid����־����Ŀ¼�ҵ����pid�򿪵�����ʵʱ�����ļ�,����serial,��������Щʵʱ�����ļ�Ϊ���������ļ�
			4.��һ���źŸ���Щpid,֪ͨ�ļ��Ѿ�ת��.
			5.��־�ļ�С��LOG_PAGESIZE��ת��

		serial�Ĳ���,��ת��Ŀ¼Ϊ׼

	3.�������ͣ����
chkconfig --add logserver
#!/bin/bash
#
# logserver        Starts logserver.
#
#
# chkconfig: 345 80 05
# description: logserver  
### BEGIN INIT INFO
# Provides: $logserver
### END INIT INFO

# Source function library.
. /etc/init.d/functions

#[ -f /sbin/syslogd ] || exit 0
#[ -f /sbin/klogd ] || exit 0

# Source config

RETVAL=0
LOGSERVER_HOME=/home/lijinhua
LOGSERVER=logserver
USER_NAME=lijinhua

if [ ! -f $LOGSERVER_HOME/logsysv3/logserver/logserver ]
then
        echo "Logserver startup: cannot start"
exit
fi

start() {
        echo -n $"Starting system logger: "
        daemon su - $USER_NAME -c $LOGSERVER_HOME/do.sh
        #daemon su - $USER_NAME -c "$LOGSERVER_HOME/logsysv3/logserver/logserver \
        #                       $LOGSERVER_HOME/logsysv3/logger.conf $LOGSERVER_HOME/logsysv3/application.xml"
        RETVAL=$?
        return $RETVAL
}

stop() {
        echo -n $"Shutting down logserver: "
        killproc $LOGSERVER -2
        echo
        RETVAL=$?
        return $RETVAL
}
restart() {
        stop
        start
}
case "$1" in
  start)
        start
        ;;
  stop) 
        stop
        ;;
  restart)
        restart
        ;;
  *)    
        echo $"Usage: $0 {start|stop|restart|}"
        exit 1
esac

exit $?

	4.��־����˴�����Ҫ����io��,���������
	5.��Ҫ����һ��Э��id��Ϊ����Э��id, ���ڳ����ݶ�type=12000
	6.��־�����ִ���ļ�Ϊlogserver,���ܸ���,����ģ������

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
                                Stream(Session *s) : session(s),
check_policy(true) { }
                        };      
                        Session(Manager *m) : sid(session_id++), is(this),
os_bytes(0), manager(m), state(m->GetInitState()){ }                     
                        Session(const Session &rhs) : NetSession(rhs),
sid(session_id++), is(this), os_bytes(rhs.os_bytes), 
					manager(rhs.manager), state(rhs.state)
{ }
                        typedef gnet_map<ID, Session *> Map;
                        static ID session_id;
                        ID sid;
                        Stream is;
                        std::deque<Octets>os;
                        size_t os_bytes;
                        Manager *manager;
                        const State *state;
                        Timer timer;
                        NetSession *Clone () const      { return new
Session(*this); }
