#include "SSBManager.h"

namespace HKCR {
	SSBManager::SSBManager()
	{
		auto mainFBO = new FBO{ INITIAL_GAME_WINDOW_WIDTH, INITIAL_GAME_WINDOW_HEIGHT };
		m_allFBOs.push_back(mainFBO);
	}

	SSBManager::~SSBManager()
	{
		if (m_allFBOs.size() == 1)
			delete m_allFBOs[0];
		else
			delete[] m_allFBOs.data();
	}

	FBO* SSBManager::getNewFBO(const uint32_t width, const uint32_t height)
	{
		auto* fbo = &FBO{ width, height };
		m_allFBOs.push_back(fbo);

		return fbo;
	}

	FBO* SSBManager::getMainFBO()
	{
		return m_allFBOs[0];
	}

	const std::vector<FBO*>& SSBManager::getAllFBOs() const
	{
		return m_allFBOs;
	}
}
