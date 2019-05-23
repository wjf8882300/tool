//
// SharedMemory.h
//
// $Id: //poco/svn/Foundation/include/Poco/SharedMemory.h#2 $
//
// Library: Foundation
// Package: Processes
// Module:  SharedMemory
//
// Definition of the SharedMemory class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_SharedMemory_INCLUDED
#define Foundation_SharedMemory_INCLUDED


#include "Poco/Foundation.h"
#include <algorithm>
#include <cstddef>


namespace Poco {


class SharedMemoryImpl;
class File;


class Foundation_API SharedMemory
	/// Create and manage a shared memory object.
	///
	/// A SharedMemory object has value semantics, but
	/// is implemented using a handle/implementation idiom.
	/// Therefore, multiple SharedMemory objects can share
	/// a single, reference counted SharedMemoryImpl object.
{
public:
	enum AccessMode
	{
		AM_READ = 0,
		AM_WRITE
	};

	SharedMemory();
		/// Default constructor creates an unmapped SharedMemory object. 
		/// No clients can connect to an unmapped SharedMemory object.

	SharedMemory(const std::string& name, std::size_t size, AccessMode mode, const void* addrHint = 0, bool server = true);
		/// Creates or connects to a shared memory object with the given name.
		///
		/// For maximum portability, name should be a valid Unix filename and not
		/// contain any slashes or backslashes.
		///
		/// An address hint can be passed to the system, specifying the desired
		/// start address of the shared memory area. Whether the hint
		/// is actually honored is, however, up to the system. Windows platform
		/// will generally ignore the hint.
		///
		/// If server is set to false, the shared memory region will be unlinked
		/// by calling shm_unlink (on POSIX platforms) when the SharedMemory object is destroyed.
		/// The server parameter is ignored on Windows platforms.

	SharedMemory(const File& file, AccessMode mode, const void* addrHint = 0);
		/// Maps the entire contents of file into a shared memory segment.
		///
		/// An address hint can be passed to the system, specifying the desired
		/// start address of the shared memory area. Whether the hint
		/// is actually honored is, however, up to the system. Windows platform
		/// will generally ignore the hint.

	SharedMemory(const SharedMemory& other);
		/// Creates a SharedMemory object by copying another one.

	~SharedMemory();
		/// Destroys the SharedMemory.

	SharedMemory& operator = (const SharedMemory& other);
		/// Assigns another SharedMemory object.

	void swap(SharedMemory& other);
		/// Swaps the SharedMemory object with another one.

	char* begin() const;
		/// Returns the start address of the shared memory segment.
		/// Will be NULL for illegal segments.

	char* end() const;
		/// Returns the one-past-end end address of the shared memory segment. 
		/// Will be NULL for illegal segments.

private:
	SharedMemoryImpl* _pImpl;
};


//
// inlines
//
inline void SharedMemory::swap(SharedMemory& other)
{
	using std::swap;
	swap(_pImpl, other._pImpl);
}


} // namespace Poco::Poco


#endif // Foundation_SharedMemory_INCLUDED
