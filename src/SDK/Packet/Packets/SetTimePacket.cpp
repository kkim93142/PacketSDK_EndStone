#include "SDK/Packet/Packets/SetTimePacket.h"

SetTimePacket::SetTimePacket(const int32_t time) {
    this->time = time;
}

SetTimePacket::SetTimePacket(const std::string_view packetData) {
    BinaryReader::Reader reader(packetData);
    this->time = reader.readZigZag32();
}

std::string SetTimePacket::serialize() const {
    BinaryWriter::Writer writer;
    writer.writeZigZag32(this->time);

    return writer.getString();
}
