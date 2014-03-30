#include "wstd.h"
#include "warray.h"
#ifndef __WBITSET_H
#define __WBITSET_H
namespace WSTD{
	enum { SETEND = -1 };
	enum { BITFORBYTE = 5, BYTEBITMASK = 31, BASEBYTEBIT = 32};

	class bitset;

	class ibit{
		friend class bitset;
		bitset *s;
		ssize pos;
		ibit( bitset *set, ssize pos) : s(set), pos(pos) {}
		public:
			ibit &operator=(bool newval);

			operator bool() const;
	};
	
	class bitset{
		array<usize, LOCALCHUNK> data;
		ssize size;
		static ssize bitoff(usize w);
		public:
			static ssize findclear(usize w);

			static ssize findset(usize w) { return findclear(~w); }

			bitset(const bitset &bs) : data(bs.data), size(bs.size) {}

			bitset(ssize len = 0);

			void setsize(ssize len);

			ssize getsize() const { return size; }

			ibit operator[](ssize pos) { return ibit(this, pos); }

			ssize firstset(ssize startpos = 0) const;

			ssize firstclear(ssize startpos = 0) const;

			bool get(ssize pos) const { return pos >= size ? false : (data[pos >> BITFORBYTE] & 1L << (pos & BASEBYTEBIT - 1) - 1) != 0;}

			void set(ssize pos, bool val);

			void set(ssize pos, ssize len, bool val);
	};//end bitset

	inline ibit & ibit::operator=(bool newval){
				s->set(pos, newval);
				return *this;
			}

	inline ibit::operator bool() const{
				return s->get(pos);		
			}


}// end wstd


#endif
