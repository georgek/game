// shared_ptr.h
// a shared (counted) smart pointer
// 
// Copyright (C) 2008 - George Kettleborough
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// 
// Contact: George Kettleborough <g.kettleborough@member.fsf.org>

#ifndef GK_SHARED_PTR_H
#define GK_SHARED_PTR_H

namespace gk {

    template <typename T>
    class shared_ptr
    {
    public:
	// constructors
	// initialise with pointer from new
	explicit shared_ptr (T* p = 0) :
	    ptr (p),
	    count (new long(1))
	    {
	    }
	
	// copy pointer
	shared_ptr (const shared_ptr<T>& p) throw() :
	    ptr (p.ptr),
	    count (p.count)
	    {
		// this is a new owner
		++(*count);
	    }
	
	// destructor
	~shared_ptr () throw()
	    {
		// delete object if this is last pointer
		release();
	    }
	
	// assignment operator
	shared_ptr<T>& operator= (const shared_ptr<T>& p) throw()
	    {
		if (this != &p) {
		    release();
		    ptr = p.ptr;
		    count = p.count;
		    ++(*count);
		}
		return *this;
	    }
	
	// pointer interface
	T& operator*() const throw()
	    {
		return *ptr;
	    }
	T* operator->() const throw()
	    {
		return ptr;
	    }

    private:
	// pointer to object
	T* ptr;
	// number of shared pointers to object
	long* count;
	
	// deletes object if this is the only pointer to it
	void release()
	    {
		if (--(*count) == 0) {
		    delete count;
		    delete ptr;
		}
	    }
    };

}

#endif /* GK_SHARED_PTR_H */

