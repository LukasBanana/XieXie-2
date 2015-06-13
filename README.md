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

[3-Clause BSD License](https://github.com/LukasBanana/XieXie-2/blob/master/LICENSE.txt)

Status
------

**Work in Progress**

Programming Guide
-----------------

Take a look at the [XièXie Programming Guide](https://github.com/LukasBanana/XieXie-2/releases/tag/guide)
to get started with the XièXie programming language.

Code Examples
-------------

This is a small *Hello World* example:
```java
import Console
class HelloWorld {
	static void main() {
		Console.writeLine("Hello, World!")
	}
}
```

And this computes the fibonacci series:
```java
import Console
class Fibonacci {
	static void fib(int n) {
		int a, b := 1
		while a < n {
			Console.writeLine(a)
			a, b := b, a + b
		}
	}
	static void main() {
		Fibonacci.fib(1000)
	}
}
```

Integration Examples
--------------------
Very high-level interface:
```cpp
#include <xiexie/xiexie.h>
int main()
{
	XieXie::RunFromString(
		"import MsgBox\n"
		"class MiniExample {\n"
		"    static void main() {\n"
		"        MsgBox.show(\"Info\", \"This is a mini Example!\")\n"
		"    }\n"
		"}\n"
	);
}
```

Offline compilation:
```cpp
#include <xiexie/xiexie.h>
int main()
{
	XieXie::CompileConfig config;
	{
		std::string filename = "ExampleFile.xx";
		config.sources.push_back({ filename, std::make_shared<std::ifstream>(filename) });
	}
	XieXie::Compile(config, &(std::cout));
}
```

Running compiled code:
```cpp
#include <xiexie/xiexie.h>

void triggerGameEvent(XVM_Env env)
{
	// Read parameter from XVM environment
	int eventID = XVM_ParamInt(env, 1);
	
	/* do something ... */
	
	// Return void and pop 1 argument (eventID)
	XVM_ReturnVoid(env, 1);
}

/* further functions ... */

int main()
{
	XieXie::VirtualMachine::ByteCode bc;
	XieXie::VirtualMachine::Stack st;
	
	// Read XieXie byte code (*.xbc) file
	bc.ReadFromFile("MyGameScript.xbc");
	
	// Bind external invocation "triggerGameEvent"
	bc.BindInvocation("GameInterface.triggerGameEvent", triggerGameEvent);
	
	// Execute byte code in virtual machine
	XieXie::VirtualMachine::ExecuteProgram(bc, st, "GameScript.entryPoint");
}
```


