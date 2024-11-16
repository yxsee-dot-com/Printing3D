///|/ Copyright (c) Prusa Research 2019 - 2021 Lukáš Hejl @hejllukas, Lukáš Matěna @lukasmatena, Enrico Turri @enricoturri1966
///|/
///|/ Printing3D is released under the terms of the AGPLv3 or higher
///|/
#ifndef slic3r_GLGizmos_hpp_
#define slic3r_GLGizmos_hpp_

// this describes events being passed from GLCanvas3D to SlaSupport gizmo
namespace Slic3r {
	namespace GUI {

		enum class SLAGizmoEventType : unsigned char {
			LeftDown = 1, LeftUp, RightDown, Dragging, Delete, SelectAll, ShiftUp, AltUp, ApplyChanges, DiscardChanges, AutomaticGeneration, ManualEditing, MouseWheelUp, MouseWheelDown, ResetClippingPlane
		};

	} // namespace GUI
} // namespace Slic3r

#include "GUI/Gizmos/GLGizmoMove.hpp"
#include "GUI/Gizmos/GLGizmoScale.hpp"
#include "GUI/Gizmos/GLGizmoRotate.hpp"
#include "GUI/Gizmos/GLGizmoFlatten.hpp"
#include "GUI/Gizmos/GLGizmoSlaSupports.hpp"
#include "GUI/Gizmos/GLGizmoFdmSupports.hpp"
#include "GUI/Gizmos/GLGizmoMmuSegmentation.hpp"
#include "GUI/Gizmos/GLGizmoCut.hpp"
#include "GUI/Gizmos/GLGizmoHollow.hpp"

#endif //slic3r_GLGizmos_hpp_
