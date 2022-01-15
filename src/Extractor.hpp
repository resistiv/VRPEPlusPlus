#ifndef EXTRACTOR_H
#define EXTRACTOR_H
using namespace std;

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>

#define BUFF_SIZE 1024

class Extractor
{
    public:
        Extractor(string fileName);
        int ExtractFiles();
        bool IsReady() const;
    private:
        void ReadTOC();
        int CreateDir(string directory);
        int CreateSubDirs(string topDir, string workingList);
        bool isReady = false;
        string fileName;
        ifstream inFile;
        int fileCount;
        vector<int> fileIndexes;
        string outputDir;
        char outBuf[BUFF_SIZE];
};

#endif