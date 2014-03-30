#include<cstdarg>
#ifndef W_IPC_H
#define W_IPC_H
namespace WIPC{

	char *sharedbase;
	enum{ WAIT4EVER = -1 };

	template<class T> struct ipc_shmptr{
		unsigned long off;
		ipc_shmptr() { off = 0; }
		ipc_shmptr(T *p) : off(p == NULL? 0 : ((char*)p - sharedbase)){}
		operator T*() { return off == 0 ? NULL : (T*)(sharedbase+off); }
		T* operator ->() { return (T*)*this; }
		T* getptr(){ return (T*)*this; }
		void clear() { off = 0; }
	};

	class ipc_atomic_value{
		long value;
		public:
			ipc_atomic_value(long value) : value(value){}
			operator long(){return value;}
			int increment(long inc=1);
			int decrement(long dec=1){ return increment(-1); }

	};

	class ipc_index{
		int idx;
		public:
			ipc_index(int idx=-1);
			operator int(){return idx;}
			~ipc_index();
	};

	class latch{
		ipc_atomic_value val;
		ipc_index idx;
		public:
			latch(int idx=-1) : idx(idx), val(1){}
			void wait();
			void release();
			~latch();

	};

	class latchlock{
		latch &m;
		public:
			latchlock(latch &m) : m(m){
				m.wait();
			}
			~latchlock(){
				m.release();
			}

	};

	struct ipc_waitobj{
		bool ismutex;
		union{
			struct{
				bool isopen;
				bool autoreset;
			}evt;
			struct{
				pid_t pid;
				pthread_t tid;
				size_t owncount;
			}mtx;

		};
	};

	class IPC{
		protected:
			ipc_shmptr<ipc_waitobj> hdl;
			static int dowait(long timeout, bool waitall, IPC *obj1, va_list va);
		public:
			IPC(ipc_waitobj *hdl = NULL) : hdl(hdl){}
			bool wait(long timeout = WAIT4EVER){
				return dowait(timeout, false, this, (va_list)NULL);
			}
			static int waitone(long timeout, IPC *obj1,...){
				va_list va;
				va_start(va, obj1);
				return dowait(timeout, false, obj1, va);
			}
			static bool waitall(long timeout, IPC *obj1, ...){
				va_list va;
				va_start(va, obj1);
				return dowait(timeout, true, obj1, va) != -1;
			}
			~IPC();

	};

	class event : public IPC{
		public:
			event(){}


	};
}//end WIPC
#endif
























