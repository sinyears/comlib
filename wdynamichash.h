#include "warray.h"
#ifndef __WDYNAMICHASH_H
#define __WDYNAMICHASH_H

namespace WSTD{
	
	enum { hashsplitsize = 32, skeylen = 256 };

	template<class T> class numberkey{
		T t;
		public:
			numberkey(T &t) : t(t) {}

			u4b hashcode() const {
				u4b v = (u4b)t;
				return (v >> 1) ^ (v & 10031);
			}

			bool operator==(const numberkey &k){
				return t == k.t;	
			}

			operator T() const { return t; }
	
	};

	class stringkey{
		array<char,skeylen> str;
		public:
			stringkey() : str() {}

			stringkey(const char *p){
				int len = ::strlen(p);
				str.setsize(len+1);
				::strncpy(str, p, len);
				str[len] = '\0';
			}

			bool operator==(const stringkey &str1){
				if (strcmp(str, str1.str) == 0)
					return true;
				return false;
			}

			s4b hashcode() const{
				if (! str) return 0;
				const char *p = str;
				s4b h = 0;
				while (*p)
					h = h+h+*p++;
				return h ^ (h & 10037);
			}
		
	};

	struct bhashelement{
		bhashelement *nextfield;
		bhashelement *&next() { return nextfield; }
		bhashelement( bhashelement *nextfield) : nextfield(nextfield) {}
	};//end bahshelement

	template<class K, class V> struct hashelement : public bhashelement{
		hashelement<K,V> *&next() { return (hashelement<K,V>*)nextfield ;}
		K k;
		V v;
		hashelement(const K &k, const V &v, hashelement<K,V> *next) : k(k), v(v), bhashelement(next){}

		hashelement(const K &k, hashelement<K,V> *next) : k(k), bhashelement(next){}
	};// end hashelement

	struct bhashbucket{
		bhashelement *hfield;
		long size, splitsize, hashsize;

		bhashbucket() : hfield(0), size(0) {}

		bhashelement *&h() { return hfield; }

		bhashbucket(const bhashbucket &b); 

		bhashbucket &operator=(const bhashbucket &b);
	};//end bhashbucket

	template<class K, class V> struct hashbucket : public bhashbucket{
		hashelement<K,V> *&h() { return (hashelement<K,V> *)hfield; }
		
		hashelement<K,V> **find(const K &key){
			for (hashelement<K,V> *p = h(); p; p = p->next()){
				if (p->k == key)
					return &p;
			}
			return 0;
		}

		bool remove(const K &key){
			hashelement<K,V> **td = find(key), *p;
			if(!td) return false;
			p = *td;
			*td = p->next();
			delete p;
			if (!--size) splitsize = hashsplitsize;
			return true;
		}

		bool add(const K &key, V *&val, bool toappend){
			hashelement<K,V> **tmp = find(key);
			if (!toappend && *tmp){
				if (val)
					*tmp->v = *val;
				else 
					val = &(*tmp)->v;
				return false;
			}

			if (val)
				h() = new hashelement<K,V>(key, *val, h());
			else{
				hashelement<K,V> *n = new hashelement<K,V>(key, h());
				val = &n->v;
				h() = n;
			}	
			size++;
			return true;
		}
	};//end hashbucket

		class bhash{
			public:
				bhash();
				long getsize() { return size; }
				virtual void clear() = 0;
			protected:
				array<bhashbucket> hasht;
				long size;
				long getposition(long h){
					return h & (hasht[h & (hasht.getsize() -1)].hashsize-1);
				}
				void split(long n);
				virtual long hashcode(bhashelement *el) = 0;

		};//end bhash

		template<class K,class V> class hash : public bhash{
			protected:
				long hashcode(bhashelement *el){
					return ((hashelement<K,V> *)el)->k.hashcode();
				}
				void clear(){
					for (long i = 0; i < hasht.getsize(); i++){
						bhashelement *el = hasht[i].h();
						while(el){
							bhashelement *ell = el->next();
							delete (hashelement<K,V> *)el;
							el = ell;
						}
					}
					hasht.setsize(1);
					bhashbucket *b = &hasht[0];
					b->splitsize = hashsplitsize;
					b->size = 0;
					b->hashsize = 1;
					b->h() = 0;
					size = 0;
				}	
				
				hashbucket<K,V> *bucket(long n) { return (hashbucket<K,V> *)&hasht[n]; }

				hashbucket<K,V> *getbucket(long h) { return bucket(getposition(h)); }
				
				void addcom(const K &key, V *&val, bool toappend){
					long n = getposition(key.hashcode());
					hashbucket<K,V> *b = getbucket(key.hashcode());
					if (b->add(key, val, toappend)) { size++; }
					if (b->size > b->splitsize) { split(n); }
				}

			public:
				void add(const K &key, const V &val, bool toappend = false){
					V *v = (V *)&val;
					if (v)
						addcom(key, v, toappend);
				}

				void addkey(const K &key,bool toappend = false){
					addcom(key, 0, toappend);
				}

				V *get(const K &key){
					hashbucket<K,V> *hb = getbucket(key.hashcode());
					hashelement<K,V> **ell = hb->find(key);	
					return ell ? &((*ell)->v) : 0;
				}

				~hash() { clear(); }
		};//end hash
}// END WSTD

#endif
