#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cstdint>

struct ImageDef
{
    int16_t bw;
    int16_t bh;
    uint8_t *bitmap;
};

int main()
{
    std::ifstream file("out/data", std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> dataBuffer(fileSize);
    file.read(dataBuffer.data(), fileSize);
    if (!file)
    {
        std::cerr << "Error reading the file." << std::endl;
        return 1;
    }

    file.close();

    // Open the file
    std::ifstream inputFile("out/info");
    if (!inputFile)
    {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    int16_t firstValue;
    int16_t secondValue;
    std::string fileContents;
    inputFile.seekg(0, std::ios::end);
    fileContents.reserve(inputFile.tellg());
    inputFile.seekg(0, std::ios::beg);
    fileContents.assign((std::istreambuf_iterator<char>(inputFile)),
                        std::istreambuf_iterator<char>());

    inputFile.close();

    std::vector<std::string> lines;

    std::string line;
    std::istringstream iss(fileContents);
    while (std::getline(iss, line))
    {
        lines.push_back(line);
    }
    firstValue = std::stoi(lines[0]);
    secondValue = std::stoi(lines[1]);

    uint8_t *bitmap = new uint8_t[dataBuffer.size()];
    std::copy(dataBuffer.begin(), dataBuffer.end(), bitmap);
    ImageDef img = {firstValue, secondValue, bitmap};

    {

        std::ofstream file("out/img.bin", std::ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<char *>(&img.bw), sizeof(img.bw));
            file.write(reinterpret_cast<char *>(&img.bh), sizeof(img.bh));

            file.write(reinterpret_cast<char *>(img.bitmap), dataBuffer.size());

            file.close();
        }
        else
        {
            std::cerr << "Error opening file." << std::endl;
        }
    }

    return 0;
}