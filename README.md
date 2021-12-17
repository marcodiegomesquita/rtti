# RTTI in C and type-generic print

## What

This repository encompasses two purposes: a small header that enables `type_name` and also an user extensible type-generic `print` implementation in **C**.

The idea behind that `type_name`, which is actually an overloadable function, is that it returns a string with the name of the type of whatever is passed as a parameter. Recognition of basic **C** types is already implemented and user-defined types are easy to add.

With regard to the type-generic `print`, it currently supports just a few types but adding user-defined types is equally simple. The file _test.c_ should be enough to illustrate how to use it.

## Why

I'm not the first one to feel this need. Actually, I got some inspiration from another implementation: [generic-print](https://github.com/exebook/generic-print). Ages ago, I decided to learn **C**. Coming from **Pascal**, I did not find `printf` nowhere near as elegant compared to how I could use `write` in **Pascal**. As time passed, I learned to like **C** and understand how its low level idiosyncrasies influences its programming model. Nevertheless, I still missed the simplicity of a simple `print` which I could pass a list of variables and let it automatically display their values.

Some progress happened when **C** included the `_Generic` keyword. Unfortunately, I could not find a way to use it to implement a type-generic `print` function which could allow the programmer to easily specify forms to print user-defined data-types. Once I heard about the `__attribute__((overloadable))` extension in *Clang* I tried to use it to finally implement a type-generic user extensible `print` function.

The first part of the effort allowed me to implement a primitive form of *Run-Time Type Information* [(RTTI)](https://en.wikipedia.org/wiki/Run-time_type_information) in **C**. The `type_name` implementation on the file _rtti.h_ is the result of it. The other part of the effort is a bit more evolved and lives on the _tgprint.h_ file which implements the type-generic user extensible `print` function.

## Requirements

This implementation requires the `__attribute__((overloadable))` extension from *Clang*. AFAIK, *Clang* is the only compiler currently available implementing it. So, for now, it can only be compiled with *Clang*. On my machine I compile it as follows:

> clang -Wall -Wextra test.c

## How

It is interesting to know a bit about how to use and how it works. Without diving to deep into details, let's study a little about it.

### How the magic happens

In the core of these headers, the pillar of these implementations, lies the *Clang* `__attribute__((overloadable))` [extension](https://releases.llvm.org/3.3/tools/clang/docs/LanguageExtensions.html#function-overloading-in-c). This extension enables function overloading in **C** in a very similar way to how it is done in **C++**. For our usage, it allows one to implement things like this:
```c
char *type_name(int x) __attribute__((overloadable)) {
	return "int";
}

char *type_name(float x) __attribute__((overloadable)) {
	return "float";
}
```

Calling `type_name` passing an `int` or a `float` will make it return the name of the type. Do it for all basic **C** types and you're done: **RTTI** in **C**!

The type-generic `print` is a little more advanced, but once aforementioned features are available, its implementation is just a bit more of work.

### How to use it

The implemented **RTTI** only knows **C** basic types. The type-generic `print` knows even less. Both features are extensible, this means: it is possible to extend them to recognize user-defined data-types.

For the **RTTI**, a new user-defined type can registered with the `REGISTER_TYPE` macro. The teach `print` how to print a new user-defined data-type, the `REGISTER_PRINT_TYPE` is used. Without further ado, this is a very simple example of it in use:
```c
#include <stdio.h>

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
	int a = 1;
	float b = 1.0/3.0;
	struct coords_t c = {.x = 123, .y = 456};

	println("Type of a is: ", type_name(a));
	println("Type of b is: ", type_name(b));
	println("Type of c is: ", type_name(c));

	println("Value of a is: ", a);
	println("Value of b is: ", b);
	println("Value of c is: ", c);

	return 0;
}
```

To specify printing format for numeric data-types, some utility macros are available. See the example _test.c_ for more details.

### How does it compare to [generic-print](https://github.com/exebook/generic-print)

Internally, these approaches are very different. While my approach expands variadic macros to make many calls to an overloaded function, `generic-print` creates an array with the types of arguments that are passed and then calls a variadic function with this list and all parameters.

In terms of code, `generic-print` seems better written, while my approach seems much easier to understand.

Portability-wise, `generic-print` wins. The extensions it depends on are supported by both *GCC* and *Clang*. My approach requires an extension that is available only on *Clang*.

There are some interesting features in `generic-print` which are not implemented on my approach. It has beautiful colors and a **Python**-esque ability to display arrays.

Some advantages of my approach over `generic-print` are mainly the possibility of extension, so it is not limited to basic **C** types only. Also, my approach allows the programmer to optionally specify format specifiers when display numerical data-types.

## Future

Initially this was intended to be a quick hack. Since it works and is reasonably interesting I decided to share it with the world. I'm not sure if I'll evolve it very much. Of course, it would be great to get PR's. So, don't give up on me.

Ultimately this work shows some painful shortcomings of the **C** programming language. Variadic macros are pure abuse. AFAICS, the simplest extension that could enable all this to work with minimal changes to the languages while still being useful for other purposes is to teach the **C** preporocessor to learn lists.

So, if it happens, I'd be very happy if it influences ISO to fix these mentioned problems or *GCC* developers to implement a few extensions.

## Thanks

I'd like to thank whoever answered a StackOverflow [question](https://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros) about abusing the **C** preprocessor to make it understand variadic macros. Also, the Hacker News user who [pointed](https://news.ycombinator.com/item?id=29559540) me to the `__attribute__((overloadable))` extension that finally allowed me to implement an extensible type-generic `print` in **C** which I've been dreaming of for decades.

Thank you!

