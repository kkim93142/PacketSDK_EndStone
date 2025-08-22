#pragma once

#include "SDK/Packet/Packet.h"
#include <string>
#include <vector>
#include <optional>
#include <cstdint>

class SetScorePacket final : public Packet {
public:
    enum class ActionType : uint8_t {
        Change = 0,
        Remove = 1
    };

    enum class EntryType : int8_t {
        Player = 1,
        Entity = 2,
        FakePlayer = 3
    };

    struct Entry {
        ZigZag64 scoreboardId;
        std::string objectiveName;       // string
        int32_t score;                   // li32

        // Only if action == Change
        std::optional<EntryType> entryType;
        std::optional<ZigZag64> entityUniqueId;  // player/entity
        std::optional<std::string> customName;  // string (fake player)
    };

public:
    SetScorePacket();
    explicit SetScorePacket(std::string_view packetData);

    ~SetScorePacket() override {}

    ActionType action;
    VarInt entriesLength;
    std::vector<Entry> entries;

    [[nodiscard]] std::string serialize() const override;

public:
    inline static const char* packetName = "SetScorePacket";
    static constexpr MinecraftPacketIds packetId = MinecraftPacketIds::SetScore;
    static constexpr int packetIdAsInt = static_cast<int>(packetId);
};
