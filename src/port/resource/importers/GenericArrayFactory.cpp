#include "GenericArrayFactory.h"
#include "../type/GenericArray.h"
#include "spdlog/spdlog.h"

namespace SF64 {
std::shared_ptr<Ship::IResource> ResourceFactoryBinaryGenericArrayV0::ReadResource(std::shared_ptr<Ship::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    auto arr = std::make_shared<GenericArray>(file->InitData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    auto type = reader->ReadUInt32();

    SPDLOG_INFO("GenericArray Type Num: {}", type);

    auto count = reader->ReadUInt32();

    SPDLOG_INFO("GenericArray Count: {}", count);

    for (uint32_t i = 0; i < count; i++) {
        switch (static_cast<ArrayType>(type)) {
            case ArrayType::u8: {
                auto x = reader->ReadUByte();
                arr->mData.push_back(x);
                break;
            }
            case ArrayType::s8: {
                auto x = reader->ReadInt8();
                arr->mData.push_back(x);
                break;
            }
            case ArrayType::u16: {
                auto x = reader->ReadUInt16();
                std::copy_n(reinterpret_cast<uint8_t*>(&x), 2, std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::s16: {
                auto x = reader->ReadInt16();
                std::copy_n(reinterpret_cast<uint8_t*>(&x), 2, std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::u32: {
                auto x = reader->ReadUInt32();
                std::copy_n(reinterpret_cast<uint8_t*>(&x), 4, std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::s32: {
                auto x = reader->ReadInt32();
                std::copy_n(reinterpret_cast<uint8_t*>(&x), 4, std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::u64: {
                auto x = reader->ReadUInt64();
                std::copy_n(reinterpret_cast<uint8_t*>(&x), 8, std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::f32: {
                auto x = reader->ReadFloat();
                std::copy_n(reinterpret_cast<uint8_t*>(&x), 4, std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::f64: {
                auto x = reader->ReadDouble();
                std::copy_n(reinterpret_cast<uint8_t*>(&x), 8, std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::Vec2f: {
                Vec2f vec(reader->ReadFloat(), reader->ReadFloat());
                std::copy_n(reinterpret_cast<uint8_t*>(&vec), sizeof(Vec2f), std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::Vec3f: {
                Vec3f vec(reader->ReadFloat(), reader->ReadFloat(), reader->ReadFloat());
                std::copy_n(reinterpret_cast<uint8_t*>(&vec), sizeof(Vec3f), std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::Vec3s: {
                Vec3s vec(reader->ReadInt16(), reader->ReadInt16(), reader->ReadInt16());
                std::copy_n(reinterpret_cast<uint8_t*>(&vec), sizeof(Vec3s), std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::Vec3i: {
                Vec3i vec(reader->ReadInt32(), reader->ReadInt32(), reader->ReadInt32());
                std::copy_n(reinterpret_cast<uint8_t*>(&vec), sizeof(Vec3i), std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::Vec3iu: {
                Vec3i vec(reader->ReadUInt32(), reader->ReadUInt32(), reader->ReadUInt32());
                std::copy_n(reinterpret_cast<uint8_t*>(&vec), sizeof(Vec3iu), std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::Vec4f: {
                Vec4f vec(reader->ReadFloat(), reader->ReadFloat(), reader->ReadFloat(), reader->ReadFloat());
                std::copy_n(reinterpret_cast<uint8_t*>(&vec), sizeof(Vec4f), std::back_inserter(arr->mData));
                break;
            }
            case ArrayType::Vec4s: {
                Vec4s vec(reader->ReadInt16(), reader->ReadInt16(), reader->ReadInt16(), reader->ReadInt16());
                std::copy_n(reinterpret_cast<uint8_t*>(&vec), sizeof(Vec4s), std::back_inserter(arr->mData));
                break;
            }
            default: {
                throw std::runtime_error("UNIMPLEMENTED GENERICARRAY TYPE");
            }
        }
    }

    return arr;
}
} // namespace SF64
