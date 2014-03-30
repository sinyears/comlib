#include "warray.h"
#include "wstd.h"
#ifndef __WLIST_H
#define __WLIST_H
namespace WSTD{
	enum { MINCHUNKSIZE = 16, MAXCHUNKSIZE = (1L << 15)};

	struct stackempty {};

	class blist{
		protected:
			ssize size;
			ssize elemsize;
			array<char *, LOCALCHUNK> listp; 

			blist(ssize elemsize) : elemsize(elemsize), size(0), listp(0) {}

			char *get(ssize pos);
			void chkempty() {
				if (isempty())
					throw stackempty();
			}

			virtual void freechunk(char *p, ssize s) = 0;

			virtual char *allocchunk(ssize s) = 0;
		public:
			ssize getsize() { return size; }

			bool isempty() { return getsize() == 0; }

			void setsize(ssize size) { this->size = size;  }

			void clear();

			ssize getchunk(ssize pos) const;

			ssize getchunksize(ssize i) const {
				return i < 11 ? (1 << (i + 4)) : (1 << 15);
			}

			ssize getelemoffset(ssize chunk, ssize pos) const {
				return chunk < 11 ? (pos - (1 << (chunk + 4) )) : (pos - (1 << 15));
			}

			ssize getchunkoffset(ssize chunk) const {}
			~blist() { clear(); }
	};// end blist

	template<class D, class S = heap> class list : public blist {
		D local[MINCHUNKSIZE];
		protected:
			char *allocchunk(ssize s) {
				return (char *)(s <= MINCHUNKSIZE ? local : S::alloc((D*)0, s));
			}

			void freechunk(char *p, ssize s) {
				if (p != (char *)local) 
					S::free((D*)p);	
			}
		public:
			list() : blist(sizeof(D)) {}
		
			list(const list<D, S> &l) : blist(sizeof(D)) { *this = l; }

			list &operator=(const list<D, S> &l) {
				if (&l != this) {
					size = l.getsize();
					for (usize i = 0; i < size; ++i)
						(*this)[i] = l[i];
				}
				return *this;
			}

			D& operator[](ssize pos) {
				if (pos < MINCHUNKSIZE) {
					if (size <= pos)
						size = pos + 1;
					return local[pos];
				}
				return *(D*)get(pos);
			}

			void set(ssize pos, D &val) {
				(*this)[pos] = val;
			}

			void clear() { blist::clear(); }

			list &operator<<(const D &v) {
				set(size, v);
				return *this;
			}

			void push(const D &v) { (*this) << v; }

			D &peek() { chkempty(); return (*this)[size - 1]; }

			D pop() {
				D tmp = peek();
				size--;
				return tmp;
			}

			list &operator>>(D &v) {
				v = peek();
				size--;
				return *this;
			}

			void remove(ssize idx, ssize end){
				assert(idx >= 0 && idx < end && end < size);
				size -= end - idx + 1;
				while (idx < size) {
					(*this)[idx++] = (*this)[++end];
				}
			}

			void remove(ssize idx) { 
				if (idx == size -1) {
					size -= 1;
					return;
				}
				remove(idx, idx + 1); 
			}

			ssize removeval(const D &val);

			void insert(ssize idx, const D *data, ssize len) {
				assert(idx >= 0 && len > 0 && idx < size);
				ssize oend = size -1;
				size += len;
				ssize end = size - 1;
				while (end >= idx + len) {
					(*this)[end--] = this[oend--];
				}
				for (ssize i = idx; i < idx + len; i++) {
					(*this)[i] = *data; 
				}
			}

			void insert(ssize idx, const D &val) { insert(idx, &val, 1); }
	
	};//end list
}// end wstd
#endif 
