/* this header contains extra tools
   that are not related to our custom classes,
   it also contains our libraries and api
   specifically SFML ,a library for 3d visualization library
   that can draw different geometrical object that
   may or may not have a texture */

#pragma once

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <math.h>
#include <fstream>

#include <SFML/Graphics.hpp>

void tokenize(std::string str ,std::vector<std::string> &vct);
//takes a string str and splits it up by its empty spaces before placing them in the vector vct

int* getScreenSize();
//return a pointer to the width of system screen whose adjacent location points to the height of it

double lineCountOfStream(std::string filePath);
//returns number of lines in a .txt file whose relative path to program directory is filePath

int stringToInt(std::string gibenString);

bool isInTheBox(int x ,int y,int w,int h,int a ,int b);
//checks whether the point (a,b) is in the box with left upper corner at (x,y) and width and height w and h respectively




