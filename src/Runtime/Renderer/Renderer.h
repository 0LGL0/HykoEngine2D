#pragma once

#include "Hyko.h"
#include "Tools/Sys.h"
#include "Platform/ShaderTools/Shader.h"

#include "Platform/Buffers/GL/VAO/VAO.h"
#include "Platform/Buffers/GL/VBO/VBO.h"
#include "Platform/Buffers/GL/EBO/EBO.h"
#include "Platform/Buffers/GL/SSBO/SSBO.h"

#include "Scene/Entity/Components/LayerComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace HK {
	struct UV;
	struct Material;
	enum class Primitive;
}

namespace HKCR {
	class App;

	class Renderer {
		using GLuint = unsigned int;
	public:
		void init();
		void lastPreparation();

		void createTriangle(const entityID entityID, const uint8_t layerIndx, const glm::vec2& pos, const glm::vec2& scale, const HK::UV& uv, const float rot, const HK::Material& material);
		void createTriangle(const entityID entityID, const uint8_t layerIndx, const glm::mat3& transform, const HK::Material& material, const HK::UV& uv);
		void createRectangle(const entityID entityID, const uint8_t layerIndx, const glm::vec2& pos, const glm::vec2& scale, const HK::UV& uv, const float rot, const HK::Material& material);
		void createRectangle(const entityID entityID, const uint8_t layerIndx, const glm::mat3& transform, const HK::Material& material, const HK::UV& uv);

		const bool removeObject(const entityID entityID);

		//void changeMaterial(const uint32_t entityID, const glm::vec3& color);
		//void setMaterial(const uint32_t entityID, const glm::vec3& color);

		const GLuint getPrimitivesShaderProgram() const;

		void renderAll();
	private:
		template<typename TPArr>
		std::vector<float> vertexDataFillVec(const entityID entityID, const unsigned int numOfIteration, const glm::mat3& transform, const HK::UV& uv, const TPArr& templatePrimitiveArray, const HK::Material material, const uint8_t layerIndx);
		const std::vector<unsigned int> addTriangleIndices();
		const std::vector<unsigned int> addRectangleIndices();

		GLuint m_shaderProgram;

		const std::string m_gameDirPath = HKCR::getGameDirPath();

		Shader m_Shader;
		VAO<float> m_vao;
		VBO m_vbo;
		EBO m_ebo;
		SSBO<uint64_t> m_ssbo;

		static SceneManager* m_sceneManager;
		static Scene* m_currentScene;

		//				 object id
		std::unordered_map<entityID, std::pair<std::pair<std::vector<float>, std::vector<unsigned int>>, HK::Material>> m_objectsData; // objects vertices, indices and materials

		//std::vector<std::vector<std::pair<std::vector<float>, std::vector<unsigned int>>*>> m_objectLayers;

		// opengl texture id | first index of the texture id in the array
		std::unordered_map<unsigned int, unsigned int> m_firstTextureIDDupcliat;
		std::unordered_set<unsigned int> m_uniqueTextureIDs; // unsigned int is a opengl texture id
		int m_lastIndexOfObjects = -1;

		std::vector<uint64_t> m_textureHandlesToGPU;

		friend HKCR::App;
	};
}