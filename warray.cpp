#include "wstd.h"
#include "warray.h"
namespace WSTD{

	buffer::buffer(const void* data, long len) : a(len), size(len){
		if (len)
			::memcpy(a, data, len);
	}

	void buffer::adddata(const void *data, long len){
		ensurecapacity(size + len);
		::memcpy(a.getptr() + size, data, len);
		size += len;
	}

	void buffer::clear(){
		size = 0;
		a.clear();
	}

	bytearray::ibytearray *bytearray::makearray(long len){
		ibytearray *b = (ibytearray*)heap::allocbytes(sizeof(ibytearray) + len);
		b->len = len;
		b->usercount = 1;
		return b;
	}

	bytearray::bytearray(long len){
		b = makearray(len);
		::memset(b->data, 0, len);	
	}

	void bytearray::clear(){
		if (b && ! --b->usercount)
			heap::freebytes(b, b->len + sizeof(ibytearray));
		b = 0;
	}

	bytearray &bytearray::operator=(const bytearray &o){
		if (b != o.b){
			clear();
			attach(o.b);
		}
		return *this;
	} 
}
