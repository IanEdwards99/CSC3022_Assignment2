//Author: Ian Edwards - EDWIAN004
// processTag.cpp - implementation for tag processing. The sourcefile.

#include <iostream>
#include "processTag.h"
#include <fstream>
#include <sstream>
#include <stack>

using namespace std;

//space variable and ltrim, rtrim and trim are extra methods to trim off any unwanted characters from a string.
const std::string space = " \n\r\t\f\v";

std::vector<unsigned char **> imageSequence; // imageSequence[i][row][col] to get i'th frame's pixel.
 

