#pragma once
#include "SDK/Packet/Packet.h"
#include <optional>
#include <vector>
#include <string>

class BookEditPacket final : public Packet {
public:
    enum class EditType : uint8_t {
        ReplacePage = 0,
        AddPage = 1,
        DeletePage = 2,
        SwapPages = 3,
        Sign = 4
    };

public:
    BookEditPacket();
    explicit BookEditPacket(std::string_view packetData);

    ~BookEditPacket() {}

    EditType type;
    uint8_t slot;

    // Replace/Add page
    std::optional<uint8_t> pageNumber;
    std::optional<std::string> text;
    std::optional<std::string> photoName; // Education Edition only

    // Swap pages
    std::optional<uint8_t> page1;
    std::optional<uint8_t> page2;

    // Sign
    std::optional<std::string> title;
    std::optional<std::string> author;
    std::optional<std::string> xuid;

    [[nodiscard]] std::string serialize() const override;

public:
    inline static const char* packetName = "BookEditPacket";
    static constexpr MinecraftPacketIds packetId = MinecraftPacketIds::BookEdit;
    static constexpr int packetIdAsInt = static_cast<int>(packetId);
};
