#pragma once
#include "SDK/Types/Math.hpp"

#include <stdexcept>

namespace BinaryReader {
    class Reader {
    private:
        std::string_view data;
        size_t position;

    public:
        explicit Reader(std::string_view data) : data(data), position(0) {}

        // 현재 위치 관리
        [[nodiscard]] size_t getPosition() const { return position; }
        [[nodiscard]] size_t size() const { return data.size(); }
        [[nodiscard]] size_t remaining() const { return data.size() - position; }
        [[nodiscard]] bool hasRemaining() const { return position < data.size(); }
        void setPosition(const size_t pos) {
            if (pos > data.size()) throw std::out_of_range("Position out of bounds");
            position = pos; 
        }
        void skip(const size_t bytes) {
            if (position + bytes > data.size()) throw std::out_of_range("Skip exceeds buffer size");
            position += bytes;
        }

        // 기본 타입 읽기 (리틀 엔디안)
        uint8_t readUInt8() {
            if (position >= data.size()) throw std::out_of_range("Buffer underflow");
            return static_cast<uint8_t>(data[position++]);
        }

        int8_t readInt8() {
            return static_cast<int8_t>(readUInt8());
        }

        uint16_t readUInt16() {
            if (position + 2 > data.size()) {
                throw std::out_of_range("Buffer underflow");
            }
            const uint16_t value = static_cast<uint16_t>(static_cast<uint8_t>(data[position])) |
                            (static_cast<uint16_t>(static_cast<uint8_t>(data[position + 1])) << 8);
            position += 2;
            return value;
        }

        int16_t readInt16() {
            return static_cast<int16_t>(readUInt16());
        }

        uint32_t readUInt32() {
            if (position + 4 > data.size()) {
                throw std::out_of_range("Buffer underflow");
            }
            const uint32_t value = static_cast<uint32_t>(static_cast<uint8_t>(data[position])) |
                            (static_cast<uint32_t>(static_cast<uint8_t>(data[position + 1])) << 8) |
                            (static_cast<uint32_t>(static_cast<uint8_t>(data[position + 2])) << 16) |
                            (static_cast<uint32_t>(static_cast<uint8_t>(data[position + 3])) << 24);
            position += 4;
            return value;
        }

        int32_t readInt32() {
            return static_cast<int32_t>(readUInt32());
        }

        uint64_t readUInt64() {
            if (position + 8 > data.size()) {
                throw std::out_of_range("Buffer underflow");
            }
            const uint64_t value = static_cast<uint64_t>(static_cast<uint8_t>(data[position])) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 1])) << 8) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 2])) << 16) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 3])) << 24) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 4])) << 32) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 5])) << 40) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 6])) << 48) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 7])) << 56);
            position += 8;
            return value;
        }

        int64_t readInt64() {
            return static_cast<int64_t>(readUInt64());
        }

        float readFloat() {
            const uint32_t bits = readUInt32();
            float value;
            std::memcpy(&value, &bits, sizeof(float));
            return value;
        }

        double readDouble() {
            const uint64_t bits = readUInt64();
            double value;
            std::memcpy(&value, &bits, sizeof(double));
            return value;
        }

        // 불린 읽기
        bool readBool() {
            return readUInt8() != 0;
        }

        // 가변 길이 정수 읽기 (Protocol Buffers 스타일)
        uint32_t readVarInt32() {
            uint32_t value = 0;
            int shift = 0;
            while (true) {
                if (shift >= 32) {
                    throw std::runtime_error("VarInt32 too large");
                }
                const uint8_t byte = readUInt8();
                value |= static_cast<uint32_t>(byte & 0x7F) << shift;
                if ((byte & 0x80) == 0) break;
                shift += 7;
            }
            return value;
        }

        uint64_t readVarInt64() {
            uint64_t value = 0;
            int shift = 0;
            while (true) {
                if (shift >= 64) {
                    throw std::runtime_error("VarInt64 too large");
                }
                const uint8_t byte = readUInt8();
                value |= static_cast<uint64_t>(byte & 0x7F) << shift;
                if ((byte & 0x80) == 0) break;
                shift += 7;
            }
            return value;
        }

        // ZigZag 인코딩 읽기
        int32_t readZigZag32() {
            const uint32_t value = readVarInt32();
            return static_cast<int32_t>((value >> 1) ^ (-(value & 1)));
        }

        int64_t readZigZag64() {
            const uint64_t value = readVarInt64();
            return static_cast<int64_t>((value >> 1) ^ (-(value & 1)));
        }

        // 문자열 읽기 (길이 prefix)
        std::string readString() {
            const uint32_t length = readVarInt32();
            if (position + length > data.size()) throw std::out_of_range("String length exceeds buffer");
            std::string result(data.substr(position, length));
            position += length;
            return result;
        }

        std::string_view readStringView() {
            const uint32_t length = readVarInt32();
            if (position + length > data.size()) {
                throw std::out_of_range("String length exceeds buffer");
            }
            const std::string_view result = data.substr(position, length);
            position += length;
            return result;
        }

        // 고정 길이 문자열 읽기
        std::string readFixedString(const size_t length) {
            if (position + length > data.size()) throw std::out_of_range("Fixed string length exceeds buffer");
            std::string result(data.substr(position, length));
            position += length;
            return result;
        }

        std::string_view readFixedStringView(const size_t length) {
            if (position + length > data.size()) {
                throw std::out_of_range("Fixed string length exceeds buffer");
            }
            const std::string_view result = data.substr(position, length);
            position += length;
            return result;
        }

        // 바이트 배열 읽기
        std::vector<uint8_t> readBytes(size_t length) {
            if (position + length > data.size()) throw std::out_of_range("Bytes length exceeds buffer");
            std::vector<uint8_t> result(length);
            for (size_t i = 0; i < length; ++i) {
                result[i] = static_cast<uint8_t>(data[position + i]);
            }
            position += length;
            return result;
        }

        // 바이트 배열 읽기 (길이 prefix)
        std::vector<uint8_t> readByteArray() {
            uint32_t length = readVarInt32();
            return readBytes(length);
        }

        // 배열 읽기 (기본 타입)
        template<typename T>
        std::vector<T> readArray() {
            uint32_t count = readVarInt32();
            std::vector<T> result;
            result.reserve(count);
            
            if constexpr (std::is_same_v<T, uint8_t>) {
                return readBytes(count);
            } else if constexpr (std::is_same_v<T, int8_t>) {
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(readInt8());
                }
            } else if constexpr (std::is_same_v<T, uint16_t>) {
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(readUInt16());
                }
            } else if constexpr (std::is_same_v<T, int16_t>) {
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(readInt16());
                }
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(readUInt32());
                }
            } else if constexpr (std::is_same_v<T, int32_t>) {
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(readInt32());
                }
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(readUInt64());
                }
            } else if constexpr (std::is_same_v<T, int64_t>) {
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(readInt64());
                }
            } else if constexpr (std::is_same_v<T, float>) {
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(readFloat());
                }
            } else if constexpr (std::is_same_v<T, double>) {
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(readDouble());
                }
            } else if constexpr (std::is_same_v<T, bool>) {
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(readBool());
                }
            } else {
                static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
                for (uint32_t i = 0; i < count; ++i) {
                    result.push_back(read<T>());
                }
            }
            return result;
        }

        // VarInt 배열 읽기
        std::vector<uint32_t> readVarInt32Array() {
            const uint32_t count = readVarInt32();
            std::vector<uint32_t> result;
            result.reserve(count);
            for (uint32_t i = 0; i < count; ++i) {
                result.push_back(readVarInt32());
            }
            return result;
        }

        std::vector<uint64_t> readVarInt64Array() {
            const uint32_t count = readVarInt32();
            std::vector<uint64_t> result;
            result.reserve(count);
            for (uint32_t i = 0; i < count; ++i) {
                result.push_back(readVarInt64());
            }
            return result;
        }

        // ZigZag 배열 읽기
        std::vector<int32_t> readZigZag32Array() {
            const uint32_t count = readVarInt32();
            std::vector<int32_t> result;
            result.reserve(count);
            for (uint32_t i = 0; i < count; ++i) {
                result.push_back(readZigZag32());
            }
            return result;
        }

        std::vector<int64_t> readZigZag64Array() {
            const uint32_t count = readVarInt32();
            std::vector<int64_t> result;
            result.reserve(count);
            for (uint32_t i = 0; i < count; ++i) {
                result.push_back(readZigZag64());
            }
            return result;
        }

        // 문자열 배열 읽기
        std::vector<std::string> readStringArray() {
            const uint32_t count = readVarInt32();
            std::vector<std::string> result;
            result.reserve(count);
            for (uint32_t i = 0; i < count; ++i) {
                result.push_back(readString());
            }
            return result;
        }

        std::vector<std::string_view> readStringViewArray() {
            const uint32_t count = readVarInt32();
            std::vector<std::string_view> result;
            result.reserve(count);
            for (uint32_t i = 0; i < count; ++i) {
                result.push_back(readStringView());
            }
            return result;
        }

        // 빅 엔디안 읽기
        uint16_t readUInt16BE() {
            if (position + 2 > data.size()) throw std::out_of_range("Buffer underflow");
            uint16_t value = (static_cast<uint16_t>(static_cast<uint8_t>(data[position])) << 8) |
                            static_cast<uint16_t>(static_cast<uint8_t>(data[position + 1]));
            position += 2;
            return value;
        }

        uint32_t readUInt32BE() {
            if (position + 4 > data.size()) throw std::out_of_range("Buffer underflow");
            uint32_t value = (static_cast<uint32_t>(static_cast<uint8_t>(data[position])) << 24) |
                            (static_cast<uint32_t>(static_cast<uint8_t>(data[position + 1])) << 16) |
                            (static_cast<uint32_t>(static_cast<uint8_t>(data[position + 2])) << 8) |
                            static_cast<uint32_t>(static_cast<uint8_t>(data[position + 3]));
            position += 4;
            return value;
        }

        uint64_t readUInt64BE() {
            if (position + 8 > data.size()) throw std::out_of_range("Buffer underflow");
            uint64_t value = (static_cast<uint64_t>(static_cast<uint8_t>(data[position])) << 56) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 1])) << 48) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 2])) << 40) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 3])) << 32) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 4])) << 24) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 5])) << 16) |
                            (static_cast<uint64_t>(static_cast<uint8_t>(data[position + 6])) << 8) |
                            static_cast<uint64_t>(static_cast<uint8_t>(data[position + 7]));
            position += 8;
            return value;
        }

        Vector2<float> readVec2Float() {
            return Vector2<float>(readFloat(), readFloat());
        }

        Vector3<int> readVec3Int() {
            return Vector3<int>(readInt32(), readInt32(), readInt32());
        }

        Vector3<VarInt> readVec3VarInt() {
            return Vector3<VarInt>(readVarInt32(), readVarInt32(), readVarInt32());
        }

        Vector3<float> readVec3Float() {
            return Vector3<float>(readFloat(), readFloat(), readFloat());
        }

        Vector3<ZigZag32> readVec3ZigZag32() {
            return Vector3<ZigZag32>(readZigZag32(), readZigZag32(), readZigZag32());
        }

        BlockCoordinates readBlockCoordinates() {
            return BlockCoordinates(
                readZigZag32(), readVarInt32(), readZigZag32()
            );
        }

        // 템플릿 기반 읽기 (POD 타입용)
        template<typename T>
        T read() {
            static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
            if (position + sizeof(T) > data.size()) throw std::out_of_range("Buffer underflow");
            T value;
            std::memcpy(&value, &data[position], sizeof(T));
            position += sizeof(T);
            return value;
        }
    };
}
