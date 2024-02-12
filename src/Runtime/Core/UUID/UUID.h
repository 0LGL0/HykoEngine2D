#pragma once

#include "Hyko.h"

namespace HKCR {
	class UUID {
	public:
		UUID();
		UUID(const UUID&) = delete;
		UUID(const UUID&& other);
		UUID& operator=(const UUID&) = delete;
		UUID& operator=(const UUID&&);

		operator uint64_t() const;
	private:
		uint64_t m_uuid;
	};
}