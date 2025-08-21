#pragma once
#include <cstdint>
#include <string>
#include <vector>

using VarInt = int32_t;
using VarInt64 = int64_t;
using VarUInt = uint32_t;
using VarUInt64 = uint64_t;
using VarFloat = float;
using VarDouble = double;
using VarString = std::string;
using VarChar = std::string;
using VarBool = bool;
using VarIntArray = std::vector<VarInt>;
using varInt64Array = std::vector<VarInt64>;
using VarFloatArray = std::vector<VarFloat>;
using varUIntArray = std::vector< std::vector<VarUInt>>;
using varUInt64Array = std::vector< std::vector<VarUInt64>>;
using VarDoubleArray = std::vector<VarDouble>;
using VarStringArray = std::vector<VarString>;
using VarCharArray = std::vector<VarChar>;
using VarBoolArray = std::vector<VarBool>;

