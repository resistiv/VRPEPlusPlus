#include <iostream>
#include <fstream>
#include "Extractor.hpp"
#include "Globals.hpp"

// Prototypes
int WriteUsage();

int main(int argc, char** argv)
{
    std::cout << "VRPE++ v" << VERSION << " by Kaitlyn NeSmith" << std::endl;

    // Test args
    if (argc != 2)
        return WriteUsage();

    // Instantiate our extractor
    Extractor ext(argv[1]);
    if (!ext.IsReady())
    {
        std::cerr << "Extractor initialization failed, aborting." << std::endl;
        return EXIT_FAILURE;
    }

    // Extract files, check for error
    if (ext.ExtractFiles() == -1)
    {
        std::cerr << "Extraction failed, aborting." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Done." << std::endl;

    return EXIT_SUCCESS;
}

// Writes usage to output
int WriteUsage()
{
    std::cout << "Usage: VRPE" << USAGE << std::endl;
    return EXIT_FAILURE;
}