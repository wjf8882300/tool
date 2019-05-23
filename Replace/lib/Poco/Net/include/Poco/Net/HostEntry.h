//
// HostEntry.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/HostEntry.h#1 $
//
// Library: Net
// Package: NetCore
// Module:  HostEntry
//
// Definition of the HostEntry class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Net_HostEntry_INCLUDED
#define Net_HostEntry_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketDefs.h"
#include "Poco/Net/IPAddress.h"
#include <vector>


namespace Poco {
namespace Net {


class Net_API HostEntry
	/// This class stores information about a host
	/// such as host name, alias names and a list
	/// of IP addresses.
{
public:
	typedef std::vector<std::string> AliasList;
	typedef std::vector<IPAddress>   AddressList;
	
	HostEntry();
		/// Creates an empty HostEntry.
		
	HostEntry(struct hostent* entry);
		/// Creates the HostEntry from the data in a hostent structure.

#if defined(_WIN32) && defined(POCO_HAVE_IPv6)
	HostEntry(struct addrinfo* info);
		/// Creates the HostEntry from the data in a Windows addrinfo structure.
#endif

	HostEntry(const HostEntry& entry);
		/// Creates the HostEntry by copying another one.

	HostEntry& operator = (const HostEntry& entry);
		/// Assigns another HostEntry.

	void swap(HostEntry& hostEntry);
		/// Swaps the HostEntry with another one.	

	~HostEntry();
		/// Destroys the HostEntry.

	const std::string& name() const;
		/// Returns the canonical host name.

	const AliasList& aliases() const;
		/// Returns a vector containing alias names for
		/// the host name.

	const AddressList& addresses() const;
		/// Returns a vector containing the IPAddresses
		/// for the host.

private:
	std::string _name;
	AliasList   _aliases;
	AddressList _addresses;
};


//
// inlines
//
inline const std::string& HostEntry::name() const
{
	return _name;
}


inline const HostEntry::AliasList& HostEntry::aliases() const
{
	return _aliases;
}


inline const HostEntry::AddressList& HostEntry::addresses() const
{
	return _addresses;
}


inline void swap(HostEntry& h1, HostEntry& h2)
{
	h1.swap(h2);
}


} } // namespace Poco::Net


#endif // Net_HostEntry_INCLUDED
