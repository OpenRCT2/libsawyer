#pragma once

#include <cstdint>
#include <sawyer/FileSystem.hpp>
#include <string>
#include <vector>

namespace gxc
{
    struct BGRColour
    {
        uint8_t blue{};
        uint8_t green{};
        uint8_t red{};
    };

    using ColourVector = std::vector<BGRColour>;

    class SpriteArchive;

    class SpriteManifest
    {
    public:
        enum class Format
        {
            automatic,
            bmp,
            rle,
            paletteImage,
            paletteArray,
            empty
        };

        enum class PaletteKind
        {
            rct,
            keep,
        };

        struct Entry
        {
            fs::path path;
            Format format{};
            PaletteKind palette{};
            ColourVector colours{};
            int32_t offsetX{};
            int32_t offsetY{};
            int32_t zoomOffset{};
            int32_t srcX{};
            int32_t srcY{};
            int32_t srcWidth{};
            int32_t srcHeight{};
            int32_t count{};
        };

        std::vector<Entry> entries;

        static SpriteManifest fromFile(const fs::path& path);
        static std::string buildManifest(const SpriteArchive& archive);
    };
}
