#include "SDK/Packet/Packets/SetScorePacket.h"

SetScorePacket::SetScorePacket() {
    this->action = ActionType::Change;
    this->entriesLength = 0;
    this->entries.clear();
}

SetScorePacket::SetScorePacket(const std::string_view packetData) {
    BinaryReader::Reader reader(packetData);

    this->action = static_cast<ActionType>(reader.readUInt8());
    this->entriesLength = reader.readVarInt32();

    for (int i = 0; i < this->entriesLength; i++) {
        Entry entry;
        entry.scoreboardId = reader.readZigZag64();
        entry.objectiveName = reader.readString();
        entry.score = reader.readInt32();

        if (this->action == ActionType::Change) {
            entry.entryType = static_cast<EntryType>(reader.readInt8());

            if (entry.entryType == EntryType::Player || entry.entryType == EntryType::Entity) {
                entry.entityUniqueId = reader.readZigZag64();
            }
            else if (entry.entryType == EntryType::FakePlayer) {
                entry.customName = reader.readString();
            }
        }

        this->entries.push_back(std::move(entry));
    }
}

std::string SetScorePacket::serialize() const {
    BinaryWriter::Writer writer;

    writer.writeUInt8(static_cast<uint8_t>(this->action));
    writer.writeVarInt32(static_cast<int32_t>(this->entries.size()));

    for (const auto& entry : this->entries) {
        writer.writeZigZag64(entry.scoreboardId);
        writer.writeString(entry.objectiveName);
        writer.writeInt32(entry.score);

        if (this->action == ActionType::Change && entry.entryType) {
            writer.writeInt8(static_cast<int8_t>(entry.entryType.value()));

            if (entry.entryType == EntryType::Player || entry.entryType == EntryType::Entity) {
                if (entry.entityUniqueId) {
                    writer.writeZigZag64(entry.entityUniqueId.value());
                }
            }
            else if (entry.entryType == EntryType::FakePlayer) {
                if (entry.customName) {
                    writer.writeString(entry.customName.value());
                }
            }
        }
    }

    return writer.getString();
}
