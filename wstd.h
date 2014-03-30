#include "wbasic.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <new>
#ifndef __WSTD_H
#define __WSTD_H
namespace WSTD{
#define assert(exp)  if (!(exp)) {  exit(-1); } 

	template<class T1, class T2> static inline T1 ceil(T1 a, T2 b){
		return ((a + b) -1 )/b ;
	}

	template<class T1, class T2> static inline T1 min(T1 a, T2 b){
		return a < b ? a : (T1)b;
	}
	
	template<class T1, class T2> static inline T1 max(T1 a, T2 b){
		return a > b ? a : (T1)b;
	}

	template<class T> static inline T ceil(T a){
		return ceil(a, 4);
	}

	template<class T1, class T2> static inline T1 align(T1 a, T2 b){
		return ceil(a, b) * b;
	}

	template<class T> static inline T align(T a){
		return align(a, 4);
	}

	template<class T> static inline void swap(T &a, T &b){
		T c = a;
		a = b;
		b = c;
	}

	class nocopy{
		private:
			nocopy &operator=(const nocopy &nc) {return *this;};
			nocopy(const nocopy &nc) { };
		protected:
			nocopy(){};

	};

	class u3b{
		u1b d0, d1, d2;
		public:
			u3b(u4b n = 0);
			u3b &operator=(const u3b &o);
			operator u4b() const{
				return (((u4b)d0)<<16)|(((u4b)d1)<<8)|((u4b)d2);
			}
	};

	class heap{
		public:
			static void *allocbytes(long len);
			static void freebytes(void *p, long len);
			static void lock(){}
			static void unlock(){}
			template<class T> inline static T *alloc(T *p, long len){
				return len ? new T[len] : 0;
			}
			template<class T> static void free(T *p){
				if (p)
					delete[] p;
			}
			template<class T> static void copy(T *d, const T *s, long len){
				while (len --)
					*(d++) = *(s++);
			}
			template<class T> static void resize(T *&p, long oldlen, long newlen){
				if (oldlen == newlen) return;
				T *n = new T[newlen];
				copy(n, p, min(newlen,oldlen));
				free(p);
				p = n;
			}

	};

	template<class T> class autoptr{
		T *ptr;
		autoptr &operator=(const autoptr&) { return *this; };
		autoptr(const autoptr&) {}
		public:
			autoptr(T *ptr) : ptr(ptr) {}

			autoptr() : ptr(0) {}

			operator T*() { return ptr; }

			T *operator->() { return ptr; }

			T &operator*() { return *ptr; }

			void operator=(T *ptr) { this->ptr = ptr; }

			void clear(){
				if (ptr)
					delete ptr;
					ptr = 0;
			}	
	};
}// end WSTD
#endif
