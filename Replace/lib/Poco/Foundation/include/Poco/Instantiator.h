//
// Instantiator.h
//
// $Id: //poco/svn/Foundation/include/Poco/Instantiator.h#2 $
//
// Library: Foundation
// Package: Core
// Module:  Instantiator
//
// Definition of the Instantiator class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_Instantiator_INCLUDED
#define Foundation_Instantiator_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


template <class Base>
class AbstractInstantiator
	/// The common base class for all Instantiator instantiations.
	/// Used by DynamicFactory.
{
public:
	AbstractInstantiator()
		/// Creates the AbstractInstantiator.
	{
	}
	
	virtual ~AbstractInstantiator()
		/// Destroys the AbstractInstantiator.
	{
	}
	
	virtual Base* createInstance() const = 0;
		/// Creates an instance of a concrete subclass of Base.	

private:
	AbstractInstantiator(const AbstractInstantiator&);
	AbstractInstantiator& operator = (const AbstractInstantiator&);
};


template <class C, class Base>
class Instantiator: public AbstractInstantiator<Base>
	/// A template class for the easy instantiation of 
	/// instantiators. 
	///
	/// For the Instantiator to work, the class of which
	/// instances are to be instantiated must have a no-argument
	/// constructor.
{
public:
	Instantiator()
		/// Creates the Instantiator.
	{
	}
	
	virtual ~Instantiator()
		/// Destroys the Instantiator.
	{
	}

	Base* createInstance() const
	{
		return new C;
	}
};


} // namespace Poco


#endif // Foundation_Instantiator_INCLUDED
