#pragma once
#include <string_view>
#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <cstring>
#include <type_traits>

namespace BinaryWriter {
    class Writer {
    private:
        std::vector<uint8_t> buffer;

    public:
        Writer() = default;
        explicit Writer(const size_t reserve_size) {
            buffer.reserve(reserve_size);
        }

        // 버퍼 관리
        const std::vector<uint8_t>& getBuffer() const { return buffer; }
        std::vector<uint8_t>&& moveBuffer() { return std::move(buffer); }
        [[nodiscard]] std::string getString() const {
            return std::string(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        }
        [[nodiscard]] std::string_view getStringView() const {
            return std::string_view(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        }
        [[nodiscard]] size_t size() const { return buffer.size(); }
        void clear() { buffer.clear(); }
        void reserve(const size_t size) { buffer.reserve(size); }

        // 기본 타입 쓰기 (리틀 엔디안)
        void writeUInt8(const uint8_t value) {
            buffer.push_back(value);
        }

        void writeInt8(const int8_t value) {
            writeUInt8(static_cast<uint8_t>(value));
        }

        void writeUInt16(const uint16_t value) {
            buffer.push_back(static_cast<uint8_t>(value));
            buffer.push_back(static_cast<uint8_t>(value >> 8));
        }

        void writeInt16(const int16_t value) {
            writeUInt16(static_cast<uint16_t>(value));
        }

        void writeUInt32(const uint32_t value) {
            buffer.push_back(static_cast<uint8_t>(value));
            buffer.push_back(static_cast<uint8_t>(value >> 8));
            buffer.push_back(static_cast<uint8_t>(value >> 16));
            buffer.push_back(static_cast<uint8_t>(value >> 24));
        }

        void writeInt32(const int32_t value) {
            writeUInt32(static_cast<uint32_t>(value));
        }

        void writeUInt64(uint64_t value) {
            buffer.push_back(static_cast<uint8_t>(value));
            buffer.push_back(static_cast<uint8_t>(value >> 8));
            buffer.push_back(static_cast<uint8_t>(value >> 16));
            buffer.push_back(static_cast<uint8_t>(value >> 24));
            buffer.push_back(static_cast<uint8_t>(value >> 32));
            buffer.push_back(static_cast<uint8_t>(value >> 40));
            buffer.push_back(static_cast<uint8_t>(value >> 48));
            buffer.push_back(static_cast<uint8_t>(value >> 56));
        }

        void writeInt64(int64_t value) {
            writeUInt64(static_cast<uint64_t>(value));
        }

        void writeFloat(float value) {
            uint32_t bits;
            std::memcpy(&bits, &value, sizeof(float));
            writeUInt32(bits);
        }

        void writeDouble(double value) {
            uint64_t bits;
            std::memcpy(&bits, &value, sizeof(double));
            writeUInt64(bits);
        }

        // 불린 쓰기
        void writeBool(bool value) {
            writeUInt8(value ? 1 : 0);
        }

        // 가변 길이 정수 쓰기 (Protocol Buffers 스타일)
        void writeVarInt32(uint32_t value) {
            while (value >= 0x80) {
                buffer.push_back(static_cast<uint8_t>((value & 0x7F) | 0x80));
                value >>= 7;
            }
            buffer.push_back(static_cast<uint8_t>(value));
        }

        void writeVarInt64(uint64_t value) {
            while (value >= 0x80) {
                buffer.push_back(static_cast<uint8_t>((value & 0x7F) | 0x80));
                value >>= 7;
            }
            buffer.push_back(static_cast<uint8_t>(value));
        }

        // ZigZag 인코딩 쓰기
        void writeZigZag32(int32_t value) {
            uint32_t encoded = (static_cast<uint32_t>(value) << 1) ^
                              (static_cast<uint32_t>(value >> 31));
            writeVarInt32(encoded);
        }

        void writeZigZag64(int64_t value) {
            uint64_t encoded = (static_cast<uint64_t>(value) << 1) ^
                              (static_cast<uint64_t>(value >> 63));
            writeVarInt64(encoded);
        }

        // 문자열 쓰기 (길이 prefix)
        void writeString(std::string_view str) {
            writeVarInt32(static_cast<uint32_t>(str.size()));
            buffer.insert(buffer.end(), str.begin(), str.end());
        }

        void writeString(const std::string& str) {
            writeString(std::string_view(str));
        }

        void writeString(const char* str) {
            writeString(std::string_view(str));
        }

        // 고정 길이 문자열 쓰기
        void writeFixedString(std::string_view str, size_t length) {
            if (str.size() > length) {
                buffer.insert(buffer.end(), str.begin(), str.begin() + length);
            } else {
                buffer.insert(buffer.end(), str.begin(), str.end());
                // 나머지를 0으로 패딩
                buffer.insert(buffer.end(), length - str.size(), 0);
            }
        }

        // 바이트 배열 쓰기
        void writeBytes(const uint8_t* data, size_t length) {
            buffer.insert(buffer.end(), data, data + length);
        }

        void writeBytes(const std::vector<uint8_t>& bytes) {
            buffer.insert(buffer.end(), bytes.begin(), bytes.end());
        }

        // 바이트 배열 쓰기 (길이 prefix)
        void writeByteArray(const std::vector<uint8_t>& bytes) {
            writeVarInt32(static_cast<uint32_t>(bytes.size()));
            writeBytes(bytes);
        }

        void writeByteArray(const uint8_t* data, size_t length) {
            writeVarInt32(static_cast<uint32_t>(length));
            writeBytes(data, length);
        }

        // 배열 쓰기 (기본 타입)
        template<typename T>
        void writeArray(const std::vector<T>& array) {
            writeVarInt32(static_cast<uint32_t>(array.size()));

            if constexpr (std::is_same_v<T, uint8_t>) {
                writeBytes(array);
            } else if constexpr (std::is_same_v<T, int8_t>) {
                for (const auto& item : array) {
                    writeInt8(item);
                }
            } else if constexpr (std::is_same_v<T, uint16_t>) {
                for (const auto& item : array) {
                    writeUInt16(item);
                }
            } else if constexpr (std::is_same_v<T, int16_t>) {
                for (const auto& item : array) {
                    writeInt16(item);
                }
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                for (const auto& item : array) {
                    writeUInt32(item);
                }
            } else if constexpr (std::is_same_v<T, int32_t>) {
                for (const auto& item : array) {
                    writeInt32(item);
                }
            } else if constexpr (std::is_same_v<T, uint64_t>) {
                for (const auto& item : array) {
                    writeUInt64(item);
                }
            } else if constexpr (std::is_same_v<T, int64_t>) {
                for (const auto& item : array) {
                    writeInt64(item);
                }
            } else if constexpr (std::is_same_v<T, float>) {
                for (const auto& item : array) {
                    writeFloat(item);
                }
            } else if constexpr (std::is_same_v<T, double>) {
                for (const auto& item : array) {
                    writeDouble(item);
                }
            } else if constexpr (std::is_same_v<T, bool>) {
                for (const auto& item : array) {
                    writeBool(item);
                }
            } else {
                static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
                for (const auto& item : array) {
                    write<T>(item);
                }
            }
        }

        // C 스타일 배열 쓰기
        template<typename T>
        void writeArray(const T* array, size_t count) {
            writeVarInt32(static_cast<uint32_t>(count));
            for (size_t i = 0; i < count; ++i) {
                if constexpr (std::is_same_v<T, uint8_t>) {
                    writeUInt8(array[i]);
                } else if constexpr (std::is_same_v<T, int8_t>) {
                    writeInt8(array[i]);
                } else if constexpr (std::is_same_v<T, uint16_t>) {
                    writeUInt16(array[i]);
                } else if constexpr (std::is_same_v<T, int16_t>) {
                    writeInt16(array[i]);
                } else if constexpr (std::is_same_v<T, uint32_t>) {
                    writeUInt32(array[i]);
                } else if constexpr (std::is_same_v<T, int32_t>) {
                    writeInt32(array[i]);
                } else if constexpr (std::is_same_v<T, uint64_t>) {
                    writeUInt64(array[i]);
                } else if constexpr (std::is_same_v<T, int64_t>) {
                    writeInt64(array[i]);
                } else if constexpr (std::is_same_v<T, float>) {
                    writeFloat(array[i]);
                } else if constexpr (std::is_same_v<T, double>) {
                    writeDouble(array[i]);
                } else if constexpr (std::is_same_v<T, bool>) {
                    writeBool(array[i]);
                } else {
                    static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
                    write<T>(array[i]);
                }
            }
        }

        // VarInt 배열 쓰기
        void writeVarInt32Array(const std::vector<uint32_t>& array) {
            writeVarInt32(static_cast<uint32_t>(array.size()));
            for (uint32_t value : array) {
                writeVarInt32(value);
            }
        }

        void writeVarInt64Array(const std::vector<uint64_t>& array) {
            writeVarInt32(static_cast<uint32_t>(array.size()));
            for (uint64_t value : array) {
                writeVarInt64(value);
            }
        }

        // ZigZag 배열 쓰기
        void writeZigZag32Array(const std::vector<int32_t>& array) {
            writeVarInt32(static_cast<uint32_t>(array.size()));
            for (int32_t value : array) {
                writeZigZag32(value);
            }
        }

        void writeZigZag64Array(const std::vector<int64_t>& array) {
            writeVarInt32(static_cast<uint32_t>(array.size()));
            for (int64_t value : array) {
                writeZigZag64(value);
            }
        }

        // 문자열 배열 쓰기
        void writeStringArray(const std::vector<std::string>& array) {
            writeVarInt32(static_cast<uint32_t>(array.size()));
            for (const auto& str : array) {
                writeString(str);
            }
        }

        void writeStringArray(const std::vector<std::string_view>& array) {
            writeVarInt32(static_cast<uint32_t>(array.size()));
            for (const auto& str : array) {
                writeString(str);
            }
        }

        // 초기화 리스트로 문자열 배열 쓰기
        void writeStringArray(std::initializer_list<std::string_view> array) {
            writeVarInt32(static_cast<uint32_t>(array.size()));
            for (const auto& str : array) {
                writeString(str);
            }
        }

        // 빅 엔디안 쓰기
        void writeUInt16BE(uint16_t value) {
            buffer.push_back(static_cast<uint8_t>(value >> 8));
            buffer.push_back(static_cast<uint8_t>(value));
        }

        void writeUInt32BE(uint32_t value) {
            buffer.push_back(static_cast<uint8_t>(value >> 24));
            buffer.push_back(static_cast<uint8_t>(value >> 16));
            buffer.push_back(static_cast<uint8_t>(value >> 8));
            buffer.push_back(static_cast<uint8_t>(value));
        }

        void writeUInt64BE(uint64_t value) {
            buffer.push_back(static_cast<uint8_t>(value >> 56));
            buffer.push_back(static_cast<uint8_t>(value >> 48));
            buffer.push_back(static_cast<uint8_t>(value >> 40));
            buffer.push_back(static_cast<uint8_t>(value >> 32));
            buffer.push_back(static_cast<uint8_t>(value >> 24));
            buffer.push_back(static_cast<uint8_t>(value >> 16));
            buffer.push_back(static_cast<uint8_t>(value >> 8));
            buffer.push_back(static_cast<uint8_t>(value));
        }

        void writeVec2Float(const Vector2<float>& vec) {
            writeFloat(vec.x);
            writeFloat(vec.y);
        }

        void writeVec3Int(const Vector3<int>& vec) {
            writeInt32(vec.x);
            writeInt32(vec.y);
            writeInt32(vec.z);
        }

        void writeVec3VarInt(const Vector3<VarInt>& vec) {
            writeVarInt32(vec.x);
            writeVarInt32(vec.y);
            writeVarInt32(vec.z);
        }

        void writeVec3Float(const Vector3<float>& vec) {
            writeFloat(vec.x);
            writeFloat(vec.y);
            writeFloat(vec.z);
        }

        void writeVec3ZigZag32(const Vector3<ZigZag32>& vec) {
            writeZigZag32(vec.x);
            writeZigZag32(vec.y);
            writeZigZag32(vec.z);
        }

        void writeBlockCoordinates(const BlockCoordinates& coords) {
            writeZigZag32(coords.x);
            writeVarInt32(coords.y);
            writeZigZag32(coords.z);
        }


        // 템플릿 기반 쓰기 (POD 타입용)
        template<typename T>
        void write(const T& value) {
            static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
            const auto* ptr = reinterpret_cast<const uint8_t*>(&value);
            buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
        }

        // 문자열 전용 write
        void write(const std::string& s) {
            const auto len = static_cast<uint32_t>(s.size());
            write(len); // 길이 먼저 기록 (POD write<uint32_t> 호출됨)
            buffer.insert(buffer.end(), s.begin(), s.end());
        }

        void write(std::string_view s) {
            const auto len = static_cast<uint32_t>(s.size());
            write(len);
            buffer.insert(buffer.end(), s.begin(), s.end());
        }

        //wide string
        void write(const std::u16string& s) {
            const auto len = static_cast<uint32_t>(s.size());
            write(len);
            const auto* ptr = reinterpret_cast<const uint8_t*>(s.data());
            buffer.insert(buffer.end(), ptr, ptr + len * sizeof(char16_t));
        }
    };
}