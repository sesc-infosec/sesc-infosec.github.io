#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


class Md5
{
	public:
		static void calc_hash(
            const uint8_t* line,
            uint64_t line_length,
            uint8_t* cache
            );
};


#define __MD5F(a, b, c) ((a&b)|((~a)&c))
#define __MD5G(a, b, c) ((a&c)|(b&(~c)))
#define __MD5H(a, b, c) (a^b^c)
#define __MD5I(a, b, c) (b^(a|(~c)))

//циклический сдвиг влево a на n бит
#define __MD5R_LEFT(a, n) ((a<<n)|(a>>(32 - n)))

//основные функции 
#define __MD5FF(a, b, c, d, m, s, t)   \
    a+= __MD5F(b, c, d) + *m + (uint32_t)t; \
    a = __MD5R_LEFT (a, s); \
    a+= b;
#define __MD5GG(a, b, c, d, m, s, t)   \
    a+= __MD5G(b, c, d) + *m + (uint32_t)t; \
    a = __MD5R_LEFT (a, s); \
    a+= b;
#define __MD5HH(a, b, c, d, m, s, t)   \
    a+= __MD5H(b, c, d) + *m + (uint32_t)t; \
    a = __MD5R_LEFT (a, s); \
    a+= b;
#define __MD5II(a, b, c, d, m, s, t)   \
    a+= __MD5I(b, c, d) + *m + (uint32_t)t; \
    a = __MD5R_LEFT (a, s); \
    a+= b;


void Md5::calc_hash(
    const uint8_t *line,
    uint64_t line_length,
    uint8_t *cache
    )
{
    unsigned char *ext_line;
    int ext_length;

    //начльная инициализация ABCD 
    uint32_t A = 0x67452301, B = 0xefcdab89, C = 0x98badcfe, D = 0x10325476;
    uint32_t a, b, c, d;
    uint32_t *m;

    //длина массива должна быть кратна 512 битам
    ext_length = ((line_length + 8) / 64 + 1) * 64;
    ext_line = (unsigned char*)malloc(ext_length);

    //начало строк одинаковое
    memcpy(ext_line, line, line_length);
    //дополнение строки одним ненулевым и нулевыми битами
    ext_line[line_length] = 128;
    memset(ext_line + line_length + 1, 0, ext_length - line_length - 8);

    uint64_t temp = line_length * 8;
    memcpy(ext_line + ext_length - 8, &temp, 8);

    //начальная инициализация abcd
    for (int i = 0; i < ext_length; i += 64)
    {
        a = A, b = B, c = C, d = D;

        //очередной блок в 512 бит
        unsigned char *temp_line = ext_line + i;
        m = (uint32_t*)(&temp_line[4 * 0]); __MD5FF(a, b, c, d, m, 7, 0xd76aa478);
        m = (uint32_t*)(&temp_line[4 * 1]); __MD5FF(d, a, b, c, m, 12, 0xe8c7b756);
        m = (uint32_t*)(&temp_line[4 * 2]); __MD5FF(c, d, a, b, m, 17, 0x242070db);
        m = (uint32_t*)(&temp_line[4 * 3]); __MD5FF(b, c, d, a, m, 22, 0xc1bdceee);
        m = (uint32_t*)(&temp_line[4 * 4]); __MD5FF(a, b, c, d, m, 7, 0xf57c0faf);
        m = (uint32_t*)(&temp_line[4 * 5]); __MD5FF(d, a, b, c, m, 12, 0x4787c62a);
        m = (uint32_t*)(&temp_line[4 * 6]); __MD5FF(c, d, a, b, m, 17, 0xa8304613);
        m = (uint32_t*)(&temp_line[4 * 7]); __MD5FF(b, c, d, a, m, 22, 0xfd469501);
        m = (uint32_t*)(&temp_line[4 * 8]); __MD5FF(a, b, c, d, m, 7, 0x698098d8);
        m = (uint32_t*)(&temp_line[4 * 9]); __MD5FF(d, a, b, c, m, 12, 0x8b44f7af);
        m = (uint32_t*)(&temp_line[4 * 10]); __MD5FF(c, d, a, b, m, 17, 0xffff5bb1);
        m = (uint32_t*)(&temp_line[4 * 11]); __MD5FF(b, c, d, a, m, 22, 0x895cd7be);
        m = (uint32_t*)(&temp_line[4 * 12]); __MD5FF(a, b, c, d, m, 7, 0x6b901122);
        m = (uint32_t*)(&temp_line[4 * 13]); __MD5FF(d, a, b, c, m, 12, 0xfd987193);
        m = (uint32_t*)(&temp_line[4 * 14]); __MD5FF(c, d, a, b, m, 17, 0xa679438e);
        m = (uint32_t*)(&temp_line[4 * 15]); __MD5FF(b, c, d, a, m, 22, 0x49b40821);

        m = (uint32_t*)(&temp_line[4 * 1]); __MD5GG(a, b, c, d, m, 5, 0xf61e2562);
        m = (uint32_t*)(&temp_line[4 * 6]); __MD5GG(d, a, b, c, m, 9, 0xc040b340);
        m = (uint32_t*)(&temp_line[4 * 11]); __MD5GG(c, d, a, b, m, 14, 0x265e5a51);
        m = (uint32_t*)(&temp_line[4 * 0]); __MD5GG(b, c, d, a, m, 20, 0xe9b6c7aa);
        m = (uint32_t*)(&temp_line[4 * 5]); __MD5GG(a, b, c, d, m, 5, 0xd62f105d);
        m = (uint32_t*)(&temp_line[4 * 10]); __MD5GG(d, a, b, c, m, 9, 0x02441453);
        m = (uint32_t*)(&temp_line[4 * 15]); __MD5GG(c, d, a, b, m, 14, 0xd8a1e681);
        m = (uint32_t*)(&temp_line[4 * 4]); __MD5GG(b, c, d, a, m, 20, 0xe7d3fbc8);
        m = (uint32_t*)(&temp_line[4 * 9]); __MD5GG(a, b, c, d, m, 5, 0x21e1cde6); //?
        m = (uint32_t*)(&temp_line[4 * 14]); __MD5GG(d, a, b, c, m, 9, 0xc33707d6);
        m = (uint32_t*)(&temp_line[4 * 3]); __MD5GG(c, d, a, b, m, 14, 0xf4d50d87);
        m = (uint32_t*)(&temp_line[4 * 8]); __MD5GG(b, c, d, a, m, 20, 0x455a14ed);
        m = (uint32_t*)(&temp_line[4 * 13]); __MD5GG(a, b, c, d, m, 5, 0xa9e3e905);
        m = (uint32_t*)(&temp_line[4 * 2]); __MD5GG(d, a, b, c, m, 9, 0xfcefa3f8);
        m = (uint32_t*)(&temp_line[4 * 7]); __MD5GG(c, d, a, b, m, 14, 0x676f02d9);
        m = (uint32_t*)(&temp_line[4 * 12]); __MD5GG(b, c, d, a, m, 20, 0x8d2a4c8a);

        m = (uint32_t*)(&temp_line[4 * 5]); __MD5HH(a, b, c, d, m, 4, 0xfffa3942);
        m = (uint32_t*)(&temp_line[4 * 8]); __MD5HH(d, a, b, c, m, 11, 0x8771f681);
        m = (uint32_t*)(&temp_line[4 * 11]); __MD5HH(c, d, a, b, m, 16, 0x6d9d6122);
        m = (uint32_t*)(&temp_line[4 * 14]); __MD5HH(b, c, d, a, m, 23, 0xfde5380c);
        m = (uint32_t*)(&temp_line[4 * 1]); __MD5HH(a, b, c, d, m, 4, 0xa4beea44);
        m = (uint32_t*)(&temp_line[4 * 4]); __MD5HH(d, a, b, c, m, 11, 0x4bdecfa9);
        m = (uint32_t*)(&temp_line[4 * 7]); __MD5HH(c, d, a, b, m, 16, 0xf6bb4b60);
        m = (uint32_t*)(&temp_line[4 * 10]); __MD5HH(b, c, d, a, m, 23, 0xbebfbc70);
        m = (uint32_t*)(&temp_line[4 * 13]); __MD5HH(a, b, c, d, m, 4, 0x289b7ec6);
        m = (uint32_t*)(&temp_line[4 * 0]); __MD5HH(d, a, b, c, m, 11, 0xeaa127fa);
        m = (uint32_t*)(&temp_line[4 * 3]); __MD5HH(c, d, a, b, m, 16, 0xd4ef3085);
        m = (uint32_t*)(&temp_line[4 * 6]); __MD5HH(b, c, d, a, m, 23, 0x04881d05);
        m = (uint32_t*)(&temp_line[4 * 9]); __MD5HH(a, b, c, d, m, 4, 0xd9d4d039);
        m = (uint32_t*)(&temp_line[4 * 12]); __MD5HH(d, a, b, c, m, 11, 0xe6db99e5);
        m = (uint32_t*)(&temp_line[4 * 15]); __MD5HH(c, d, a, b, m, 16, 0x1fa27cf8);
        m = (uint32_t*)(&temp_line[4 * 2]); __MD5HH(b, c, d, a, m, 23, 0xc4ac5665);

        m = (uint32_t*)(&temp_line[4 * 0]); __MD5II(a, b, c, d, m, 6, 0xf4292244);
        m = (uint32_t*)(&temp_line[4 * 7]); __MD5II(d, a, b, c, m, 10, 0x432aff97);
        m = (uint32_t*)(&temp_line[4 * 14]); __MD5II(c, d, a, b, m, 15, 0xab9423a7);
        m = (uint32_t*)(&temp_line[4 * 5]); __MD5II(b, c, d, a, m, 21, 0xfc93a039);
        m = (uint32_t*)(&temp_line[4 * 12]); __MD5II(a, b, c, d, m, 6, 0x655b59c3);
        m = (uint32_t*)(&temp_line[4 * 3]); __MD5II(d, a, b, c, m, 10, 0x8f0ccc92);
        m = (uint32_t*)(&temp_line[4 * 10]); __MD5II(c, d, a, b, m, 15, 0xffeff47d);
        m = (uint32_t*)(&temp_line[4 * 1]); __MD5II(b, c, d, a, m, 21, 0x85845dd1);
        m = (uint32_t*)(&temp_line[4 * 8]); __MD5II(a, b, c, d, m, 6, 0x6fa87e4f);
        m = (uint32_t*)(&temp_line[4 * 15]); __MD5II(d, a, b, c, m, 10, 0xfe2ce6e0);
        m = (uint32_t*)(&temp_line[4 * 6]); __MD5II(c, d, a, b, m, 15, 0xa3014314);
        m = (uint32_t*)(&temp_line[4 * 13]); __MD5II(b, c, d, a, m, 21, 0x4e0811a1);
        m = (uint32_t*)(&temp_line[4 * 4]); __MD5II(a, b, c, d, m, 6, 0xf7537e82);
        m = (uint32_t*)(&temp_line[4 * 11]); __MD5II(d, a, b, c, m, 10, 0xbd3af235);
        m = (uint32_t*)(&temp_line[4 * 2]); __MD5II(c, d, a, b, m, 15, 0x2ad7d2bb);
        m = (uint32_t*)(&temp_line[4 * 9]); __MD5II(b, c, d, a, m, 21, 0xeb86d391);

        A += a; B += b; C += c; D += d;
    }

    //записываем ABCD в строку - это и есть хэш
    for (int i = 0; i < 4; i++)
    {
        cache[0 + i] = A % 256;
        cache[4 + i] = B % 256;
        cache[8 + i] = C % 256;
        cache[12 + i] = D % 256;
        A /= 256; B /= 256; C /= 256; D /= 256;
    }
}
