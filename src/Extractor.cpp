#include "Extractor.hpp"

// Constructor
Extractor::Extractor(std::string fileName)
{
    this->fileName = fileName;

    // Attempt file open
    inFile.open(fileName, std::ios::in | std::ios::binary);
    if (!inFile.is_open())
    {
        std::cerr << "Could not open file \"" << fileName << "\" for reading, aborting." << std::endl;
        return;
    }

    // Get full path to input file
    char fullPath[MAX_PATH];
    GetFullPathName(fileName.c_str(), MAX_PATH, fullPath, nullptr);

    // Append output signifier
    outputDir = std::string(fullPath) + "_out";

    // Create main output dir
    if (CreateDir(outputDir) == -1)
        return;

    // Done!
    isReady = true;
}

// Extracts all files
int Extractor::ExtractFiles()
{
    // Read contents
    ReadTOC();

    // Traverse the archive
    for (int i = 0; i < fileCount; i++)
    {
        char nameBuf;
        std::string outFileName = "";
        std::string outSubDir = "";
        int fileLength;

        // Seek to file offset
        inFile.seekg(fileIndexes[i], std::ios_base::beg);

        // Read file name (null-terminated, null-padded to next four byte border)
        inFile.read(&nameBuf, sizeof(nameBuf));
        while (nameBuf != '\0')
        {
            outFileName += nameBuf;
            inFile.read(&nameBuf, sizeof(nameBuf));
        }

        // Fix file name for Windows-style pathing
        replace(outFileName.begin(), outFileName.end(), '/', '\\');

        // Create all needed subdirectories
        if (CreateSubDirs(outputDir, outFileName) == -1)
        {
            std::cerr << "Failed to generate sub-directories for file \"" << outFileName << "\", aborting." << std::endl;
            return -1;
        }

        // Skip to next four-byte border
        inFile.seekg(3 - (outFileName.size() % 4), std::ios_base::cur);

        // Read file length
        inFile.read((char *)&fileLength, sizeof(fileLength));

        // Initialize output file
        std::ofstream outFile(outputDir + "\\" + outFileName, std::ios::out | std::ios::binary);
        if (!outFile.is_open())
        {
            std::cerr << "Could not open file \"" << outputDir + "\\" + outFileName << "\" for writing, aborting." << std::endl;
            return -1;
        }

        // Output file info
        std::cout << std::hex << "[" << outFileName << "] Offset: 0x" << fileIndexes[i] << " | Length: 0x" << fileLength << std::dec << std::endl;

        // Read file
        int bytesLeft = fileLength;
        while (bytesLeft != 0)
        {
            // Bytes to be read
            int bytesRead = bytesLeft >= BUFF_SIZE ? BUFF_SIZE : bytesLeft;
            // Read in needed amount of bytes
            inFile.read((char *)&outBuf, bytesRead);
            // Decrement by bytes read
            bytesLeft -= bytesRead;
            // Write to output
            outFile.write(outBuf, bytesRead);
        }

        // Release handle
        outFile.close();
    }

    // Release handle
    inFile.close();

    return 0;
}

// Reads file count and location; table of contents
void Extractor::ReadTOC()
{
    int temp;

    // Read file count
    inFile.read((char *)&fileCount, sizeof(fileCount));

    // Traverse table of contents
    for (int i = 0; i < fileCount; i++)
    {
        inFile.read((char *)&temp, sizeof(fileCount));
        fileIndexes.push_back(temp);
    }
}

// Creates a directory; returns -1 if the creation failed
int Extractor::CreateDir(std::string directory)
{
    // Create directory
    if (!CreateDirectory(directory.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
    {
        std::cerr << "Failed to create output directory with path \"" << directory << "\", aborting." << std::endl;
        return -1;
    }
    return 0;
}

// Generates all the needed sub-directories for a nested file
int Extractor::CreateSubDirs(std::string topDir, std::string workingList)
{
    // Find dir delimiter
    unsigned int slashIndex = workingList.find("\\");
    if (slashIndex == std::string::npos)
        return 0;

    // Update strings
    topDir += "\\" + workingList.substr(0, slashIndex);
    workingList = workingList.substr(slashIndex + 1);

    // Create directory
    if (CreateDir(topDir) == -1)
        return -1;

    CreateSubDirs(topDir, workingList);

    return 0;
}

bool Extractor::IsReady() const
{
    return isReady;
}