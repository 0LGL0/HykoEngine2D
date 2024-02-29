#pragma once

/* =====================================================================
	Screen-Space Buffer Manager
======================================================================== */

#include "Hyko.h"
#include "FBO/FBO.h"

namespace HKCR {
	class SSBManager {
	public:
		SSBManager();
		~SSBManager();

		FBO* getNewFBO(const uint32_t width, const uint32_t height);
		FBO* getMainFBO();

		const std::vector<FBO*>& getAllFBOs() const;
	private:
		std::vector<FBO*> m_allFBOs;
	};
}