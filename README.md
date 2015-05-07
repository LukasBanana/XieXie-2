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

Programming Guide
-----------------

There is also a programming guide, which will be available for free,
as soon as the first compiler version is released.

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
		int a, b := 1
		while a < n {
			System.out.writeLine(a)
			a, b := b, a + b
		}
	}
	static void main() {
		Fibonacci.fib(1000)
	}
}
```

