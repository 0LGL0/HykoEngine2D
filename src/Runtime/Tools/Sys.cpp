#include "Sys.h"

#include <ctime>
#include <chrono>

struct SysProps {
	static std::time_t time;
	static std::tm* tm;

	static std::string currGameDirPath;
};

std::time_t SysProps::time;
std::tm* SysProps::tm;
std::string SysProps::currGameDirPath;

void HKCR::sysInit()
{
	SysProps::time = std::time(0);
	SysProps::tm = std::localtime(&SysProps::time);
	SysProps::currGameDirPath = std::filesystem::current_path().string();
}

const std::string HKCR::getCurrentTime()
{
	return std::to_string(HKCR::getCurrentHour()) + '-' + std::to_string(HKCR::getCurrentMinute()) + '-' + std::to_string(HKCR::getCurrentSecond());;
}

const std::string HKCR::getGameDirPath() {
	return SysProps::currGameDirPath;
}

const uint8_t HKCR::getCurrentHour()
{
	return SysProps::tm->tm_hour;
}

const uint8_t HKCR::getCurrentMinute()
{
	return SysProps::tm->tm_min;
}

const uint8_t HKCR::getCurrentSecond()
{
	return SysProps::tm->tm_sec;
}

const uint16_t HKCR::getCurrentYear()
{
	return SysProps::tm->tm_year + 1900;
}

const uint8_t HKCR::getCurrentMounth()
{
	return SysProps::tm->tm_mon + 1;
}

const uint8_t HKCR::getCurrentDay()
{
	return SysProps::tm->tm_mday;
}

const std::string HKCR::getCurrentDate()
{
	return std::to_string(HKCR::getCurrentYear()) + '-' + std::to_string(HKCR::getCurrentMounth()) + '-' + std::to_string(HKCR::getCurrentDay());;
}

const std::string HKCR::getFullCurrentDate(const bool withSpace)
{
	return { (getCurrentDate() + (withSpace ? " " : "_")) + getCurrentTime() };
}
