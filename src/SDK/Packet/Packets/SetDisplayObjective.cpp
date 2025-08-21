#include "SDK/Packet/Packets/SetDisplayObjectivePacket.h"

SetDisplayObjectivePacket::SetDisplayObjectivePacket() {
    this->displaySlot = "";
    this->objectiveName = "";
    this->displayName = "";
    this->criteriaName = "";
    this->sortOrder = 0;
}

SetDisplayObjectivePacket::SetDisplayObjectivePacket(const std::string_view packetData) {
    BinaryReader::Reader reader(packetData);
    this->displaySlot = reader.readString();
    this->objectiveName = reader.readString();
    this->displayName = reader.readString();
    this->criteriaName = reader.readString();
    this->sortOrder = reader.readZigZag32();
}

std::string SetDisplayObjectivePacket::serialize() const {
    BinaryWriter::Writer writer;
    writer.writeString(this->displaySlot);
    writer.writeString(this->objectiveName);
    writer.writeString(this->displayName);
    writer.writeString(criteriaName);
    writer.writeZigZag32(sortOrder);

    return writer.getString();
}
