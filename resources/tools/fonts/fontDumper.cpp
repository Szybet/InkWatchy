#include "out/fonts.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

typedef struct
{
  uint16_t count;
  GFXfont font;
} GFXfontDump;

int main()
{
  GFXfontDump fontDump = {BYTE_COUNT, font};

  std::ofstream ofs("out/font.bin", std::ios::binary);

  if (!ofs)
  {
    std::cerr << "Error opening file for writing" << std::endl;
    return 1;
  }

  // Write count
  ofs.write(reinterpret_cast<const char *>(&fontDump.count), sizeof(fontDump.count));

  // Write bitmap data
  ofs.write(reinterpret_cast<const char *>(fontDump.font.bitmap), BYTE_COUNT);

  // Write glxglyph
  ofs.write(reinterpret_cast<const char *>(fontDump.font.glyph), sizeof(GFXglyph) * 95);

  // Write GFXfont fields
  ofs.write(reinterpret_cast<const char *>(&fontDump.font.first), sizeof(fontDump.font.first));
  ofs.write(reinterpret_cast<const char *>(&fontDump.font.last), sizeof(fontDump.font.last));
  ofs.write(reinterpret_cast<const char *>(&fontDump.font.yAdvance), sizeof(fontDump.font.yAdvance));

  ofs.close();

  //std::cout << "First: " << fontDump.font.first << std::endl;
  //std::cout << "Last: " << fontDump.font.last << std::endl;
  //std::cout << "YAdvance: " << std::dec << static_cast<int>(fontDump.font.yAdvance) << std::endl;
  
  return 0;
}