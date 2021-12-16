#include <stdio.h>

#include "rtti.h"
#include "tgprint.h"

struct coords_t {
	int x;
	int y;
};

REGISTER_TYPE(struct coords_t)

int main(void)
{
	int var = 42;
	int array[3] = {0, 1, 2};
	struct coords_t coords;

	println("Hello world!");
	println(var);
	println(0);

	println("Type of var is: \"", type_name(var), "\"");
	println("Type of coords is \"", type_name(coords), "\"");

	println("Array: ", array);

	var = println("1234");
	println("Previous line has ", var, " characters.");

	return 0;
}
