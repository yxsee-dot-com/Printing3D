///|/ Copyright (c) Prusa Research 2019 - 2020 Lukáš Matěna @lukasmatena, Enrico Turri @enricoturri1966
///|/
///|/ Printing3D is released under the terms of the AGPLv3 or higher
///|/
#include "ThumbnailData.hpp"

namespace Slic3r {

void ThumbnailData::set(unsigned int w, unsigned int h)
{
    if ((w == 0) || (h == 0))
        return;

    if ((width != w) || (height != h))
    {
        width = w;
        height = h;
        // defaults to white texture
        pixels = std::vector<unsigned char>(width * height * 4, 255);
    }
}

void ThumbnailData::reset()
{
    width = 0;
    height = 0;
    pixels.clear();
}

bool ThumbnailData::is_valid() const
{
    return (width != 0) && (height != 0) && ((unsigned int)pixels.size() == 4 * width * height);
}

} // namespace Slic3r
