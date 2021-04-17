#include <iostream>
#include <string>

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))


using namespace std;

class MD5maker
{
public:
    string MD5(string input);

private:
    //массив содержащий все байты для хешрования
    uint8_t* msg{ nullptr };
    // Вспомогательные функции
    inline uint32_t F(uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (~x & z); }
    inline uint32_t G(uint32_t x, uint32_t y, uint32_t z) { return (x & z) | (~z & y); }
    inline uint32_t H(uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; }
    inline uint32_t I(uint32_t x, uint32_t y, uint32_t z) { return y ^ (~z | x); }

    void to_bytes(uint32_t val, uint8_t* bytes)
    {
        bytes[0] = (uint8_t)val;
        bytes[1] = (uint8_t)(val >> 8);
        bytes[2] = (uint8_t)(val >> 16);
        bytes[3] = (uint8_t)(val >> 24);
    }

    uint32_t to_int32(const uint8_t* bytes)
    {
        return (uint32_t)bytes[0]
            | ((uint32_t)bytes[1] << 8)
            | ((uint32_t)bytes[2] << 16)
            | ((uint32_t)bytes[3] << 24);
    }
    //Вспомогательные константы
    const uint32_t k[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };
    const uint32_t r[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };



};

string MD5maker::MD5(string input)
{
    //Шаг первый: выравнивание потока.
    size_t original_size = distance(input.begin(), input.end());

    size_t new_size = original_size;

    for (; new_size % (512 / 8) != 448 / 8; new_size++);

    msg = (uint8_t*)malloc(new_size + 8);

    copy(input.begin(), input.end(), msg);

    msg[original_size] = 0x80;

    for (size_t offset = original_size + 1; offset < new_size; offset++)
        msg[offset] = 0;

    //Шаг второй: добавление длинны сообщения
    to_bytes((uint16_t)original_size * 8, msg + new_size);
    to_bytes((uint16_t)original_size >> 29, msg + new_size + 4);

    //Шаг третий: Иницализация буфера
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xefcdab89;
    uint32_t h2 = 0x98badcfe;
    uint32_t h3 = 0x10325476;

    //Шаг четвертый:Вычисления в цикле
    uint32_t w[16];

    for (int offset = 0; offset < new_size; offset += (512 / 8))
    {
        for (int i = 0; i < 16; i++)
            w[i] = to_int32(msg + offset + i * 4);

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;

        int f, g, temp;

        for (int i = 0; i < 64; i++) {

            if (i < 16) {
                f = F(b, c, d);
                g = i;
            }
            else if (i < 32) {
                f = G(b, c, d);
                g = (5 * i + 1) % 16;
            }
            else if (i < 48) {
                f = H(b, c, d);
                g = (3 * i + 5) % 16;
            }
            else {
                f = I(b, c, d);
                g = (7 * i) % 16;
            }
            temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;
        }


        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
    }
    //Шаг пятый: результат вычислений 

    free(msg);

    uint8_t result[16];

    to_bytes(h0, result);
    to_bytes(h1, result + 4);
    to_bytes(h2, result + 8);
    to_bytes(h3, result + 12);

    string hex_char = "0123456789abcdef";
    string ret = "";

    for (int i = 0; i < 16; ++i) {
        ret += hex_char[result[i] >> 4];
        ret += hex_char[result[i] & 0x0F];
    }


    return ret;
}


int main()
{
    MD5maker mk;
    std::cout << mk.MD5("Hello, world!") << endl;
}