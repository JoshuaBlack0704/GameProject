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
        readStream.open(path, std::ios::ate | std::ios::binary);
        uint64_t size = readStream.tellg();
        outputBuffer.resize(size);
        readStream.seekg(0);
        readStream.read(outputBuffer.data(), static_cast<uint32_t>(size));
        readStream.close();
    }
}

