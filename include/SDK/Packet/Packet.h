#pragma once

#include "../Types/ZigZag.hpp"
#include "PacketIds.h"
#include "SDK/Types/VarTypes.hpp"
#include "Utils/BinaryReader.hpp"
#include "Utils/BinaryWriter.hpp"
#include <iostream>
#include <optional>
#include <vector>

class Packet {
private:
    ~Packet() = default;

public:
    [[nodiscard]] std::string getName() const {
        return this->packetName;
    }

    [[nodiscard]] virtual std::string serialize() const = 0;

protected:
    static constexpr std::string packetName = "Packet";
    static constexpr MinecraftPacketIds packetId = MinecraftPacketIds::Unknown;
    static constexpr int packetIdAsInt = static_cast<int>(packetId);
};

#include "Packets/DisconnectPacket.h"
#include "Packets/TextPacket.h"
#include "Packets/TransferPacket.h"
#include "Packets/BookEditPacket.h"
#include "Packets/SetDisplayObjectivePacket.h"
#include "Packets/SetScorePacket.h"