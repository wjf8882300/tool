//
// HTTPServerConnection.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/HTTPServerConnection.h#1 $
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerConnection
//
// Definition of the HTTPServerConnection class.
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


#ifndef Net_HTTPServerConnection_INCLUDED
#define Net_HTTPServerConnection_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"


namespace Poco {
namespace Net {


class HTTPServerSession;


class Net_API HTTPServerConnection: public TCPServerConnection
	/// This subclass of TCPServerConnection handles HTTP
	/// connections.
{
public:
	HTTPServerConnection(const StreamSocket& socket, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory);
		/// Creates the HTTPServerConnection.

	virtual ~HTTPServerConnection();
		/// Destroys the HTTPServerConnection.
		
	void run();
		/// Handles all HTTP requests coming in.

protected:
	void sendErrorResponse(HTTPServerSession& session, HTTPResponse::HTTPStatus status);

private:
	HTTPServerParams::Ptr          _pParams;
	HTTPRequestHandlerFactory::Ptr _pFactory;
};


} } // namespace Poco::Net


#endif // Net_HTTPServerConnection_INCLUDED
