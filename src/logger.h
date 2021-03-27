#ifndef ZOMBIE_LOGGER_H
#define ZOMBIE_LOGGER_H

#include <spdlog/spdlog.h>

namespace zombie::logger {
	
	bool init(const std::string& folderPath = "");

}

#endif
