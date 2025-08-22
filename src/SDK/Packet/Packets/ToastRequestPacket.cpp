#include "SDK/Packet/Packets/ToastRequestPacket.h"

ToastRequestPacket::ToastRequestPacket() {
    this->message = "";
    this->title = "";
}

ToastRequestPacket::ToastRequestPacket(const std::string_view packetData) {
    BinaryReader::Reader packetReader(packetData);
    this->title = packetReader.readString();
    this->message = packetReader.readString();
}

std::string ToastRequestPacket::serialize() const {
    BinaryWriter::Writer writer;
    writer.writeString(this->title);
    writer.writeString(this->message);

    return writer.getString();
}
