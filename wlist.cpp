#include "wlist.h"

namespace WSTD{

	static const char chunkmap[16] = { 0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3 };

	ssize blist::getchunk(ssize i) const {
		if (i >> 4 == 0) return -1;
		if (i >> 8 == 0) return chunkmap[i >> 4];
		if (i >> 12 == 0) return 4 + chunkmap[i >> 8];
		if (i >> 16 == 0) return 8 + chunkmap[i >> 12];
		return 12 + ((i - (1 << 16) ) >> 15);
	}

	void blist::clear() {
		for (int i = 0; i < listp.getsize(); i++) {
			if (listp[i])
				freechunk(listp[i], getchunksize(i) * elemsize);
		}
		listp.setsize(0);
		size = 0;
	}

	char *blist::get(ssize pos){
		ssize chunk = getchunk(pos), oldchunk = listp.getsize(), newchunk = oldchunk;
		while (newchunk <= chunk) {
			newchunk += newchunk;
		}
		if (newchunk != oldchunk){
			listp.setsize(newchunk);
			::memset(&listp[oldchunk], 0, (newchunk - oldchunk) * sizeof(char *));
		}
		if (pos >= size) size = pos + 1;
		if (!listp[chunk]) 
			listp[chunk] = allocchunk(getchunksize(chunk));
		return listp[chunk] + elemsize * getelemoffset(chunk, pos);
	}	
}//end wstd
