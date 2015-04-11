XièXie Compiler and VirtualMachine (Version 2)
==============================================

Features
--------

- High Level Programming Language
- Compiler written in C++11 (in progress)
- Virtual Assembler (XASM)
- Virtual Machine (written in C99, with wrapper written in C++11)

License
-------

3-Clause BSD License

Status
------

**Work in Progress**

See "ToDo" list in Programming Guide for more information.

Programming Guide
-----------------

Take a look into the [XièXie Programming Guide](docu/TeX/xiexie-programming-guide.pdf)
to get started with this new object-oriented programming language.

Examples
--------

This is a small *Hello World* example:
```java
import System
class HelloWorld {
	static void main() {
		System.out.writeLine("Hello, World!")
	}
}
```

And this computes the fibonacci series:
```java
import System
class Fibonacci {
	static void fib(int n) {
		int a := 0, b := 1, prev
		while a < n {
			System.out.writeLine(a)
			prev := a
			a := b
			b += prev
		}
	}
	static void main() {
		Fibonacci.fib(1000)
	}
}
```

