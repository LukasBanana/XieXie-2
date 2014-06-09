/*
 * Shell header
 * 
 * This file is part of the "XieXie2-Compiler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __XX_SHELL_COMMAND_H__
#define __XX_SHELL_COMMAND_H__


#include <string>
#include <vector>


class Command
{
    
    public:
        
        //! Command argument list type (array list of strings).
        typedef std::vector<std::string> ArgumentListType;

        /**
        The command constructor parses the command line and stores all arguments.
        \param[in] commandLine Specifies the command line which will be parsed to access all arguments via 'GetArguments'.
        \remarks The following command constructions will result in the respective argument lists:
        \code
        Command("test") -> { "test" }
        Command("test -foo bart") -> { "test", "-foo", "bar" }
        Command("command > foo_bar") -> { "command", ">", "foo_bar" }
        Command("foo \"and bar\"") -> { "foo", "and bar" }
        Command("    ") -> { }
        \endcode
        \note String literals are not treated as in ANSI C, i.e. there are no escape characters!
        \see GetArguments
        */
        Command(const std::string& commandLine);
        Command(const char* commandLine);
        Command(const Command&& other);

        //! Returns the command name of an empty string if the argument list is empty.
        std::string Name() const;

        //! Cast operator to std::string. This will concatenate all arguments and separate them with a space character.
        operator std::string () const;

        //! Returns the argument list (inclusive command name).
        inline const ArgumentListType& GetArguments() const
        {
            return arguments_;
        }

        //! Returns true if the argument list is empty.
        inline bool IsEmpty() const
        {
            return arguments_.empty();
        }

    private:
        
        ArgumentListType arguments_;

};


#endif



// ========================