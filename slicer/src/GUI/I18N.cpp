///|/ Copyright (c) Prusa Research 2018 Vojtěch Bubník @bubnikv
///|/
///|/ Printing3D is released under the terms of the AGPLv3 or higher
///|/
#include "I18N.hpp"

namespace Slic3r {
	namespace GUI {

		wxString L_str(const std::string &str) {
			//! Explicitly specify that the source string is already in UTF-8 encoding
			return wxGetTranslation(wxString(str.c_str(), wxConvUTF8));
		}

	}
}
