#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>

#define BUFF_SIZE 1024

class Extractor
{
    public:
        Extractor(std::string fileName);
        int ExtractFiles();
        bool IsReady() const;
    private:
        void ReadTOC();
        int CreateDir(std::string directory);
        int CreateSubDirs(std::string topDir, std::string workingList);
        bool isReady = false;
        std::string fileName;
        std::ifstream inFile;
        int fileCount;
        std::vector<int> fileIndexes;
        std::string outputDir;
        char outBuf[BUFF_SIZE];
};

#endif