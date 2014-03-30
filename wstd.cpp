#include "wstd.h"

namespace WSTD{
	u3b::u3b(u4b n){
		d0 = (n & 0x00ff0000) >> 16;
		d1 = (n & 0x0000ff00) >> 8;
		d2 = (n & 0x000000ff) ;
	}
	
	u3b& u3b::operator=(const u3b &o){
		d0 = o.d0;
		d1 = o.d1;
		d2 = o.d2;
		return *this;
	} 

	void * heap::allocbytes(long len){
		if (!len) return 0;
		void *p = ::malloc(len);
		if (!p)
			throw std::bad_alloc() ;
		return p;
	}

	void heap::freebytes(void *p, long len){
		if (p)
			::free(p);
	}
}
