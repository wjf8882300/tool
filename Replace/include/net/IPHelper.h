#ifndef IPHelper_H_
#define IPHelper_H_

#include "Poco/Net/DNS.h"
#include <string>

using std::string;

class IPHelper
{
public:
static string GetHostName();

static string GetHostIPAddress();

static string GetMacAddress();
};


#endif /*IPHelper_H_*/



