/*
 * xvm_shell.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "xvm.h"


void xvm_shell_print_help()
{
    xvm_log_println("usage:");
    xvm_log_println("  xvm [options] FILE         Executes the specified virtual program");
    xvm_log_println("options:");
    xvm_log_println("  -h, --help, help           Prints the help information");
    xvm_log_println("  --version                  Prints the version and license note");
    xvm_log_println("  --verbose                  Prints additional output before and after program execution");
    xvm_log_println("  -st, --stack-size SIZE     Sets the stack size (by default 256)");
    xvm_log_println("  -mod, --load-module FILE   Loads the module, specified by FILE");
}

void xvm_shell_print_version()
{
    #ifdef _ENABLE_RUNTIME_DEBUGGER_
    xvm_log_println("XieXie 2.0 (Rev.1) VirtualMachine (XVM) with RuntimeDebugger (RTD)");
    #else
    xvm_log_println("XieXie 2.0 (Rev.1) VirtualMachine (XVM)");
    #endif
    xvm_log_println("");
    xvm_log_println("Copyright (C) 2014 Lukas Hermanns");
    xvm_log_println("All rights reserved.");
    xvm_log_println("");
    xvm_log_println("This software may be modified and distributed under the terms");
    xvm_log_println("of the BSD license.  See the LICENSE file for details.");
}

int xvm_shell_parse_args(int argc, char* argv[])
{
    // Configuration memory
    int verbose = 0;
    const char* filename = NULL;
    size_t stack_size = 256;

    // Initialize module container
    xvm_module_container module_cont;
    xvm_module_container_init(&module_cont);

    // Check if there are any arguments
    const char* arg;

    if (argc <= 0)
    {
        xvm_log_println("no input: enter \"help\" for information");
        return 0;
    }

    // Parse all arguments
    while (argc > 0)
    {
        // Store current argument
        arg = *argv;

        // Get next argument
        ++argv;
        --argc;

        // Parse current argument
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0 || strcmp(arg, "help") == 0)
            xvm_shell_print_help();
        else if (strcmp(arg, "--version") == 0)
            xvm_shell_print_version();
        else if (strcmp(arg, "--verbose") == 0)
            verbose = 1;
        else if (strcmp(arg, "-mod") == 0 || strcmp(arg, "--load-module") == 0)
        {
            if (argc > 0)
            {
                // Get parameter from next argument
                arg = *argv;

                // Load module
                xvm_module module;
                xvm_module_init(&module);
                
                if (xvm_module_load(&module, arg) != 0)
                    xvm_module_container_add(&module_cont, module);

                ++argv;
                --argc;
            }
            else
            {
                xvm_log_error("expected argument after \"-mod\" and \"--load-module\" flag");
                return 0;
            }
        }
        else if (strcmp(arg, "-st") == 0 || strcmp(arg, "--stack-size") == 0)
        {
            if (argc > 0)
            {
                // Get parameter from next argument
                arg = *argv;
                int param = atoi(arg);

                if (param <= 0)
                {
                    xvm_log_error("stack size must be greater than zero");
                    return 0;
                }
                else
                    stack_size = (size_t)param;

                // Get next argument
                ++argv;
                --argc;
            }
            else
            {
                xvm_log_error("expected argument after \"-st\" and \"--stack-size\" flag");
                return 0;
            }
        }
        else if (strlen(arg) > 0)
        {
            if (filename != NULL)
            {
                xvm_log_error("only a single program can be executed at a time");
                return 0;
            }
            else
                filename = arg;
        }
    }

    // Execute shell command
    if (filename != NULL)
    {
        // Read byte code from file
        xvm_bytecode byte_code;
        xvm_bytecode_init(&byte_code);

        if (xvm_bytecode_read_from_file(&byte_code, filename) == 0)
        {
            xvm_log_readfile_error(filename);
            return 0;
        }

        // Bind modules
        if (xvm_module_iteration_start(&module_cont) != 0)
        {
            xvm_module* module = NULL;
            while ( ( module = xvm_module_iteration_next() ) != NULL )
                xvm_bytecode_bind_module(&byte_code, module);
        }

        // Create stack
        xvm_stack stack;
        xvm_stack_init(&stack);
        xvm_stack_create(&stack, stack_size);

        // Execute program
        const xvm_exit_codes exit_code = xvm_execute_program(&byte_code, &stack);

        if (exit_code != EXITCODE_SUCCESS)
            xvm_log_exitcode_error(exit_code);
        else if (verbose != 0)
            xvm_log_println("program terminated successful");

        // Clean up
        xvm_bytecode_free(&byte_code);
        xvm_stack_free(&stack);
    }

    return 1;
}

// Main function for the shell
int main(int argc, char* argv[])
{
    // Ignore program path argument, then parse all other arguments
    xvm_shell_parse_args(--argc, ++argv);
    return 0;
}


