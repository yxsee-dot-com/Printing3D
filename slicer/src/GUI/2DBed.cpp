///|/ Copyright (c) Prusa Research 2018 - 2022 Oleksandra Iushchenko @YuSanka, Vojtěch Bubník @bubnikv, Lukáš Matěna @lukasmatena, Enrico Turri @enricoturri1966, Vojtěch Král @vojtechkral
///|/
///|/ ported from lib/Slic3r/GUI/2DBed.pm:
///|/ Copyright (c) Prusa Research 2016 - 2018 Vojtěch Bubník @bubnikv
///|/ Copyright (c) Slic3r 2015 Alessandro Ranellucci @alranel
///|/
///|/ Printing3D is released under the terms of the AGPLv3 or higher
///|/
#include "2DBed.hpp"
#include "GUI_App.hpp"

#include <wx/dcbuffer.h>

#include "libslic3r/src/BoundingBox.hpp"
#include "libslic3r/src/Geometry.hpp"
#include "libslic3r/src/ClipperUtils.hpp"

namespace Slic3r {
	namespace GUI {

		Bed_2D::Bed_2D(wxWindow *parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(25 * wxGetApp().em_unit(), -1), wxTAB_TRAVERSAL) {
#ifdef __APPLE__
			m_user_drawn_background = false;
#else
			SetBackgroundStyle(wxBG_STYLE_PAINT); // to avoid assert message after wxAutoBufferedPaintDC
#endif /*__APPLE__*/
		}

		void Bed_2D::repaint(const std::vector<Vec2d> &shape) {
			wxAutoBufferedPaintDC dc(this);
			auto cw = GetSize().GetWidth();
			auto ch = GetSize().GetHeight();
			// when canvas is not rendered yet, size is 0, 0
			if (cw == 0) return;

			if (m_user_drawn_background) {
				// On all systems the AutoBufferedPaintDC() achieves double buffering.
				// On MacOS the background is erased, on Windows the background is not erased
				// and on Linux / GTK the background is erased to gray color.
				// Fill DC with the background on Windows & Linux / GTK.
#ifdef _WIN32
				auto color = wxGetApp().get_highlight_default_clr();
#else
				auto color = wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT); //GetSystemColour
#endif
				dc.SetPen(wxPen(color, 1, wxPENSTYLE_SOLID));
				dc.SetBrush(wxBrush(color, wxBRUSHSTYLE_SOLID));
				auto rect = GetUpdateRegion().GetBox();
				dc.DrawRectangle(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
			}

			if (shape.empty())
				return;

			// reduce size to have some space around the drawn shape
			cw -= (2 * Border);
			ch -= (2 * Border);

			auto cbb = BoundingBoxf(Vec2d(0, 0), Vec2d(cw, ch));
			auto ccenter = cbb.center();

			// get bounding box of bed shape in G - code coordinates
			auto bed_polygon = Polygon::new_scale(shape);
			auto bb = BoundingBoxf(shape);
			bb.merge(Vec2d(0, 0));  // origin needs to be in the visible area
			auto bw = bb.size()(0);
			auto bh = bb.size()(1);
			auto bcenter = bb.center();

			// calculate the scaling factor for fitting bed shape in canvas area
			auto sfactor = std::min(cw / bw, ch / bh);
			auto shift = Vec2d(ccenter(0) - bcenter(0) * sfactor, ccenter(1) - bcenter(1) * sfactor);

			m_scale_factor = sfactor;
			m_shift = Vec2d(shift(0) + cbb.min(0), shift(1) - (cbb.max(1) - ch));

			// draw bed fill
			dc.SetBrush(wxBrush(wxColour(255, 255, 255), wxBRUSHSTYLE_SOLID));

			wxPointList pt_list;
			const size_t pt_cnt = shape.size();
			std::vector<wxPoint> points;
			points.reserve(pt_cnt);
			for (const auto &shape_pt: shape) {
				Point pt_pix = to_pixels(shape_pt, ch);
				points.emplace_back(wxPoint(pt_pix(0), pt_pix(1)));
				pt_list.Append(&points.back());
			}
			dc.DrawPolygon(&pt_list, 0, 0);

			// draw grid
			auto step = 10;  // 1cm grid
			Polylines polylines;
			for (auto x = bb.min(0) - fmod(bb.min(0), step) + step; x < bb.max(0); x += step) {
				polylines.push_back(Polyline::new_scale({Vec2d(x, bb.min(1)), Vec2d(x, bb.max(1))}));
			}
			for (auto y = bb.min(1) - fmod(bb.min(1), step) + step; y < bb.max(1); y += step) {
				polylines.push_back(Polyline::new_scale({Vec2d(bb.min(0), y), Vec2d(bb.max(0), y)}));
			}
			polylines = intersection_pl(polylines, bed_polygon);

			dc.SetPen(wxPen(wxColour(230, 230, 230), 1, wxPENSTYLE_SOLID));
			for (auto pl: polylines) {
				for (size_t i = 0; i < pl.points.size() - 1; i++) {
					Point pt1 = to_pixels(unscale(pl.points[i]), ch);
					Point pt2 = to_pixels(unscale(pl.points[i + 1]), ch);
					dc.DrawLine(pt1(0), pt1(1), pt2(0), pt2(1));
				}
			}

			// draw bed contour
			dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));
			dc.SetBrush(wxBrush(wxColour(0, 0, 0), wxBRUSHSTYLE_TRANSPARENT));
			dc.DrawPolygon(&pt_list, 0, 0);

			auto origin_px = to_pixels(Vec2d(0, 0), ch);

			// draw axes
			auto axes_len = 50;
			auto arrow_len = 6;
			auto arrow_angle = Geometry::deg2rad(45.0);
			dc.SetPen(wxPen(wxColour(255, 0, 0), 2, wxPENSTYLE_SOLID));  // red
			auto x_end = Vec2d(origin_px(0) + axes_len, origin_px(1));
			dc.DrawLine(wxPoint(origin_px(0), origin_px(1)), wxPoint(x_end(0), x_end(1)));
			for (auto angle: {-arrow_angle, arrow_angle}) {
				auto end = Eigen::Translation2d(x_end) * Eigen::Rotation2Dd(angle) * Eigen::Translation2d(-x_end) * Eigen::Vector2d(x_end(0) - arrow_len, x_end(1));
				dc.DrawLine(wxPoint(x_end(0), x_end(1)), wxPoint(end(0), end(1)));
			}

			dc.SetPen(wxPen(wxColour(0, 255, 0), 2, wxPENSTYLE_SOLID));  // green
			auto y_end = Vec2d(origin_px(0), origin_px(1) - axes_len);
			dc.DrawLine(wxPoint(origin_px(0), origin_px(1)), wxPoint(y_end(0), y_end(1)));
			for (auto angle: {-arrow_angle, arrow_angle}) {
				auto end = Eigen::Translation2d(y_end) * Eigen::Rotation2Dd(angle) * Eigen::Translation2d(-y_end) * Eigen::Vector2d(y_end(0), y_end(1) + arrow_len);
				dc.DrawLine(wxPoint(y_end(0), y_end(1)), wxPoint(end(0), end(1)));
			}

			// draw origin
			dc.SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SOLID));
			dc.SetBrush(wxBrush(wxColour(0, 0, 0), wxBRUSHSTYLE_SOLID));
			dc.DrawCircle(origin_px(0), origin_px(1), 3);

			static const auto origin_label = wxString("(0,0)");
			dc.SetTextForeground(wxColour(0, 0, 0));
			dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
			auto extent = dc.GetTextExtent(origin_label);
			const auto origin_label_x = origin_px(0) <= cw / 2 ? origin_px(0) + 1 : origin_px(0) - 1 - extent.GetWidth();
			const auto origin_label_y = origin_px(1) <= ch / 2 ? origin_px(1) + 1 : origin_px(1) - 1 - extent.GetHeight();
			dc.DrawText(origin_label, origin_label_x, origin_label_y);

			// draw current position
			if (m_pos != Vec2d(0, 0)) {
				auto pos_px = to_pixels(m_pos, ch);
				dc.SetPen(wxPen(wxColour(200, 0, 0), 2, wxPENSTYLE_SOLID));
				dc.SetBrush(wxBrush(wxColour(200, 0, 0), wxBRUSHSTYLE_TRANSPARENT));
				dc.DrawCircle(pos_px(0), pos_px(1), 5);

				dc.DrawLine(pos_px(0) - 15, pos_px(1), pos_px(0) + 15, pos_px(1));
				dc.DrawLine(pos_px(0), pos_px(1) - 15, pos_px(0), pos_px(1) + 15);
			}
		}

		// convert G - code coordinates into pixels
		Point Bed_2D::to_pixels(const Vec2d &point, int height) {
			auto p = point * m_scale_factor + m_shift;
			return Point(p(0) + Border, height - p(1) + Border);
		}

		void Bed_2D::set_pos(const Vec2d &pos) {
			m_pos = pos;
			Refresh();
		}

	} // GUI
} // Slic3r
