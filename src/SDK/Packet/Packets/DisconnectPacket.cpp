#include "SDK/Packet/Packets/DisconnectPacket.h"

DisconnectPacket::DisconnectPacket()
{
    this->reason = DisconnectFailReason::Unknown;
    this->hideDisconnectReason = false;
}

DisconnectPacket::DisconnectPacket(const std::string_view packetData)
{
    BinaryReader::Reader packetReader(packetData);

    this->reason = static_cast<DisconnectFailReason>(packetReader.readZigZag32());
    this->hideDisconnectReason = packetReader.readBool();

    if (!hideDisconnectReason) {
        this->message = packetReader.readString();
        this->filteredMessage = packetReader.readString();
    }
}

std::string DisconnectPacket::serialize() const
{
    BinaryWriter::Writer writer;
    writer.writeZigZag32(static_cast<int32_t>(this->reason));
    writer.writeBool(this->hideDisconnectReason);
    if (!hideDisconnectReason) {
        writer.writeString(this->message.value_or(""));
        writer.writeString(this->filteredMessage.value_or(""));
    }

    return writer.getString();
}
