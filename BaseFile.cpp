#include "BaseFile.h"

#include <sstream>

SCRANTIC::BaseFile::BaseFile(std::string name) : filename(name)
{
    defaultPalette[0].r = 168;
    defaultPalette[0].g = 0;
    defaultPalette[0].b = 168;
    defaultPalette[0].a = 0;

    defaultPalette[1].r = 0;
    defaultPalette[1].g = 0;
    defaultPalette[1].b = 168;
    defaultPalette[1].a = 255;

    defaultPalette[2].r = 0;
    defaultPalette[2].g = 168;
    defaultPalette[2].b = 0;
    defaultPalette[2].a = 255;

    defaultPalette[3].r = 0;
    defaultPalette[3].g = 168;
    defaultPalette[3].b = 168;
    defaultPalette[3].a = 255;

    defaultPalette[4].r = 168;
    defaultPalette[4].g = 0;
    defaultPalette[4].b = 0;
    defaultPalette[4].a = 255;

    defaultPalette[5].r = 0;
    defaultPalette[5].g = 0;
    defaultPalette[5].b = 0;
    defaultPalette[5].a = 255;

    defaultPalette[6].r = 168;
    defaultPalette[6].g = 168;
    defaultPalette[6].b = 0;
    defaultPalette[6].a = 255;

    defaultPalette[7].r = 212;
    defaultPalette[7].g = 212;
    defaultPalette[7].b = 212;
    defaultPalette[7].a = 255;

    defaultPalette[8].r = 128;
    defaultPalette[8].g = 128;
    defaultPalette[8].b = 128;
    defaultPalette[8].a = 255;

    defaultPalette[9].r = 0;
    defaultPalette[9].g = 0;
    defaultPalette[9].b = 255;
    defaultPalette[9].a = 255;

    defaultPalette[10].r = 0;
    defaultPalette[10].g = 255;
    defaultPalette[10].b = 0;
    defaultPalette[10].a = 255;

    defaultPalette[11].r = 0;
    defaultPalette[11].g = 255;
    defaultPalette[11].b = 255;
    defaultPalette[11].a = 255;

    defaultPalette[12].r = 255;
    defaultPalette[12].g = 0;
    defaultPalette[12].b = 0;
    defaultPalette[12].a = 255;

    defaultPalette[13].r = 255;
    defaultPalette[13].g = 0;
    defaultPalette[13].b = 255;
    defaultPalette[13].a = 255;

    defaultPalette[14].r = 255;
    defaultPalette[14].g = 255;
    defaultPalette[14].b = 0;
    defaultPalette[14].a = 255;

    defaultPalette[15].r = 255;
    defaultPalette[15].g = 255;
    defaultPalette[15].b = 255;
    defaultPalette[15].a = 255;
}

SCRANTIC::BaseFile::~BaseFile()
{

}

std::string SCRANTIC::BaseFile::read_string(std::ifstream *in, u_int8_t length)
{
    if (!in->is_open())
        return "";

    std::streampos pos = in->tellg();
    std::streampos i = 0;
    std::string str = "";
    char c;

    in->read(&c, 1);
    i += 1;
    while (c != '\0')
    {
        str += c;
        in->read(&c, 1);
        i += 1;
        if ((length) && (i > length))
            break;
    }

    if ((length) && (str.length() != length))
        in->seekg(pos + static_cast<std::streampos>(length + 1), std::ios::beg);

    return str;
}

std::string SCRANTIC::BaseFile::read_string(std::vector<u_int8_t>::iterator &it, u_int8_t length)
{
    std::string str = "";
    u_int8_t i = 0;

    char c = (char)*it;
    ++it;
    ++i;

    while (c != '\0')
    {
        str += c;
        c = (char)*it;
        ++i;
        ++it;

        if ((length) && (i > length))
            break;
    }

    if (length)
        for (i = i; i < length; ++i)
            ++it;

    return str;
}

std::string SCRANTIC::BaseFile::read_const_string(std::ifstream *in, u_int8_t length)
{
    if (!in->is_open())
        return "";

    char c;
    std::string str = "";

    for (u_int8_t i = 0; i < length; ++i)
    {
        in->read(&c, 1);
        str += c;
    }

    return str;
}

std::string SCRANTIC::BaseFile::read_const_string(std::vector<u_int8_t>::iterator &it, u_int8_t length)
{
    std::string str = "";

    for (u_int8_t i = 0; i < length; ++i)
    {
        str += (char)*it;
        ++it;
    }

    return str;
}

void SCRANTIC::BaseFile::saveFile(const std::vector<u_int8_t> &data, std::string name, std::string path)
{
    std::ofstream out;

    if (path.length() && (path[path.length()-1] != '/'))
        path += "/";

    out.open(path + name, std::ios::binary | std::ios::out);
    out.unsetf(std::ios::skipws);

    if (!out.is_open())
    {
        std::cerr << "BaseFile: Could not open " << path + name << std::endl;
        return;
    }

    for (auto const &i: data)
        out.write((char*)&i, 1);

    out.close();
}

std::vector<u_int8_t> SCRANTIC::BaseFile::RLEDecompress(std::vector<u_int8_t> const &compressedData, size_t offset, u_int32_t size)
{
    std::vector<u_int8_t> decompressedData;
    u_int8_t byte, length;

    while ((offset < compressedData.size()) && ((size == 0) || (decompressedData.size() < size)))
    {
      byte = compressedData[offset++];
      if ((byte & 0x80) == 0x80)
      {
        length = (u_int8_t)(byte & 0x7F);
        byte = compressedData[offset++];
        for (int i = 0; i < length; ++i)
          decompressedData.push_back(byte);
      }
      else
      {
        for (int i = 0; i < byte; ++i)
          decompressedData.push_back(compressedData[offset++]);
      }
    }

    return decompressedData;
}


std::vector<u_int8_t> SCRANTIC::BaseFile::RLE2Decompress(std::vector<u_int8_t> const &compressedData, size_t offset, u_int32_t size)
{
    std::vector<u_int8_t> decompressedData;
    u_int8_t byte, length;

    while ((offset < compressedData.size()) && ((size == 0) || (decompressedData.size() < size)))
    {
      byte = compressedData[offset++];
      if ((byte & 0x80) == 0x80)
      {
        length = compressedData[offset++];
        for (int i = 0; i < length; ++i)
          decompressedData.push_back(byte & 0x7F);
      }
      else
      {
        for (int i = 0; i < byte; ++i)
          decompressedData.push_back(compressedData[offset++]);
      }
    }

    return decompressedData;
}

u_int16_t SCRANTIC::BaseFile::readBits(std::vector<u_int8_t> const &data, size_t &bytePos, u_int8_t &bitPos, u_int16_t bits)
{
    u_int16_t byte = 0x00;
    for (u_int16_t i = 0; i < bits; ++i)
    {
        if (bytePos >= data.size())
            return byte;

        byte |= ((data[bytePos] >> bitPos) & 1) << i;

                if (bitPos >= 7)
                {
                    bitPos = 0;
                    ++bytePos;
                }
                else
                    ++bitPos;
    }

    return byte;
}

std::vector<u_int8_t> SCRANTIC::BaseFile::LZWDecompress(std::vector<u_int8_t> const &compressedData, size_t offset, u_int32_t size)
{
    std::vector<u_int8_t> decompressedData;
    std::pair<u_int16_t, u_int8_t> dictionary[4096];
    std::vector<u_int8_t> decodeStack;

    u_int8_t bitLength = 9;
    u_int16_t freeDictPos = 257;

    u_int16_t oldcode, newcode, code;
    size_t bytePos = offset;
    u_int8_t bitPos = 0;
    u_int8_t lastbyte;
    u_int32_t bitCounter = 0;

    oldcode = readBits(compressedData, bytePos, bitPos, bitLength);
    lastbyte = oldcode;

    decompressedData.push_back((u_int8_t)oldcode);

    while ((bytePos < compressedData.size()-1) && ((size == 0) || (decompressedData.size() < size)))
    {
        newcode = readBits(compressedData, bytePos, bitPos, bitLength);
        bitCounter += bitLength;

        if (newcode == 256)
        {
            u_int16_t nskip = ((bitLength*8) - ((bitCounter - 1) % (bitLength*8))) - 1;
            readBits(compressedData, bytePos, bitPos, nskip);
            bitLength = 9;
            freeDictPos = 256;
            bitCounter = 0;
            continue;
        }

        code = newcode;
        if (code >= freeDictPos)
        {
            if (decodeStack.size() >= 4096)
                break;
            decodeStack.push_back(lastbyte);
            code = oldcode;
        }

        while (code >= 256)
        {
            if (code > 4095)
                break;
            decodeStack.push_back(dictionary[code].second);
            code = dictionary[code].first;
        }

        decodeStack.push_back((u_int8_t)code);
        lastbyte = (u_int8_t)code;

        for (size_t i = decodeStack.size(); i > 0; --i)
            decompressedData.push_back(decodeStack[i-1]);
        decodeStack.clear();

        if (freeDictPos < 4096)
        {
            dictionary[freeDictPos].first = oldcode;
            dictionary[freeDictPos].second = lastbyte;
            ++freeDictPos;

            if ((bitLength < 12) && (freeDictPos >= (1 << bitLength)))
            {
              ++bitLength;
              bitCounter = 0;
            }
        }

        oldcode = newcode;
    }

    return decompressedData;
}

std::string SCRANTIC::BaseFile::commandToString(Command cmd, bool ads)
{
    std::string ret = " ";
    std::string hex;
    size_t len = 4;
    for (size_t i = 0; i < cmd.data.size(); ++i)
    {
        hex = hex_to_string(cmd.data.at(i), std::hex);
        for (size_t j = hex.size(); j < len; ++j)
            hex = "0" + hex;
        ret += hex + " ";
    }

    if (cmd.name.length())
        ret += cmd.name;
    if (!ads)
    {
        switch (cmd.opcode)
        {
        case CMD_UNK_0080: return "Unkown 0x0080" + ret;
        case CMD_PURGE: return "Purge" + ret;
        case CMD_UPDATE: return "Update" + ret;
        case CMD_DELAY: return "Delay" + ret;
        case CMD_SEL_SLOT_IMG: return "Select Image Slot" + ret;
        case CMD_SEL_SLOT_PAL: return "Select Palette Slot" + ret;
        case CMD_SET_SCENE_LABEL: return "Label Scene" + ret;
        case CMD_SET_SCENE: return "New Scene" + ret;
        case CMD_UNK_1120: return "Unkown 0x1120" + ret;
        case CMD_JMP_SCENE: return "Jump to Scene" + ret;
        case CMD_SET_COLOR: return "Set Color" + ret;
        case CMD_UNK_2010: return "Unkown 0x2010" + ret;
        case CMD_UNK_2020: return "Unkown 0x2020" + ret;
        case CMD_CLIP_REGION: return "Clip Region" + ret;
        case CMD_SAVE_IMAGE: return "Save Image" + ret;
        case CMD_SAVE_IMAGE_NEW: return "Save New Image" + ret;
        case CMD_DRAW_PIXEL: return "Draw Pixel" + ret;
        case CMD_UNK_A050: return "Unkown 0xA050" + ret;
        case CMD_UNK_A060: return "Unkown 0xA060" + ret;
        case CMD_DRAW_LINE: return "Draw Line" + ret;
        case CMD_DRAW_RECTANGLE: return "Draw Rectangle" + ret;
        case CMD_DRAW_ELLIPSE: return "Draw Ellipse" + ret;
        case CMD_DRAW_SPRITE: return "Draw Sprite (normal)" + ret;
        case CMD_DRAW_SPRITE_MIRROR: return "Draw Sprite (mirror)" + ret;
        case CMD_CLEAR_RENDERER: return "Clear Renderer" + ret;
        case CMD_UNK_B600: return "Unkown 0xB600" + ret;
        case CMD_PLAY_SOUND: return "Play Sound" + ret;
        case CMD_LOAD_SCREEN: return "Load Screen" + ret;
        case CMD_LOAD_BITMAP: return "Load Bitmap" + ret;
        case CMD_LOAD_PALETTE: return "Load Palette" + ret;
        default: return "DEFAULT 0x" + hex_to_string(cmd.opcode, std::hex) + ret;
        }
    }
    else
    {
        switch (cmd.opcode)
        {
        // ADS instructions
        case CMD_SET_SCENE: return "New ADS Movie" + ret;
        case CMD_UNK_1070: return "Unkown 0x1070" + ret;
        case CMD_ADD_INIT_TTM: return "Add Init TTM" + ret;
        case CMD_TTM_LABEL: return "Label" + ret;
        case CMD_SKIP_IF_LAST: return "Skip Next If Last" + ret;
        case CMD_UNK_1370: return "Unkown 0x1370" + ret;
        case CMD_OR_SKIP: return "Or (Skip)" + ret;
        case CMD_OR: return "Or (Cond)" + ret;
        case CMD_PLAY_MOVIE: return "Play Movie" + ret;
        case CMD_UNK_1520: return "Unkown 0x1520" + ret;
        case CMD_ADD_TTM: return "Add TTM" + ret;
        case CMD_KILL_TTM: return "Kill TTM" + ret;
        case CMD_RANDOM_START: return "Random Start" + ret;
        case CMD_UNK_3020: return "Unkown 0x3020" + ret;
        case CMD_RANDOM_END: return "Random End" + ret;
        case CMD_UNK_4000: return "Unkown 0x4000" + ret;
        case CMD_UNK_F010: return "Unkown 0xF010" + ret;
        case CMD_PLAY_ADS_MOVIE: return "Play ADS Movie" + ret;
        case CMD_UNK_FFFF: return "Unkown 0xFFFF" + ret;

        default: return "DEFAULT 0x" + hex_to_string(cmd.opcode, std::hex) + ret;
    }
}
}
