#include "SDK/Packet/Packets/BookEditPacket.h"
#include "Utils/BinaryReader.hpp"
#include "Utils/BinaryWriter.hpp"

BookEditPacket::BookEditPacket()
{
    this->type = EditType::ReplacePage;
    this->slot = 0;
}

BookEditPacket::BookEditPacket(const std::string_view packetData)
{
    BinaryReader::Reader packetReader(packetData);
    this->type = static_cast<EditType>(packetReader.readUInt8());
    this->slot = packetReader.readUInt8();

    switch (this->type) {
        case EditType::ReplacePage:
        case EditType::AddPage:
            this->pageNumber = packetReader.readUInt8();
            this->text = packetReader.readString();
            this->photoName = packetReader.readString(); // Optional, only for Education Edition
            break;

        case EditType::DeletePage:
            this->pageNumber = packetReader.readUInt8();
            break;

        case EditType::SwapPages:
            this->page1 = packetReader.readUInt8();
            this->page2 = packetReader.readUInt8();
            break;

        case EditType::Sign:
            this->title = packetReader.readString();
            this->author = packetReader.readString();
            this->xuid = packetReader.readString();
            break;
    }
}

std::string BookEditPacket::serialize() const
{
    BinaryWriter::Writer writer;
    writer.writeUInt8(static_cast<uint8_t>(this->type));
    writer.writeUInt8(this->slot);

    switch (this->type) {
        case EditType::ReplacePage:
        case EditType::AddPage:
            if (pageNumber) writer.writeUInt8(pageNumber.value());
            if (text) writer.writeString(text.value());
            if (photoName) writer.writeString(photoName.value());
            break;

        case EditType::DeletePage:
            if (pageNumber) writer.writeUInt8(pageNumber.value());
            break;

        case EditType::SwapPages:
            if (page1) writer.writeUInt8(page1.value());
            if (page2) writer.writeUInt8(page2.value());
            break;

        case EditType::Sign:
            if (title) writer.writeString(title.value());
            if (author) writer.writeString(author.value());
            if (xuid) writer.writeString(xuid.value());
            break;
    }

    return writer.getString();
}
