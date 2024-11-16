///|/ Copyright (c) Prusa Research 2020 - 2022 Tomáš Mészáros @tamasmeszaros
///|/ Copyright (c) 2022 ole00 @ole00
///|/
///|/ Printing3D is released under the terms of the AGPLv3 or higher
///|/
#ifndef SLARASTER_CPP
#define SLARASTER_CPP

#include <SLA/RasterBase.hpp>
#include <SLA/AGGRaster.hpp>
// minz image write:
#include <miniz.h>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <cstdlib>

#include "agg/agg_gamma_functions.h"

namespace Slic3r {
	namespace sla {

		EncodedRaster PNGRasterEncoder::operator()(const void *ptr, size_t w, size_t h, size_t num_components) {
			std::vector<uint8_t> buf;
			size_t s = 0;

			void *rawdata = tdefl_write_image_to_png_file_in_memory(ptr, int(w), int(h), int(num_components), &s);

			// On error, data() will return an empty vector. No other info can be
			// retrieved from miniz anyway...
			if (rawdata == nullptr) return EncodedRaster({}, "png");

			auto pptr = static_cast<std::uint8_t *>(rawdata);

			buf.reserve(s);
			std::copy(pptr, pptr + s, std::back_inserter(buf));

			MZ_FREE(rawdata);
			return EncodedRaster(std::move(buf), "png");
		}

		std::ostream &operator<<(std::ostream &stream, const EncodedRaster &bytes) {
			stream.write(reinterpret_cast<const char *>(bytes.data()), std::streamsize(bytes.size()));

			return stream;
		}

		EncodedRaster PPMRasterEncoder::operator()(const void *ptr, size_t w, size_t h, size_t num_components) {
			std::vector<uint8_t> buf;

			auto header = std::string("P5 ") + std::to_string(w) + " " + std::to_string(h) + " " + "255 ";

			auto sz = w * h * num_components;
			size_t s = sz + header.size();

			buf.reserve(s);

			auto buff = reinterpret_cast<const std::uint8_t *>(ptr);
			std::copy(header.begin(), header.end(), std::back_inserter(buf));
			std::copy(buff, buff + sz, std::back_inserter(buf));

			return EncodedRaster(std::move(buf), "ppm");
		}

		std::unique_ptr<RasterBase> create_raster_grayscale_aa(const Resolution &res, const PixelDim &pxdim, double gamma, const RasterBase::Trafo &tr) {
			std::unique_ptr<RasterBase> rst;

			if (gamma > 0)
				rst = std::make_unique<RasterGrayscaleAAGammaPower>(res, pxdim, tr, gamma);
			else if (std::abs(gamma - 1.) < 1e-6)
				rst = std::make_unique<RasterGrayscaleAA>(res, pxdim, tr, agg::gamma_none());
			else
				rst = std::make_unique<RasterGrayscaleAA>(res, pxdim, tr, agg::gamma_threshold(.5));

			return rst;
		}

	} // namespace sla
} // namespace Slic3r

#endif // SLARASTER_CPP