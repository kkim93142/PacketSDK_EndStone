#include "SDK/Packet/Packets/ServerToClientHandshakePacket.h"

ServerToClientHandshakePacket::ServerToClientHandshakePacket(const std::string token) {
    this->token = token;
}

ServerToClientHandshakePacket::ServerToClientHandshakePacket(const std::string_view packetData) {
    BinaryReader::Reader packetReader(packetData);
    this->token = packetReader.readString();
}

std::string ServerToClientHandshakePacket::serialize() const {
    BinaryWriter::Writer writer;
    writer.writeString(this->token);

    return writer.getString();
}
