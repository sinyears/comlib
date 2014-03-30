#ifndef __WSTRING_H
#define __WSTRING_H
namespace WSTD {
	class string {
		struct istring {
			ssize usecount;
			ssize len;
			char str[0];
		};
		istring *makestr(const char *s, ssize len);
		istring *makestr(ssize len);
		istring *is;
		void attach(istring *i);
		void detach();
		public:
			string() : is(0){}

			string(const char *s);

			ssize getsize() { return is->len; }

			string(const string &s);

			istring *getptr() const { return is; }

			operator char *() { return is->str; }

			string &operator=(const string &s);

			char &operator[](ssize idx);

			bool operator==(const string &s);

			~string() { detach(); }
	};
}//end WSTD
#endif
