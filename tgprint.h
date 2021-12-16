#pragma once

#define print_macro(...) ({\
	int ret = 0;\
	if(__printerf == printf) {\
		ret = printf(__VA_ARGS__);\
	} else\
	if(__printerf == fprintf) {\
		ret = fprintf(__stream, __VA_ARGS__);\
	} else\
	if(__printerf == dprintf) {\
		ret = dprintf(__fd, __VA_ARGS__);\
	} else\
	if(__printerf == sprintf) {\
		ret = sprintf(__str, __VA_ARGS__);\
	} else\
	{ ret = -1;}\
	ret;\
})

#define REGISTER_PRINT_TYPE(type, code)\
int tgprint(FILE *__stream, int __fd, char *__str, void *__printerf, type exp) __attribute__((overloadable)) code

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
		last_ret = tgprint(__stream, __fd, __str, __printerf, X);\
		__str += last_ret;\
	}

#define GENERATE_OUTPUT(...)\
	int ret = 0, last_ret = 0;\
	FOR_EACH(MIDDLE, "", __VA_ARGS__)\
	if(last_ret >= 0)\
		ret += last_ret;\
	else ret = last_ret;\
	ret;

#define print(...) ({\
	FILE *__stream = NULL;\
	int __fd = 0;\
	char *__str = NULL;\
	void *__printerf;\
	__printerf = (void*)printf;\
	GENERATE_OUTPUT(__VA_ARGS__)\
})

#define fprint(_stream, ...) ({\
	FILE *__stream = _stream;\
	int __fd = 0;\
	char *__str = NULL;\
	void *__printerf;\
	__printerf = (void*)fprintf;\
	GENERATE_OUTPUT(__VA_ARGS__)\
})

#define dprint(_fd, ...) ({\
	FILE *__stream = NULL;\
	int __fd = _fd;\
	char *__str = NULL;\
	void *__printerf;\
	__printerf = (void*)dprintf;\
	GENERATE_OUTPUT(__VA_ARGS__)\
})

#define sprint(_str, ...) ({\
	FILE *__stream = NULL;\
	int __fd = 0;\
	char *__str = _str;\
	void *__printerf;\
	__printerf = (void*)sprintf;\
	GENERATE_OUTPUT(__VA_ARGS__)\
})

#define println(...) print(__VA_ARGS__, "\n")
#define fprintln(...) fprint(__VA_ARGS__, "\n")
#define dprintln(...) dprint(__VA_ARGS__, "\n")
#define sprintln(...) sprint(__VA_ARGS__, "\n")
