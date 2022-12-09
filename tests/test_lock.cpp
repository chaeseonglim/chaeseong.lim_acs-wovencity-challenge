// Test binary for the challenge

#include <iostream>
#include <algorithm>
#include <Challenge/Lock.hpp>

// I usually use boost::program_options but don't want to add additional dependencies// this time. Copied from https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c
char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

void usage()
{
    // Print out the usage
    std::cout << "Usage: " << std::endl;
}

int main(int argc, char * argv[])
{
    if( argc == 1 || cmdOptionExists( argv, argv + argc, "-h" ) )
    {
        usage();
        return 0;
    }

    char* algorithm = getCmdOption(argv, argv + argc, "-a");

    if (algorithm)
    {
        // Do interesting things
        // ...
    }

    return 0;
}

