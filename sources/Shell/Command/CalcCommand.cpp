/*
 * CalcCommand.cpp
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommandFactory.h"
#include "ConsoleManip.h"

#include <xiexie/xiexie.h>
#include <sstream>


using namespace Platform::ConsoleManip;
using namespace XieXie;

// Define the code for the calculator script
static void GenCalculatorScript(std::stringstream& sstr, const std::string& expr)
{
    sstr
        << "class Calculator { " << std::endl
        << "static void printLn(bool val) { Calculator.printLn(val ? \"true\" : \"false\") }" << std::endl
        << "static void printLn(int val) { Intrinsics.printInt(val) Intrinsics.printLn(0) }" << std::endl
        << "static void printLn(float val) { Intrinsics.printFloat(val) Intrinsics.printLn(0) }" << std::endl
        << "static void printLn(String val) { Intrinsics.printLn(val.pointer()) }" << std::endl
        << "static void main() {" << std::endl
        << "    var calc := new Calculator()" << std::endl
        << "}" << std::endl
        
        << "float sin(float x) { return Math.sin(x) }" << std::endl
        << "float cos(float x) { return Math.cos(x) }" << std::endl
        << "float tan(float x) { return Math.tan(x) }" << std::endl
        << "float asin(float x) { return Math.asin(x) }" << std::endl
        << "float acos(float x) { return Math.acos(x) }" << std::endl
        << "float atan(float x) { return Math.atan(x) }" << std::endl
        << "float sinh(float x) { return Math.sinh(x) }" << std::endl
        << "float cosh(float x) { return Math.cosh(x) }" << std::endl
        << "float tanh(float x) { return Math.tanh(x) }" << std::endl
        << "float exp(float x) { return Math.exp(x) }" << std::endl
        << "float exp2(float x) { return Math.exp2(x) }" << std::endl
        << "float pow(float b, float e) { return Math.pow(b, e) }" << std::endl
        << "float log(float x) { return Math.log(x) }" << std::endl
        << "float log2(float x) { return Math.log2(x) }" << std::endl
        << "float log10(float x) { return Math.log10(x) }" << std::endl
        << "float sqrt(float x) { return Math.sqrt(x) }" << std::endl
        << "float ceil(float x) { return Math.ceil(x) }" << std::endl
        << "float floor(float x) { return Math.floor(x) }" << std::endl
        
        << "float sin(int x) { return Math.sin((float)x) }" << std::endl
        << "float cos(int x) { return Math.cos((float)x) }" << std::endl
        << "float tan(int x) { return Math.tan((float)x) }" << std::endl
        << "float asin(int x) { return Math.asin((float)x) }" << std::endl
        << "float acos(int x) { return Math.acos((float)x) }" << std::endl
        << "float atan(int x) { return Math.atan((float)x) }" << std::endl
        << "float sinh(int x) { return Math.sinh((float)x) }" << std::endl
        << "float cosh(int x) { return Math.cosh((float)x) }" << std::endl
        << "float tanh(int x) { return Math.tanh((float)x) }" << std::endl
        << "float exp(int x) { return Math.exp((float)x) }" << std::endl
        << "float exp2(int x) { return Math.exp2((float)x) }" << std::endl
        << "float pow(int b, float e) { return Math.pow((float)b, e) }" << std::endl
        << "float pow(float b, int e) { return Math.pow(b, (float)e) }" << std::endl
        << "float pow(int b, int e) { return Math.pow((float)b, (float)e) }" << std::endl
        << "float log(int x) { return Math.log((float)x) }" << std::endl
        << "float log2(int x) { return Math.log2((float)x) }" << std::endl
        << "float log10(int x) { return Math.log10((float)x) }" << std::endl
        << "float sqrt(int x) { return Math.sqrt((float)x) }" << std::endl
        << "float ceil(int x) { return Math.ceil((float)x) }" << std::endl
        << "float floor(int x) { return Math.floor((float)x) }" << std::endl
        
        << "init() {" << std::endl
        << "    const pi := Math.pi" << std::endl
        << "    const e := Math.e" << std::endl
        << "    Calculator.printLn(" << expr << ")" << std::endl
        << "}}" << std::endl
    ;
}

void CalcCommand::Execute(StreamParser& input, Log& output)
{
    bool repeat = false;

    if (input.Get() == "-R" || input.Get() == "--repeat")
    {
        input.Accept();
        repeat = true;
    }

    do
    {
        /* Get expression from user */
        output.Message("enter expression to evaluate (or 'exit', or Ctrl+Z):");

        {
            ScopedColor color(std::cout, Color::Green | Color::Blue);
            output.Print("> ");
        }

        std::string expr;
        std::getline(std::cin, expr);

        if (expr == "exit" || std::cin.eof())
            break;
        if (expr.empty())
            continue;

        /* Generate script code */
        std::stringstream code;
        GenCalculatorScript(code, expr);

        /* Execute expression evaluation */
        auto exitCode = RunFromString(
            code.str(),
            CompileFlags::Warn,
            VirtualMachine::Stack::defaultSize,
            &output.stream
        );

        if (exitCode != VirtualMachine::ExitCodes::Success)
            output.Error(VirtualMachine::ExitCodeString(exitCode));
    }
    while(repeat);
}

void CalcCommand::Help(HelpPrinter& printer) const
{
    printer.Command("calc", "calculates the entered expressions");
    printer.Flag("-R, --repeat", "repeats the expression prompt");
}



// ================================================================================