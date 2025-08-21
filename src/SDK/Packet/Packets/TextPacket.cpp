#include "SDK/Packet/Packets/TextPacket.h"

TextPacket::TextPacket()
{
     this->type = Type::Raw;
     this->needsTranslation = false;
     this->message = "";
}

 TextPacket::TextPacket(const std::string_view packetData)
{
     BinaryReader::Reader packetReader(packetData);
     this->type = static_cast<Type>(packetReader.readUInt8());
     this->needsTranslation = packetReader.readBool();

     switch (this->type) {
         case Type::Chat:
         case Type::Whisper:
         case Type::Announcement:
             this->sourceName = packetReader.readString();
             this->message = packetReader.readString();
             break;

         case Type::Raw:
         case Type::Tip:
         case Type::System:
         case Type::Json:
         case Type::Json_Whisper:
         case Type::Json_Announcement:
             this-> message = packetReader.readString();
             break;

         case Type::Translation:
         case Type::Jukebox_Popup:
         case Type::Popup:
             this->message = packetReader.readString();
             this->parametersLength = packetReader.readVarInt32();
             this->parameters = packetReader.readStringArray();
             break;
     }

     this->xuid = packetReader.readString();
     this->platformChatId = packetReader.readString();
     this->filteredMessage = packetReader.readString();
}

std::string TextPacket::serialize() const {
    BinaryWriter::Writer writer;
    writer.writeUInt8(static_cast<uint8_t>(this->type));
    writer.writeBool(this->needsTranslation);
    if (sourceName) writer.writeString(this->sourceName.value());
    writer.writeString(this->message);
    if (parametersLength) writer.writeInt32(this->parametersLength.value());
    if (parameters) writer.writeStringArray(this->parameters.value());
    writer.writeString(this->xuid);
    writer.writeString(this->platformChatId);
    writer.writeString(this->filteredMessage);

    return writer.getString();
}
