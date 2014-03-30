#include "wstd.h"
#ifndef __WARRAY_H
#define __WARRAY_H
namespace WSTD{
	enum {MAXDBLSIZE = 1L<<16, LOCALCHUNK = 8};

	template <class T, long len> class fixedarray{
		T a[len == 0 ? 0 : (len <= MAXDBLSIZE/sizeof(T) ? len : MAXDBLSIZE/sizeof(T))];
		public:
			operator T*(){ return a; }
	};

	template<class T, long L = 0, class S=heap> class array{
		protected:
			T *a;
			fixedarray<T, L> local;
			long size;
			void make(const T *data, long len){
				setsize(len);
				S::copy((T*)*this, data, len);
			}
		public:
			array() : size(L), a(L <= MAXDBLSIZE/sizeof(T) ? local : S::alloc((T*)0, L)){}
			array(const array<T,L,S> &l) : size(L), a(L ? (T*)local : 0){
				make(l, l.getsize());
			}

			array(long len) : size(len), a(L && len <= L ? (T*)local : S::alloc((T*)0, len)){}

			array &operator=(const array<T,L,S> &l){
				if (&l != this)
					make(&l, l.getsize());
				return *this;
			}

			operator T*() { return a; }

			operator const T*() const { return a; }

			T* getptr() { return a; }

			void free(){
				if (a != local)
					S::free(a);
			}

			long getsize() const{
				return size;
			}

			void setsize(long len){
				if ( size <= L){ // stored on stack
					if (len <= L){
					}
					else{
						a = S::alloc((T*)0, len);
						S::copy(a, (const T*)local, size);
					}

				}	
				else{  //stored on heap
					if (len <= L){
						S::copy((T*)local, (T*)a, len);
						S::free(a);
						a = local;
					}
					else{
						S::resize(a, size, len);
					}
				}
				size = len;
			}

			void clear(){
				setsize(0);
			}

			void ensuresize(long len){
				if (size < len)
					setsize( len >= MAXDBLSIZE/sizeof(T) ? len+MAXDBLSIZE/(sizeof(T) * 8) : (len<<1));
			}

			void copy(const T *buf, long off, long len){
				setsize(len);
				S::copy(a+off, buf, len);

			}

			~array(){
				if (size > L)
					S::free(a);
			}
	};//end array

	template<class T, long L = 4 >class arraylist{
		array<T,L,heap> a;
		long size;
		public:
			arraylist() : a(L), size(0) {}

			long getsize() const {return size;}

			void ensurecapacity(long cap){
				a.ensuresize(cap);
			}

			void setsize(long len){
				ensurecapacity(len);
				size = len;
			}

			operator T*(){ return a; }

			arraylist &operator<<(const T &v){
				setsize(size + 1);
				(*this)[size -1] = v;
				return *this;
			}	

			void clear(){ setsize(0);}

	};// end arraylist
	
	class buffer : public nocopy{
		protected:
			array<u1b> a;
			long size;
			virtual void resizearray(long len) { a.setsize(len); }
		public:
			enum { CAP = 256 };
			buffer(long len = 0) : a(max(CAP, len)), size(len){}

			buffer(const void *data, long len);

			long getsize() const {return size;}

			void ensurecapacity(long len){
				if (a.getsize() < len)
					resizearray(len >= MAXDBLSIZE ? len + MAXDBLSIZE / 8 :(len << 1));
			
			}

			void setsize(long len){
				ensurecapacity(len);
				size = len;
			}
			
			void addsize(long len) { setsize(size + len); }

			void adddata(const void *data, long len);

			operator u1b*() { return a; }

			void clear();
	};

	class bytearray{
		protected:
			struct ibytearray{
				long usercount;
				long len;
				u1b data[0];
			};

			static ibytearray *makearray(long len);
			ibytearray *b;
			void attach(ibytearray *b){
				this->b = b;
				if (b) 
					b->usercount++;
			}
		public:
			bytearray() : b(0) {}

			bytearray(long len);

			bytearray(const void *data, long len);

			bytearray(const bytearray &other) { attach(other.b); }

			bytearray &operator=(const bytearray &other);

			long getsize() const { return b ? b->len : 0; }

			operator u1b*() const { return b ? b->data : 0; }

			bool operator!() const { return b != 0; }
			

			void clear();
	};
	
}// end WSTD
#endif
