#pragma once
#include "SDK/Packet/Packet.h"

class TextPacket final : public Packet {
public:
    enum class Type {
        Raw,
        Chat,
        Translation,
        Popup,
        Jukebox_Popup,
        Tip,
        System,
        Whisper,
        Announcement,
        Json_Whisper,
        Json,
        Json_Announcement
    };

public:
    TextPacket();
    explicit TextPacket(std::string_view packetData);

    ~TextPacket() override {};

    Type type;
    bool needsTranslation;
    std::optional<std::string> sourceName;
    std::string message;
    std::optional<int32_t> parametersLength;
    std::optional<std::vector<std::string>> parameters;

    std::string xuid;
    std::string platformChatId;
    std::string filteredMessage;

    [[nodiscard]] std::string serialize() const override;
public:
    inline static const char* packetName = "TextPacket";
    static constexpr MinecraftPacketIds packetId = MinecraftPacketIds::Text;
    static constexpr int packetIdAsInt = static_cast<int>(packetId);
};