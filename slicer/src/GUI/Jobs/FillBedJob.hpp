///|/ Copyright (c) Prusa Research 2020 - 2023 Tomáš Mészáros @tamasmeszaros, David Kocík @kocikdav
///|/
///|/ Printing3D is released under the terms of the AGPLv3 or higher
///|/
#ifndef FILLBEDJOB_HPP
#define FILLBEDJOB_HPP

#include "ArrangeJob.hpp"

namespace Slic3r {
	namespace GUI {

		class Plater;

		class FillBedJob : public Job {
				int m_object_idx = -1;

				using ArrangePolygon = arrangement::ArrangePolygon;
				using ArrangePolygons = arrangement::ArrangePolygons;

				ArrangePolygons m_selected;
				ArrangePolygons m_unselected;
				coord_t m_min_bed_inset = 0.;

				arrangement::ArrangeBed m_bed;

				int m_status_range = 0;
				Plater *m_plater;

			public:
				void prepare();
				void process(Ctl &ctl) override;

				FillBedJob();

				int status_range() const /*override*/
				{
					return m_status_range;
				}

				void finalize(bool canceled, std::exception_ptr &e) override;
		};

	}
} // namespace Slic3r::GUI

#endif // FILLBEDJOB_HPP
