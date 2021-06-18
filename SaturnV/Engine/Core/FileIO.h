#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <string>
#include <vector>
#include "../Log/Log.h"

std::string ReadFile(std::string fileName);
std::vector<char> ReadFile_s(const std::string& filename);
void WriteFile(std::string fileName, std::string content);
std::string GetValue(std::string fileName, std::string key);
bool IsFilePresent(std::string fileName);

#endif FILEIO_H