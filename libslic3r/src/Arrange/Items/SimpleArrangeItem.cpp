///|/ Copyright (c) Prusa Research 2023 Tomáš Mészáros @tamasmeszaros
///|/
///|/ Printing3D is released under the terms of the AGPLv3 or higher
///|/
#include "SimpleArrangeItem.hpp"
#include "Arrange/ArrangeImpl.hpp" // IWYU pragma: keep
#include "Arrange/Tasks/ArrangeTaskImpl.hpp" // IWYU pragma: keep
#include "Arrange/Tasks/FillBedTaskImpl.hpp" // IWYU pragma: keep
#include "Arrange/Tasks/MultiplySelectionTaskImpl.hpp" // IWYU pragma: keep

namespace Slic3r {
	namespace arr2 {

		Polygon SimpleArrangeItem::outline() const {
			Polygon ret = shape();
			ret.rotate(m_rotation);
			ret.translate(m_translation);

			return ret;
		}

		template
		class ArrangeableToItemConverter<SimpleArrangeItem>;

		template
		struct ArrangeTask<SimpleArrangeItem>;
		template
		struct FillBedTask<SimpleArrangeItem>;
		template
		struct MultiplySelectionTask<SimpleArrangeItem>;

		template
		class Arranger<SimpleArrangeItem>;

	}
} // namespace Slic3r::arr2
