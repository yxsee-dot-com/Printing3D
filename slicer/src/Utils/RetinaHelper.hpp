///|/ Copyright (c) Prusa Research 2019 - 2021 Vojtěch Bubník @bubnikv, Vojtěch Král @vojtechkral
///|/
///|/ Printing3D is released under the terms of the AGPLv3 or higher
///|/
#ifndef slic3r_RetinaHelper_hpp_
#define slic3r_RetinaHelper_hpp_

class wxWindow;

namespace Slic3r {
	namespace GUI {

		class RetinaHelper {
			public:
				RetinaHelper(wxWindow *window);
				~RetinaHelper();

				void set_use_retina(bool value);
				bool get_use_retina();
				float get_scale_factor();

			private:
#ifdef __WXGTK3__
				wxWindow* m_window;
#endif // __WXGTK3__
				void *m_self;
		};

	} // namespace GUI
} // namespace Slic3r

#endif // RetinaHelper_h
