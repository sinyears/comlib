#include "wbitset.h"

namespace WSTD{
	bitset::bitset(ssize len) : size(len), data(ceil(len, (ssize)BASEBYTEBIT)){
		::memset(&data[0], 0, data.getsize() * sizeof(usize));
	}

	void bitset::setsize(ssize len){
		if (len <= size){
			size = len;
			return;
		}
		ssize olen = ceil(size, (ssize)BASEBYTEBIT), nlen =ceil(len,(ssize) BASEBYTEBIT);
		data.ensuresize(nlen);
		::memset(&data[olen], 0, (nlen - olen)*sizeof(usize));
		size = len;
	}

	void bitset::set(ssize pos, bool val){
		if (pos >= size) setsize(pos + 1);
		if (val)
			data[pos >> BITFORBYTE] |= 1 << (pos & BYTEBITMASK) ;
		else
			data[pos >> BITFORBYTE] &= ~(1 << (pos & BYTEBITMASK));
	}

	ssize bitset::findclear(usize w){
		if (w == (usize)~0) return SETEND;
		ssize off = 0, cnt = BASEBYTEBIT/2;
		usize mask = 0xFFFF;
		for (;;){
			if ((w & mask) == mask){
				w >>= cnt;
				off += cnt;
			}
			cnt /= 2;
			if (!cnt) return off;
			mask >>= cnt;
		}
		
	}

	ssize bitset::firstclear(ssize startpos) const{
		if (startpos >= size) return SETEND;
		ssize pos = startpos >> BITFORBYTE, len = data.getsize();
		usize w = data[pos];
		w |= (1 << (startpos & BYTEBITMASK)) -1;
		for (;;){
			if (w != (usize)~0l){	
				ssize r = (pos << BITFORBYTE) + findclear(w);
				return r >= size ? SETEND : r;
			}
			if (++pos >= len) break;
			w = data[pos];
		}
		return SETEND;
	}

	ssize bitset::firstset(ssize startpos) const{
		if (startpos > size) return SETEND;
		ssize pos = startpos >> BITFORBYTE, len = data.getsize();
		usize w = data[pos];
		w &= ~0 << (startpos & BYTEBITMASK);
		for (;;){
			if (w != 0){	
				ssize r = (pos << BITFORBYTE) + findset(w);
				return r >= size ? SETEND : r;
			}
			if (++pos >= len) break;
			w = data[pos];
		}
		return SETEND;
	}

	static inline usize bitspan(int so, int eo){
		return ((usize)~0 << so) & (usize)~0 >> (BYTEBITMASK- eo);
	} 

	void bitset::set(ssize pos, ssize len, bool val){
		ssize epos = pos + len;
		if (!len || pos > size && val == false) return;
		if (epos > size) setsize(epos);
		usize v = val ? ~0 : 0;
		ssize sbkt = pos >> BITFORBYTE, ebkt = epos >> BITFORBYTE, soff = pos & BYTEBITMASK, eoff = epos & BYTEBITMASK;
		if (sbkt == ebkt){
			if (val)
				data[sbkt] |= bitspan(soff, eoff);
			else
				data[sbkt] &= ~bitspan(soff, eoff);
			return;
		}
		for (int i = sbkt + 1; i < ebkt; ++i)
			data[i] = v;	
		if (val){
			data[sbkt] |= bitspan(soff, BYTEBITMASK);
			data[ebkt] |= bitspan(0, eoff);
		} else{
			data[sbkt] &= ~bitspan(soff, BYTEBITMASK);
			data[ebkt] &= ~bitspan(0, eoff);
		}
		
	}
}//end WSTD
