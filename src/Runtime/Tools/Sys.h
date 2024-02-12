#pragma once

#include "Hyko.h"

namespace HKCR {
	void sysInit();

	const std::string getCurrentTime();
	const std::string getGameDirPath();
	const uint8_t getCurrentHour();
	const uint8_t getCurrentMinute();
	const uint8_t getCurrentSecond();

	const std::string getCurrentDate();
	const uint16_t getCurrentYear();
	const uint8_t getCurrentMounth();
	const uint8_t getCurrentDay();

	const std::string getFullCurrentDate(const bool withSpace);
}