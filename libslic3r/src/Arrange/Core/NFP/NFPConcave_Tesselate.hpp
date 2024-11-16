///|/ Copyright (c) Prusa Research 2023 Tomáš Mészáros @tamasmeszaros
///|/
///|/ Printing3D is released under the terms of the AGPLv3 or higher
///|/
#ifndef NFPCONCAVE_TESSELATE_HPP
#define NFPCONCAVE_TESSELATE_HPP

#include <ExPolygon.hpp>

#include "Polygon.hpp"

namespace Slic3r {

	Polygons convex_decomposition_tess(const Polygon &expoly);
	Polygons convex_decomposition_tess(const ExPolygon &expoly);
	Polygons convex_decomposition_tess(const ExPolygons &expolys);
	ExPolygons nfp_concave_concave_tess(const ExPolygon &fixed, const ExPolygon &movable);

} // namespace Slic3r

#endif // NFPCONCAVE_TESSELATE_HPP
