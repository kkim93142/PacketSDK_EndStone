#include "SDK/Packet/Packets/TransferPacket.h"

TransferPacket::TransferPacket(const std::string &serverAddress, const uint16_t port, const bool reloadWorld) {
    this->serverAddress = serverAddress;
    this->port = port;
    this->reloadWorld = reloadWorld;
}

TransferPacket::TransferPacket(const std::string &packetData) {
    BinaryReader::Reader reader(packetData);
    this->serverAddress = reader.readString();
    this->port = reader.readUInt16();
    this->reloadWorld = reader.readBool();
}

std::string TransferPacket::serialize() const {
    BinaryWriter::Writer writer;
    writer.writeString(this->serverAddress);
    writer.writeUInt16(this->port);
    writer.writeBool(this->reloadWorld);

    return writer.getString();
}

