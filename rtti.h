#pragma once

#define REGISTER_TYPE(type) \
char* type_name(type exp) __attribute__((overloadable)) {\
	return #type;\
}\

#define Q_REGISTER_TYPE(type)\
REGISTER_TYPE(type)\
REGISTER_TYPE(type *)\
REGISTER_TYPE(type **)

Q_REGISTER_TYPE(unsigned char)
Q_REGISTER_TYPE(char)
Q_REGISTER_TYPE(signed char)

Q_REGISTER_TYPE(short)

Q_REGISTER_TYPE(int)
Q_REGISTER_TYPE(unsigned int)

Q_REGISTER_TYPE(long)
Q_REGISTER_TYPE(unsigned long)

Q_REGISTER_TYPE(long long)
Q_REGISTER_TYPE(unsigned long long)

Q_REGISTER_TYPE(float)

Q_REGISTER_TYPE(double)

Q_REGISTER_TYPE(long double)

Q_REGISTER_TYPE(void *)
