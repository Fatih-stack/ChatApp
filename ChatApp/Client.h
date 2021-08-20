#pragma once
#include <afxsock.h> 
#include <string>

class Client
{
public:
	SOCKET m_socketId;
	SOCKADDR_IN m_ServerAddr{};
	static bool m_isInitWsa, isFile;
	CString display;
	Client();
	void setClientSocket(std::string ip, int port);
	virtual ~Client();
	static bool initWSA();
	void destroyWSA();
	bool receiveMSG();
	int sendMSG(const char* buffer) const;
	void Connect() const;
};

