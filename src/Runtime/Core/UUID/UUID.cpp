#include "UUID.h"

#include <random>

namespace HKCR {
	UUID::UUID() {
		static std::random_device randomDevice;
		static std::mt19937_64 mt(randomDevice());
		static std::uniform_int_distribution<uint64_t> uniformInDistr;

		m_uuid = std::move(uniformInDistr(mt));
	}

	UUID::UUID(const UUID&& other) {
		m_uuid = std::move(other.m_uuid);
	}

	UUID& UUID::operator=(const UUID&& other) {
		m_uuid = std::move(other.m_uuid);
		return *this;
	}

	UUID::operator uint64_t() const {
		return std::move(m_uuid);
	}
}