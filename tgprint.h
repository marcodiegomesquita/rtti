#pragma once

#define PRINTER(printer, ...)\
	if(__printerf == printer) {\
		ret = printer(__VA_ARGS__);\
	} else

#define print_macro(...) ({\
	int ret = 0;\
	PRINTER(printf, __VA_ARGS__)\
	PRINTER(fprintf, __stream, __VA_ARGS__)\
	PRINTER(dprintf, __fd, __VA_ARGS__)\
	PRINTER(sprintf, __str, __VA_ARGS__)\
	PRINTER(snprintf, __str, __size, __VA_ARGS__)\
	{ ret = -1;}\
	ret;\
})

#define REGISTER_PRINT_TYPE(type, code)\
int tgprint(FILE *__stream, int __fd, char *__str, size_t __size, void *__printerf, type exp) __attribute__((overloadable)) code

REGISTER_PRINT_TYPE(char *, {return print_macro("%s", exp);})
REGISTER_PRINT_TYPE(void *, {return print_macro("%p", exp);})
REGISTER_PRINT_TYPE(int, {return print_macro("%d", exp);})

//FOR_EACH magic taken from https://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

#define FOR_EACH_1(what, x, ...) what(x)
#define FOR_EACH_2(what, x, ...)\
  what(x);\
  FOR_EACH_1(what,  __VA_ARGS__);
#define FOR_EACH_3(what, x, ...)\
  what(x);\
  FOR_EACH_2(what, __VA_ARGS__);
#define FOR_EACH_4(what, x, ...)\
  what(x);\
  FOR_EACH_3(what,  __VA_ARGS__);
#define FOR_EACH_5(what, x, ...)\
  what(x);\
 FOR_EACH_4(what,  __VA_ARGS__);
#define FOR_EACH_6(what, x, ...)\
  what(x);\
  FOR_EACH_5(what,  __VA_ARGS__);
#define FOR_EACH_7(what, x, ...)\
  what(x);\
  FOR_EACH_6(what,  __VA_ARGS__);
#define FOR_EACH_8(what, x, ...)\
  what(x);\
  FOR_EACH_7(what,  __VA_ARGS__);

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define FOR_EACH_RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, x, ...) CONCATENATE(FOR_EACH_, N)(what, x, __VA_ARGS__)
#define FOR_EACH(what, x, ...) FOR_EACH_(FOR_EACH_NARG(x, __VA_ARGS__), what, x, __VA_ARGS__)

// End of FOR_EACH magic

#define MIDDLE(X)\
	if(last_ret >= 0) {\
		ret += last_ret;\
		last_ret = tgprint(__stream, __fd, __str, __size, __printerf, X);\
		__str += last_ret;\
		__size -= last_ret;\
	}

#define GENERATE_OUTPUT(...)\
	int ret = 0, last_ret = 0;\
	FOR_EACH(MIDDLE, "", __VA_ARGS__)\
	if(last_ret >= 0)\
		ret += last_ret;\
	else ret = last_ret;\
	ret;

#define HEAD(stream, fd, str, size, function)\
	FILE *__stream = stream;\
	int __fd = fd;\
	char *__str = str;\
	size_t __size = size;\
	void *__printerf;\
	__printerf = (void*)function;

#define print(...) ({\
	HEAD(NULL, 0, NULL, 0, printf)\
	GENERATE_OUTPUT(__VA_ARGS__)\
})

#define fprint(_stream, ...) ({\
	HEAD(_stream, 0, NULL, 0, fprintf)\
	GENERATE_OUTPUT(__VA_ARGS__)\
})

#define dprint(_fd, ...) ({\
	HEAD(NULL, _fd, NULL, 0, dprintf)\
	GENERATE_OUTPUT(__VA_ARGS__)\
})

#define sprint(_str, ...) ({\
	HEAD(NULL, 0, _str, 0, sprintf)\
	GENERATE_OUTPUT(__VA_ARGS__)\
})

#define snprint(_str, _size, ...) ({\
	HEAD(NULL, 0, _str, _size, snprintf)\
	GENERATE_OUTPUT(__VA_ARGS__)\
})

#define println(...) print(__VA_ARGS__, "\n")
#define fprintln(...) fprint(__VA_ARGS__, "\n")
#define dprintln(...) dprint(__VA_ARGS__, "\n")
#define sprintln(...) sprint(__VA_ARGS__, "\n")
#define snprintln(...) snprint(__VA_ARGS__, "\n")
