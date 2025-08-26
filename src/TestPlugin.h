#pragma once
#include "SDK/Packet/Packets/BookEditPacket.h"
#include "SDK/Packet/Packets/DisconnectPacket.h"
#include "SDK/Packet/Packets/TextPacket.h"
#include "SDK/Packet/Packets/TransferPacket.h"

#include <endstone/endstone.hpp>
#include <endstone/event/server/packet_receive_event.h>
#include <chrono>

class TestPlugin : public endstone::Plugin {
    void onEnable() override
    {
        getLogger().info("Successfully loaded and enabled test plugin.");
        registerEvent(&TestPlugin::onPacket, *this);
        registerEvent(&TestPlugin::onPacketSend, *this);
    }

    bool onCommand(endstone::CommandSender &sender, const endstone::Command &command, const std::vector<std::string> &args) override
    {
        // if (command.getName() == "test") {
        //     getLogger().info("command test called");
        //
        // }

        return true;
    }

    void onPacket(endstone::PacketReceiveEvent& event)
    {
        // if (event.getPacketId() == static_cast<int>(MinecraftPacketIds::Text)) {
        //     auto textPacket = TextPacket(event.getPayload());
        //     getLogger().info("TextPacket received");
        //     getLogger().info("Source Name: {}", textPacket.sourceName.value());
        //     getLogger().info("message: {}", textPacket.message);
        //     getLogger().info("xuid: {}", textPacket.xuid);
        // }
        //
        // if (event.getPacketId() == BookEditPacket::packetIdAsInt) {
        //     auto bookEditPacket = BookEditPacket(event.getPayload());
        //     getLogger().info("BookEditPacket received");
        //     getLogger().info("Action: {}", static_cast<int>(bookEditPacket.type));
        //     getLogger().info("xuid: {}", bookEditPacket.xuid.value_or("nothing"));
        //     getLogger().info("text: {}", bookEditPacket.text.value_or("nothing"));
        //     getLogger().info("slot: {}", bookEditPacket.slot);
        // }
    }

    void onPacketSend(endstone::PacketSendEvent& event) {

    }
};