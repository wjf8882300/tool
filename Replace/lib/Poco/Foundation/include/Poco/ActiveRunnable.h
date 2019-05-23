//
// ActiveRunnable.h
//
// $Id: //poco/svn/Foundation/include/Poco/ActiveRunnable.h#2 $
//
// Library: Foundation
// Package: Threading
// Module:  ActiveObjects
//
// Definition of the ActiveRunnable class.
//
// Copyright (c) 2004-2007, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_ActiveRunnable_INCLUDED
#define Foundation_ActiveRunnable_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/ActiveResult.h"
#include "Poco/Runnable.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"


namespace Poco {


class ActiveRunnableBase: public Runnable, public RefCountedObject
	/// The base class for all ActiveRunnable instantiations.
{
public:
	typedef AutoPtr<ActiveRunnableBase> Ptr;
};


template <class ResultType, class ArgType, class OwnerType>
class ActiveRunnable: public ActiveRunnableBase
	/// This class is used by ActiveMethod.
	/// See the ActiveMethod class for more information.
{
public:
	typedef ResultType (OwnerType::*Callback)(const ArgType&);
	typedef ActiveResult<ResultType> ActiveResultType;

	ActiveRunnable(OwnerType* pOwner, Callback method, const ArgType& arg, const ActiveResultType& result):
		_pOwner(pOwner),
		_method(method),
		_arg(arg),
		_result(result)
	{
		poco_check_ptr (pOwner);
	}

	void run()
	{
		ActiveRunnableBase::Ptr guard(this, false); // ensure automatic release when done
		try
		{
			_result.data(new ResultType((_pOwner->*_method)(_arg)));
		}
		catch (Exception& e)
		{
			_result.error(e);
		}
		catch (std::exception& e)
		{
			_result.error(e.what());
		}
		catch (...)
		{
			_result.error("unknown exception");
		}
		_result.notify();
	}

private:
	OwnerType* _pOwner;
	Callback   _method;
	ArgType    _arg;
	ActiveResultType _result;
};


template <class ArgType, class OwnerType>
class ActiveRunnable<void, ArgType, OwnerType>: public ActiveRunnableBase
	/// This class is used by ActiveMethod.
	/// See the ActiveMethod class for more information.
{
public:
	typedef void (OwnerType::*Callback)(const ArgType&);
	typedef ActiveResult<void> ActiveResultType;

	ActiveRunnable(OwnerType* pOwner, Callback method, const ArgType& arg, const ActiveResultType& result):
		_pOwner(pOwner),
		_method(method),
		_arg(arg),
		_result(result)
	{
		poco_check_ptr (pOwner);
	}

	void run()
	{
		ActiveRunnableBase::Ptr guard(this, false); // ensure automatic release when done
		try
		{
			(_pOwner->*_method)(_arg);
		}
		catch (Exception& e)
		{
			_result.error(e);
		}
		catch (std::exception& e)
		{
			_result.error(e.what());
		}
		catch (...)
		{
			_result.error("unknown exception");
		}
		_result.notify();
	}

private:
	OwnerType* _pOwner;
	Callback   _method;
	ArgType    _arg;
	ActiveResultType _result;
};


template <class ResultType, class OwnerType>
class ActiveRunnable<ResultType, void, OwnerType>: public ActiveRunnableBase
	/// This class is used by ActiveMethod.
	/// See the ActiveMethod class for more information.
{
public:
	typedef ResultType (OwnerType::*Callback)();
	typedef ActiveResult<ResultType> ActiveResultType;

	ActiveRunnable(OwnerType* pOwner, Callback method, const ActiveResultType& result):
		_pOwner(pOwner),
		_method(method),
		_result(result)
	{
		poco_check_ptr (pOwner);
	}

	void run()
	{
		ActiveRunnableBase::Ptr guard(this, false); // ensure automatic release when done
		try
		{
			_result.data(new ResultType((_pOwner->*_method)()));
		}
		catch (Exception& e)
		{
			_result.error(e);
		}
		catch (std::exception& e)
		{
			_result.error(e.what());
		}
		catch (...)
		{
			_result.error("unknown exception");
		}
		_result.notify();
	}

private:
	OwnerType* _pOwner;
	Callback   _method;
	ActiveResultType _result;
};


template <class OwnerType>
class ActiveRunnable<void, void, OwnerType>: public ActiveRunnableBase
	/// This class is used by ActiveMethod.
	/// See the ActiveMethod class for more information.
{
public:
	typedef void (OwnerType::*Callback)();
	typedef ActiveResult<void> ActiveResultType;

	ActiveRunnable(OwnerType* pOwner, Callback method, const ActiveResultType& result):
		_pOwner(pOwner),
		_method(method),
		_result(result)
	{
		poco_check_ptr (pOwner);
	}

	void run()
	{
		ActiveRunnableBase::Ptr guard(this, false); // ensure automatic release when done
		try
		{
			(_pOwner->*_method)();
		}
		catch (Exception& e)
		{
			_result.error(e);
		}
		catch (std::exception& e)
		{
			_result.error(e.what());
		}
		catch (...)
		{
			_result.error("unknown exception");
		}
		_result.notify();
	}

private:
	OwnerType* _pOwner;
	Callback   _method;
	ActiveResultType _result;
};


} // namespace Poco


#endif // Foundation_ActiveRunnable_INCLUDED
