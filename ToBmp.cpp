#include "ToBmp.h"


uint32_t ToBmp::GetColor(uint64_t grains) {
    if (grains < 4) {
        return grains;
    }
    return 4;

}


ToBmp::ToBmp(uint64_t width, uint64_t height)
    : m_width(width), m_height(height)
{
}
void ToBmp::Export(const std::string& path, uint64_t**& grid) {
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);
    if (!f.is_open()) {
        std::cout << "Cant open file\n";
        return;
    }
    uint64_t row = m_width / 2 + m_width % 2;
    const int paddingAmount = ((4 - (row) % 4) % 4);
    int total = (row + paddingAmount) * m_height;
    unsigned char fileHeader[file_header_size];
    //File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';

    //File size
    fileHeader[2] = file_size;
    fileHeader[3] = file_size >> 8;
    fileHeader[4] = file_size >> 16;
    fileHeader[5] = file_size >> 24;

    //Reserved 1
    fileHeader[6] = 0;
    fileHeader[7] = 0;

    //Reserved 2
    fileHeader[8] = 0;
    fileHeader[9] = 0;

    //Pixel data offset
    fileHeader[10] = file_size;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;
    unsigned char informationHeader[information_header_size];

    //Header size
    informationHeader[0] = information_header_size;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;

    //Image width
    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;

    //Image height
    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;

    //Planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;

     //Bits per pixel
    informationHeader[14] = 4;
    informationHeader[15] = 0;

    //Compression
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;

    //Image size
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;

    //X pixels per meter
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;

    //Y pixels per meter
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;

    //Total colors
    informationHeader[32] = 16;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;

    //Important colors
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;
    f.write(reinterpret_cast<char*>(fileHeader), file_header_size);
    f.write(reinterpret_cast<char*>(informationHeader), information_header_size);
    f.write(reinterpret_cast<char*>(palette), palette_size);

    uint8_t* pixels = new uint8_t[total];
    uint8_t* ptr = pixels;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; x += 2) {
            uint64_t first = grid[y][x];
            uint64_t second = 0;
            if (x + 1 < m_width) {
                second = grid[y][x + 1];
            }
            uint32_t color1 = GetColor(first);
            uint32_t color2 = GetColor(second);

            *(ptr++) = static_cast<uint8_t>((color1 << 4) | color2);
        }
        ptr += paddingAmount;
    }
    f.write(reinterpret_cast<char*>(pixels), total);
    delete [] pixels;
    f.close();
    std::cout << "File created\n";
}





