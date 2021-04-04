//Author: Ian Edwards EDWIAN004
//Basic headerfile for C++ program for processTag.cpp

#ifndef _tag
#define _tag

struct tagStruct { //Structure used in driver file and sourcefile.
   std::string tagname;
   int nrPairs;
   std::string tagText;
};

#include <vector>

namespace EDWIAN004 {
//All function declarations are included below:
void readFile(std::string filename);

void createStruct(char line[100]);

std::vector<tagStruct> stackRead(std::string filename);

void displayVect(std::vector<tagStruct> vect);

void writeFile(std::vector<tagStruct> vect, std::string filename);

void outputTag(std::vector<tagStruct> vect, std::string tagName);

#endif
}
