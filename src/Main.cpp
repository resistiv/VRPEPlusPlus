#include <iostream>
#include <fstream>
#include "Extractor.hpp"
#include "Globals.hpp"
using namespace std;

// Prototypes
int WriteUsage();

int main(int argc, char** argv)
{
    cout << "VRPE++ v" << VERSION << " by Kaitlyn NeSmith" << endl;

    // Test args
    if (argc != 2)
        return WriteUsage();

    // Instantiate our extractor
    Extractor ext(argv[1]);
    if (!ext.IsReady())
    {
        cerr << "Extractor initialization failed, aborting." << endl;
        return EXIT_FAILURE;
    }

    // Extract files, check for error
    if (ext.ExtractFiles() == -1)
    {
        cerr << "Extraction failed, aborting." << endl;
        return EXIT_FAILURE;
    }

    cout << "Done." << endl;

    return EXIT_SUCCESS;
}

// Writes usage to output
int WriteUsage()
{
    cout << "Usage: VRPE" << USAGE << endl;
    return EXIT_FAILURE;
}