#include <iostream>

#include "stdafx.h"
#include "Keccak.h"
#include "ParserCommon.h"
#include "RAII_Wrapper.h"
#include <cstdlib>
#include "Hex.h"
#include "Base64.h"



enum class HashType { Sha3, Keccak, Shake };
enum class OutputFormat { Hex, Base64 };

struct options
{
    HashType type;
    unsigned int hashWidth;
    unsigned int shakeDigestLength;
    OutputFormat output;
};


unsigned int sha3widths[] = {224, 256, 384, 512, 0};
unsigned int keccakwidths[] = {224, 256, 384, 512, 0};
unsigned int shakewidths[] = {128, 256, 0};

unsigned int bufferSize = 1024 * 4;

template<typename F>
int readFileIntoFunc(const char *fileName, F f)
{
    FILE *fHand = fopen(fileName, "rb");
    if (!fHand)
    {
        std::cerr << "Unable to open input file: " << fileName << "\n";
        return 0;
    }
    FileHandleWrapper fhw(fHand);

    fseek(fHand, 0, SEEK_SET);
    ArrayWrapper<char> buf(bufferSize);
    while (true)
    {
        unsigned int bytesRead = fread((void *)buf.data, 1, bufferSize, fHand);

        f((uint8_t*)buf.data, bytesRead);
        if (bytesRead < bufferSize)
        {
            break;
        }
    }

    return 1;
};

template<typename F1>
int hashFile(const char *fileName, const std::string &hashName, F1 &hashObj, OutputFormat outpt)
{
    //unsigned int hashSize = hashWidth;

    if (readFileIntoFunc(fileName, [&hashObj](uint8_t* buf, unsigned int bLength){ hashObj.addData(buf, 0, bLength); }) == 0)
    {
        return 0;
    }

    std::vector<unsigned char> op = hashObj.digest();

    std::cout << hashName << " " << fileName << ": ";
    if (outpt == OutputFormat::Hex)
    {
        std::ostringstream b;
        for (auto& oi : op)
        {
            Hex(oi, [&b](unsigned char a) { b << a; });
        }
        std::cout << b.str();
    }
    else
    {
        std::string dest;
        size_t len = op.size();
        dest.resize(base64::encoded_size(len));
        dest.resize(base64::encode(&dest[0], op.data(), len));
        std::cout << dest;
    }
    std::cout << "\n";
    return 1;
}

int doFile(const char *fileName, options &opt)
{
    if(opt.type == HashType::Sha3)
    {
        //  SHA-3
        Sha3 h(opt.hashWidth);
        std::ostringstream description;
        description << "SHA-3-" << opt.hashWidth;
        return hashFile(fileName, description.str(), h, opt.output);
    }
    else if (opt.type == HashType::Keccak)
    {
        // Keccak
        Keccak h(opt.hashWidth);
        std::ostringstream description;
        description << "Keccak-" << opt.hashWidth;
        return hashFile(fileName, description.str(), h, opt.output);
    }
    else if (opt.type == HashType::Shake)
    {
        // SHAKE
        Shake h(opt.hashWidth, opt.shakeDigestLength);
        std::ostringstream description;
        description << "SHAKE-" << opt.shakeDigestLength;
        return hashFile(fileName, description.str(), h, opt.output);

    }
    return 0;
}



int main() {
    options opt;
    // Default options
    opt.type = HashType::Keccak;
    opt.hashWidth = 512;
    opt.shakeDigestLength = 512;
    opt.output = OutputFormat::Hex;
    doFile("../input.txt", opt);
    return 0;
}
