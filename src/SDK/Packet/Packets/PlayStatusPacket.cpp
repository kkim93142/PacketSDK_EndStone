#include "SDK/Packet/Packets/PlayStatusPacket.h"

PlayStatusPacket::PlayStatusPacket(const Status status) {
    this->status = status;
}

PlayStatusPacket::PlayStatusPacket(const std::string_view packetData) {
    BinaryReader::Reader packetReader(packetData);
    this->status = static_cast<Status>(packetReader.readInt32());
}

std::string PlayStatusPacket::serialize() const {
    BinaryWriter::Writer writer;
    writer.writeInt32(static_cast<int32_t>(status));

    return writer.getString();
}