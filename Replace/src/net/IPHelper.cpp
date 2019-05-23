#include "net/IPHelper.h"
#include <sstream>

string IPHelper::GetHostName()
{
	std::ostringstream ostr;
	Poco::Net::HostEntry he = Poco::Net::DNS::thisHost();
	ostr << he.name() << "\r\n";
	return ostr.str();
}

string IPHelper::GetHostIPAddress()
{
	std::ostringstream ostr;
	Poco::Net::HostEntry he = Poco::Net::DNS::thisHost();
	Poco::Net::HostEntry::AddressList list = he.addresses();
	Poco::Net::HostEntry::AddressList::iterator it = list.begin();
	for( ; it != list.end(); it ++)
	{
		ostr << (*it).toString() << "\r\n";	
	}
	return ostr.str();
}

#include <Windows.h>
#include <iphlpapi.h>
#pragma comment(lib,"iphlpapi.lib")
string IPHelper::GetMacAddress()
{
	std::ostringstream ostr;	
	IP_ADAPTER_INFO adapter[5]; //Maximum 5 adapters
	DWORD buflen = sizeof(adapter);
	DWORD status = GetAdaptersInfo(adapter,&buflen);
	BYTE s[8];
	char szMac[256];
	int len = 0;
	if(status == ERROR_SUCCESS)
	{
		PIP_ADAPTER_INFO painfo=adapter;
		memcpy(s, painfo->Address, 6);
		sprintf(szMac, "%02X-%02X-%02X-%02X-%02X-%02X", s[0],s[1],s[2],s[3],s[4],s[5]);
	}
	ostr << string(szMac)  << "\r\n";
	return ostr.str();
}


