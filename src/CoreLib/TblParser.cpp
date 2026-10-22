#include "TblParser.hpp"

#include <algorithm>

namespace D2ModGen {

using namespace Mernel;

namespace {

// clang-format off
static const uint16_t CRCTable[256] =  {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF, 
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE, 
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D, 
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC, 
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823, 0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B, 
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78, 
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067, 
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256, 
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634, 
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3, 
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92, 
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1, 
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
// clang-format on

}

void TblStructure::readBinary(ByteOrderDataStreamReader& stream)
{
    TblHeader header = {};
    stream >> header;
    stream.zeroPadding(header.NodesNumber * sizeof(uint16_t)); // indices

    std::vector<TblHashNode> hashNodes(header.HashTableSize);
    for (TblHashNode& node : hashNodes) {
        stream >> node;
    }
    auto        remain = stream.getBuffer().getRemainRead();
    std::string buf;
    buf.resize(remain + 1);
    stream.readBlock(buf.data(), remain);

    for (uint16_t i = 0; i < header.HashTableSize; i++) {
        if (!hashNodes.at(i).Active) // current entry is not used, i.e. it's deleted
            continue;

        uint32_t    stringOffset = hashNodes.at(i).StringValOffset - header.DataStartOffset;
        std::string val;
        val = std::string(buf.data() + stringOffset);

        uint32_t keyOffset = hashNodes.at(i).StringKeyOffset - header.DataStartOffset;
        auto     key       = std::string(buf.data() + keyOffset);
        m_rows.push_back(DataNode{ key, val, keyOffset });
    }
    std::sort(m_rows.begin(), m_rows.end(), [](const DataNode& l, const DataNode& r) { return l.Offset < r.Offset; });
}

void TblStructure::writeBinary(ByteOrderDataStreamWriter& stream) const
{
    auto entriesNumber = (uint16_t) m_rows.size();

    uint32_t                 dataStartOffset = TblHeader::size + entriesNumber * sizeof(uint16_t) + entriesNumber * TblHashNode::size;
    std::vector<uint16_t>    indices(entriesNumber);
    std::vector<TblHashNode> nodes(entriesNumber);
    std::vector<uint8_t>     collisionsDetected(entriesNumber);
    uint32_t                 currentOffset = dataStartOffset, maxCollisionsNumber = 0;
    for (uint16_t i = 0; i < entriesNumber; i++) {
        const std::string currentKey              = m_rows[i].Key;
        const std::string currentVal              = m_rows[i].Val;
        uint32_t          hashValue               = TblStructure::hashValue(currentKey.data(), entriesNumber);
        uint32_t          hashIndex               = hashValue;
        uint32_t          currentCollisionsNumber = 0;

        while (collisionsDetected[hashIndex]) // counting collisions for current hash value
        {
            currentCollisionsNumber++;
            hashIndex++;
            hashIndex %= entriesNumber;
        }
        collisionsDetected[hashIndex] = true;
        if (currentCollisionsNumber > maxCollisionsNumber)
            maxCollisionsNumber = currentCollisionsNumber;
        indices[i] = hashIndex;

        uint16_t currentKeyLength = currentKey.size() + 1;
        uint16_t currentValLength = currentVal.size() + 1;
        nodes[hashIndex]          = TblHashNode{ 1, i, hashValue, currentOffset, currentOffset + currentKeyLength, currentValLength };
        currentOffset += currentKeyLength + currentValLength;
    }
    TblHeader header = {};
    stream << header; // header will be filled in the end, so we reserve bytes for it

    // current offset for writing = 0x15
    for (uint16_t i = 0; i < entriesNumber; i++)
        stream << indices[i];

    for (uint16_t i = 0; i < entriesNumber; i++)
        stream << nodes[i];

    for (uint16_t i = 0; i < entriesNumber; i++) {
        const std::string currentKey = m_rows[i].Key;
        const std::string currentVal = m_rows[i].Val;
        stream.writeBlock(currentKey.c_str(), currentKey.size() + 1);
        stream.writeBlock(currentVal.c_str(), currentVal.size() + 1);
    }

    uint32_t         fileSize   = stream.getBuffer().getSize();
    std::string_view dataForCrc = std::string_view((const char*) stream.getBuffer().begin() + dataStartOffset, fileSize - dataStartOffset);

    stream.getBuffer().setOffsetWrite(0); // current offset for writing = 0x00 (to write the header)
    header.CRC             = TblStructure::getCRC(dataForCrc);
    header.NodesNumber     = entriesNumber;
    header.HashTableSize   = entriesNumber;
    header.Version         = 1;
    header.DataStartOffset = dataStartOffset;
    header.HashMaxTries    = maxCollisionsNumber + 1;
    header.FileSize        = fileSize;

    stream << header;
}

void TblStructure::readFromBuffer(const std::string& buffer)
{
    ByteArrayHolder blob;
    blob.resize(buffer.size());
    memcpy(blob.data(), buffer.data(), buffer.size());

    ByteOrderBuffer           bobuffer(blob);
    ByteOrderDataStreamReader stream(bobuffer, ByteOrderDataStream::s_littleEndian);

    readBinary(stream);
}

void TblStructure::writeToBuffer(std::string& buffer) const
{
    ByteArrayHolder binaryBuffer;

    ByteOrderBuffer           bobuffer(binaryBuffer);
    ByteOrderDataStreamWriter writer(bobuffer, ByteOrderDataStream::s_littleEndian);
    writeBinary(writer);

    buffer.resize(bobuffer.getSize());

    memcpy(buffer.data(), binaryBuffer.data(), binaryBuffer.size());
}

uint32_t TblStructure::hashValue(std::string_view str, int hashTableSize)
{
    uint32_t hashValue = 0;
    for (char currentChar : str) {
        hashValue *= 0x10;
        hashValue += currentChar;
        if ((hashValue & 0xF0000000) != 0) {
            uint32_t tempValue = hashValue & 0xF0000000;
            tempValue /= 0x01000000;
            hashValue &= 0x0FFFFFFF;
            hashValue ^= tempValue;
        }
    }
    return hashValue % hashTableSize;
}

uint16_t TblStructure::getCRC(std::string_view str)
{
    uint16_t CRCValue = 0xFFFF;
    for (char c : str) {
        uint16_t charvalue = uint8_t(c) ^ ((CRCValue & 0xFFFF) >> 8), temp = (CRCValue & 0xFF) << 8;
        CRCValue = CRCTable[charvalue] ^ temp;
    }
    return CRCValue;
}

void TblHeader::readBinary(ByteOrderDataStreamReader& stream)
{
    stream >> CRC >> NodesNumber >> HashTableSize >> Version >> DataStartOffset >> HashMaxTries >> FileSize;
}

void TblHeader::writeBinary(ByteOrderDataStreamWriter& stream) const
{
    stream << CRC << NodesNumber << HashTableSize << Version << DataStartOffset << HashMaxTries << FileSize;
}

void TblHashNode::readBinary(ByteOrderDataStreamReader& stream)
{
    stream >> Active >> Index >> HashValue >> StringKeyOffset >> StringValOffset >> StringValLength;
}

void TblHashNode::writeBinary(ByteOrderDataStreamWriter& stream) const
{
    stream << Active << Index << HashValue << StringKeyOffset << StringValOffset << StringValLength;
}

}
