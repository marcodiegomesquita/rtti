#include <stdio.h>
#include <math.h>

#include "rtti.h"
#include "tgprint.h"

struct coords_t {
	int x;
	int y;
};

REGISTER_TYPE(struct coords_t)

REGISTER_PRINT_TYPE(struct coords_t, {\
	return print_macro("(%d, %d)", exp.x, exp.y);\
})


int main(void)
{
	int var = 42;
	int array[3] = {0, 1, 2};
	struct coords_t coords = {.x = 123, .y = 456};
	char string[100];

	println("Hello world!");
	println(var);
	println(0);

	println("Type of var is: \"", type_name(var), "\"");
	println("Type of coords is \"", type_name(coords), "\"");

	println("Array: ", array);

	var = println("1234");
	println("Previous line has ", var, " characters.");

	fprintln(stdout, "And this one is sent to stdout.");
	dprintln(fileno(stdout), "And this one too!");

	sprintln(string, "The first characters of a string, printed to a string.");
	print(string);

	snprint(string, 21, "The first characters of a string, printed to a string.");
	println(string);

	snprintln(string, 21, "123", "456");
	print(string);

	println(coords.x, " + ", coords.y, " = ", coords.x + coords.y);
	println("sum: ", FORMAT("%05d", coords.x + coords.y));

	println("How much is 1/3? ", 1/3);
	println("What if we cast it to float? ", ((float)1.0/(float)3.0));

	float div = 1.0/3.0;

	println("Let's apply a better format: ", FORMAT("%.2f", div));

	println("And now, for a user-defined data type: ", coords);

	println("Let's try a series of formatted values...");
	println(FORMAT_BEGIN("%.2f"), (float)1.0, " ", (float)M_PI, " ", div, FORMAT_END);
	println(FORMAT_BEGIN("%.2f"), (float)1.0, " ", (float)M_PI, " ", FORMAT_END, div);

	return 0;
}
