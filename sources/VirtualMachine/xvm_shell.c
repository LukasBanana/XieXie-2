/*
 * xvm_shell.c
 * 
 * This file is part of the "XieXie 2.0 Project" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include <xiexie/xvm.h>


static void shell_print_help()
{
    xvm_log_println("usage:");
    #ifdef _ENABLE_RUNTIME_DEBUGGER_
    xvm_log_println("  xvm-rtd [options] FILE ... executes the specified virtual program");
    #else
    xvm_log_println("  xvm [options] FILE ....... executes the specified virtual program");
    #endif
    xvm_log_println("options:");
    xvm_log_println("  help ..................... prints the help information");
    xvm_log_println("  version .................. prints the version and license note");
    xvm_log_println("  -v, --verbose ............ prints additional output before and after program execution");
    xvm_log_println("  -s, --stack SIZE ......... sets the stack size (by default 256)");
    xvm_log_println("  -m, --module FILE ........ loads the module, specified by FILE");
    xvm_log_println("  -e, --entry LABEL ........ sets the main entry point, specified by LABEL");
}

static void shell_print_version()
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

static xvm_string extract_file_path(const char* filename)
{
    // Determine path length
    size_t pos = strlen(filename);
    if (pos > 0)
    {
        do
        {
            --pos;
        }
        while (filename[pos] != '/' && filename[pos] != '\\');
    }
    
    // Create path string
    xvm_string path = xvm_string_create_from_sub(filename, pos);

    // Append "module/" to path string
    #ifndef XIEXIE_RELEASE_VERSION
    xvm_string_append(&path, "/../repository/modules/");
    #else
    xvm_string_append(&path, "/../modules/");
    #endif

    return path;
}

static int shell_parse_args(const char* app_path, int argc, char* argv[])
{
    // Configuration memory
    int         verbose     = 0;
    const char* filename    = NULL;
    const char* entry       = NULL;
    size_t      stack_size  = 256;
    xvm_string  module_path = extract_file_path(app_path);
    
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
        if (strcmp(arg, "help") == 0)
            shell_print_help();
        else if (strcmp(arg, "version") == 0)
            shell_print_version();
        else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--verbose") == 0)
            verbose = 1;
        else if (strcmp(arg, "-m") == 0 || strcmp(arg, "--module") == 0)
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

                // Get next argument
                ++argv;
                --argc;
            }
            else
            {
                xvm_log_error("expected argument after \"-m\" and \"--module\" flag");
                return 0;
            }
        }
        else if (strcmp(arg, "-e") == 0 || strcmp(arg, "--entry") == 0)
        {
            if (argc > 0)
            {
                // Get parameter from next argument
                entry = *argv;

                // Get next argument
                ++argv;
                --argc;
            }
            else
            {
                xvm_log_error("expected argument after \"-e\" and \"--entry\" flag");
                return 0;
            }
        }
        else if (strcmp(arg, "-s") == 0 || strcmp(arg, "--stack") == 0)
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
                xvm_log_error("expected argument after \"-s\" and \"--stack\" flag");
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

        // Load automatic modules
        for (size_t i = 0; i < byte_code.num_module_names; ++i)
        {
            // Setup module filename
            xvm_string filename = xvm_string_create_from(module_path.str);
            const char* module_name = byte_code.module_names[i].str;

            xvm_string_append(&filename, module_name);

            #if defined(_WIN32)
            xvm_string_append(&filename, ".dll");
            #elif defined(__linux__)
            xvm_string_append(&filename, ".so");
            #endif

            // Load module
            xvm_module module;
            xvm_module_init(&module);
                
            if (xvm_module_load(&module, filename.str) != 0)
                xvm_module_container_add(&module_cont, module);

            // Free module filename
            xvm_string_free(&filename);
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
        xvm_exit_codes exit_code = EXITCODE_SUCCESS;
        
        if (entry != NULL)
            exit_code = xvm_execute_program_entry_point(&byte_code, &stack, entry);
        else
            exit_code = xvm_execute_program(&byte_code, &stack);

        if (exit_code != EXITCODE_SUCCESS)
            xvm_log_exitcode_error(exit_code);
        else if (verbose != 0)
            xvm_log_println("program terminated successful");

        // Clean up byte code
        xvm_bytecode_free(&byte_code);
        xvm_stack_free(&stack);
    }

    // Clean up shell
    xvm_module_container_clear(&module_cont);
    xvm_string_free(&module_path);

    return 1;
}

// Main function for the shell
int main(int argc, char* argv[])
{
    // Ignore program path argument, then parse all other arguments
    const char* app_path = argv[0];
    shell_parse_args(app_path, --argc, ++argv);

    // Check for memory leaks
    int ref_count = xvm_memory_ref_count();
    if (ref_count != 0)
        xvm_log_warning("%i memory leak(s) detected", ref_count);

    return 0;
}


