#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <spdlog/spdlog.h>

namespace fio
{
    inline void PathToBytes(std::string path, std::vector<char>& outputBuffer){
        std::string data;
        std::ifstream readStream;
        readStream.open(path);
        while(readStream){
            getline(readStream, data);
            outputBuffer.insert(outputBuffer.begin(), data.begin(), data.end());
            spdlog::info("From file read line:\n{}", data);
        }
        readStream.close();

    }
}

