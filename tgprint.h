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
int tgprint(__attribute__((unused))char *__format, FILE *__stream, int __fd, char *__str, size_t __size, void *__printerf, type exp) __attribute__((overloadable)) code

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
#define FOR_EACH_9(what, x, ...)\
  what(x);\
  FOR_EACH_8(what,  __VA_ARGS__);
#define FOR_EACH_10(what, x, ...)\
  what(x);\
  FOR_EACH_9(what,  __VA_ARGS__);

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define FOR_EACH_RSEQ_N() 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, x, ...) CONCATENATE(FOR_EACH_, N)(what, x, __VA_ARGS__)
#define FOR_EACH(what, x, ...) FOR_EACH_(FOR_EACH_NARG(x, __VA_ARGS__), what, x, __VA_ARGS__)

// End of FOR_EACH magic

#define MIDDLE(X)\
	if(last_ret >= 0) {\
		ret += last_ret;\
		last_ret = tgprint(__format, __stream, __fd, __str, __size, __printerf, X);\
		__str += last_ret;\
		__size -= last_ret;\
	}

#define BODY(stream, fd, str, size, function, ...) ({\
	char *__format = NULL;\
	FILE *__stream = stream;\
	int __fd = fd;\
	char *__str = str;\
	size_t __size = size;\
	void *__printerf;\
	__printerf = (void*)function;\
	int ret = 0, last_ret = 0;\
	FOR_EACH(MIDDLE, "", __VA_ARGS__)\
	if(last_ret >= 0)\
		ret += last_ret;\
	else ret = last_ret;\
	ret;\
})

#define print(...) 			BODY(NULL, 0, NULL, 0, printf, __VA_ARGS__)
#define fprint(_stream, ...) 		BODY(_stream, 0, NULL, 0, fprintf, __VA_ARGS__)
#define dprint(_fd, ...) 		BODY(NULL, _fd, NULL, 0, dprintf, __VA_ARGS__)
#define sprint(_str, ...) 		BODY(NULL, 0, _str, 0, sprintf, __VA_ARGS__)
#define snprint(_str, _size, ...) 	BODY(NULL, 0, _str, _size, snprintf, __VA_ARGS__)
#define println(...) print(__VA_ARGS__, "\n")
#define fprintln(...) fprint(__VA_ARGS__, "\n")
#define dprintln(...) dprint(__VA_ARGS__, "\n")
#define sprintln(...) sprint(__VA_ARGS__, "\n")
#define snprintln(...) snprint(__VA_ARGS__, "\n")

REGISTER_PRINT_TYPE(char *, {return print_macro("%s", exp);})
REGISTER_PRINT_TYPE(void *, {return print_macro("%p", exp);})
REGISTER_PRINT_TYPE(int, {return print_macro(__format ? __format : "%d", exp);})
REGISTER_PRINT_TYPE(float, {return print_macro(__format ? __format : "%f", exp);})

#define FORMAT_BEGIN(_format) ({__format = _format; "";})
#define FORMAT_END ({__format = NULL; "";})
#define FORMAT(_format, exp)  FORMAT_BEGIN(_format), exp, FORMAT_END
