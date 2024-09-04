#include "iostream"
#include "fstream"
#include "cinttypes"


class ToBmp {
public:
    ToBmp(uint64_t width, uint64_t height);
    uint32_t GetColor(uint64_t grains);
    void Export(const std::string& path, uint64_t**& grid);
private:
    uint64_t m_width;
    uint64_t m_height;
    static const int palette_size = 16;
    static const int file_header_size = 14;
    static const int information_header_size = 40;
    const int file_size = file_header_size + information_header_size + palette_size;
    uint32_t palette[palette_size] = {
            0x00FFFFFF,
            0x0000FF00,
            0x00FFFF00,
            0x008B00FF,
            0x00000000
    };
};


