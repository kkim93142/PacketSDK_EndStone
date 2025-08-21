#pragma once
#include "SDK/Packet/Packet.h"

class SetDisplayObjectivePacket final : public Packet {
public:
    explicit SetDisplayObjectivePacket();
    explicit SetDisplayObjectivePacket(const std::string_view packetData);

    ~SetDisplayObjectivePacket() {};

    std::string serialize() const override;
public:
    std::string displaySlot;
    std::string objectiveName;
    std::string displayName;
    std::string criteriaName;
    ZigZag32 sortOrder;

public:
    inline static const char* packetName = "SetDisplayObjectivePacket";
    static constexpr MinecraftPacketIds packetId = MinecraftPacketIds::SetDisplayObjective;
    static constexpr int packetIdAsInt = static_cast<int>(packetId);
};