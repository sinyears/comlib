#include "wstd.h"
#include "wstring.h"

namespace WSTD {
	string::istring *string::makestr(ssize len) {
		istring *str = (istring *)malloc(sizeof(istring) + len);
		str->len = len;
		str->usecount = 0;
		return str;
	}

	string::istring *string::makestr(const char *s, ssize len) {
		istring *str = makestr(len);
		memcpy(str->str, s, len);
		(str->str)[len] = '\0';
		return str;	
	}

	void string::detach() {
		if (!is) return;
		if(--is->usecount == 0)
			free(is);
		is = 0;
	}

	void string::attach(istring *i) {
		if (!is) return;
		is = i;
		is->usecount++;
	}
	
	string::string(const char *s) {
		if (s) {
			is = makestr(s, strlen(s));
		} else {
			is = 0;
		}
	}

	string &string::operator=(const string &s) {
		detach();
		attach(s.getptr());
		return *this;	
	}

	char &string::operator[](ssize idx) {
		assert(idx < is->len);
		return (is->str)[idx];
	}

	bool string::operator==(const string &s) {
		return (is == s.getptr()) ? true : false;
	}
}
