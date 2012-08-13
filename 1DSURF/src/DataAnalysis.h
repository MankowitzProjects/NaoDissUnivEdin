#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H
#include <fstream>
#include <iostream>
#include <list>
#include <opencv2/opencv.hpp>


#include<string>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

class DataAnalysis
{
    public:
        DataAnalysis();
        virtual ~DataAnalysis();
        int getNumImagesInDirectory(string *dir, bool terminal);
        void help(char**);
    protected:
    private:
};

#endif // DATAANALYSIS_H
