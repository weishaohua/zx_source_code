-------------------------------------------------------------------------------------------------------------
��־�ͻ���ʹ��˵��
-------------------------------------------------------------------------------------------------------------
	ʹ�÷���:
		1.����������conf�����ļ������

[Log2UdpClient]
type                    =       udp
port                    =       23568
address                 =       172.31.2.39
so_sndbuf               =       16384
so_rcvbuf               =       16384
ibuffermax              =       16384
obuffermax              =       1048576
accumulate              =       1048576

[Log2TcpClient]
type                    =       tcp
port                    =       23568
address                 =       172.31.2.39
so_sndbuf               =       16777216
so_rcvbuf               =       16384
accumulate              =       16777216
ibuffermax              =       16384
obuffermax              =       1048576
accumulate              =       1048576


		�޸���logserverƥ��Ķ˿ں�ip��ַ

		2.��������include "logger.hpp"�ļ�

		3.��ʼ������־������������
			����conf�����ļ����ʼ������ 
			LOG::Log_Init();

		4.������־�ӿں���
        		void LOG::Log(const GNET::Protocol *protocol, bool reliability = true);
		        void LOG::Log(const GNET::Protocol &protocol, bool reliability = true);
			reliability=trueΪͨ��tcp���Ӵ���

		5.���ֿռ�ΪLOG,���ļ���ΪlibLogClient.a
