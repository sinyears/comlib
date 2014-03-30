#include "wdynamichash.h"

namespace WSTD{
	bhashbucket& bhashbucket::operator=(const bhashbucket &b){
		size = b.size;
		splitsize = b.splitsize;
		hashsize = b.hashsize;
		hfield = b.hfield;
		return *this;
	}

	bhash::bhash() : hasht(1), size(0){
		bhashbucket *b = &hasht[0];
		b->splitsize = hashsplitsize;
		b->hashsize = 1;
	}

	void bhash::split(long n){
		long hashsize = hasht.getsize(), cursize = hasht[n].hashsize;
		if (hashsize == cursize){
			hashsize += hashsize;
			hasht.setsize(hashsize);
			for (int i = cursize; i < hashsize; i++){
				hasht[i].hashsize = hasht[i-cursize].hashsize;
				hasht[i].splitsize=hashsplitsize;
			}
		}
		bhashbucket *sb = &hasht[n];
		bhashelement *data = sb->h();
		sb->h() = NULL;
		while (data){
			long np = hashcode(data) & (hashsize-1);
			bhashbucket *nb = &hasht[np];
			bhashelement *dn = data->next();
			data->next() = nb->h();
			nb->h() = data;
			data = dn;
			nb->size++;
		}
		for (int i = n; i < hashsize; i+=cursize){
			bhashbucket *b = &hasht[i];
			b->hashsize = hashsize;
			b->splitsize = max(b->size*5/4, (long)hashsplitsize);
		}
	
	}// end split

}//end WSTD
