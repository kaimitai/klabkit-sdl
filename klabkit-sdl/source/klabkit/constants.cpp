#include "constants.h"

std::string kkit::c::get_application_window_caption(void) {
	return std::string(c::APP_TITLE) + " (v" + std::string(c::APP_VERSION) + ")";
}
