#include "Renderer.h"

#include "Renderer/UV/UV.h"
#include "Renderer/Material/Material.h"
#include "Tools/Log/Logger.h"
#include "Scene/Entity/Components/SpriteComponent.h"
#include "Scene/Entity/Components/CameraComponent.h"
#include "Scene/Entity/Entity.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#define LOGGER_NAME "RendererLogger"
#define LAYERS_DIVISION_COEFF 10.0f // The division coefficient for normalization of the Z coordinate
#define TEXTURE_HANDLESS_COUNT_IN_CLUSTER 10

struct RendererData {
	static constexpr glm::vec2 TriangleVerticesPos[3]{
		{-1.0f, -1.0f},
		{ 1.0f,	-1.0f},
		{ 0.0f,  1.0f}
	};

	static constexpr glm::vec2 RectangleVerticesPos[4]{
		{-1.0f, -1.0f},
		{ 1.0f, -1.0f},
		{ 1.0f,  1.0f},
		{-1.0f,  1.0f}
	};
};

namespace HKCR {
	SceneManager* Renderer::m_sceneManager = nullptr;
	Scene* Renderer::m_currentScene = nullptr;

	void Renderer::init() {
		m_Shader.loadVertexShader("res\\Shaders\\basicPrimitiveVertexShader.vert");
		m_Shader.loadFragmentShader("res\\Shaders\\basicPrimitiveFragmentShader.frag");

		m_Shader.compileVertexShader();
		m_Shader.compileFragmentShader();
		m_shaderProgram = m_Shader.compileShaderProgram();

		m_ssbo.bind();
		m_ssbo.allocateMem(TEXTURE_HANDLESS_COUNT_IN_CLUSTER, 1);
		m_ssbo.unbind();
	}

	void Renderer::lastPreparation() {
		//// TODO: переместить создание проекции и вида в класс камеры, а заполнение юниформы в рендерер (init)
		//glm::mat4 proj = glm::ortho(-1280.0f / 2.0f, 1280.0f / 2.0f, -720.0f / 2.0f, 720.0f / 2.0f);
		//glm::mat4 view = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 0.0f });

		m_currentScene = m_sceneManager->getCurrentScene();

		auto currentCamera = m_currentScene->getCurrentCamera();
	}

	// Triangle
	void Renderer::createTriangle(const entityID entityID, const uint8_t layerIndx, const glm::vec2& pos, const glm::vec2& scale, const HK::UV& uv, const float rot, const HK::Material& material) {
		const glm::mat3 transform = HK::MatTransform::translate(pos) * HK::MatTransform::rot(rot) * HK::MatTransform::scale(scale);
		createTriangle(entityID, layerIndx, transform, material, uv);
	}

	void Renderer::createTriangle(const entityID entityID, const uint8_t layerIndx, const glm::mat3& transform, const HK::Material& material, const HK::UV& uv) {
		if (m_objectsData.find(entityID) == m_objectsData.end()) {
			const std::vector<float> verticesVec = vertexDataFillVec(entityID, 3, transform, uv, RendererData::TriangleVerticesPos, material, layerIndx);
			const std::vector<unsigned int> indicesVec = addTriangleIndices();


			m_objectsData.insert({ entityID, {{verticesVec, indicesVec}, material} }).first->second.first;

			// Adding the texture handle to the queue to be sent to the GPU
			if (const auto handle = material.texture.m_handle; handle != 0)
				m_textureHandlesToGPU.push_back(handle);

			// Old layer-system
			/*auto& layerData = m_objectsData.insert({ entityID, {{verticesVec, indicesVec}, material} }).first->second.first;

			if (m_objectLayers.size() < layerIndx + 1)
				m_objectLayers.resize(layerIndx + 1);

			m_objectLayers[layerIndx].push_back(&layerData); */
		}
	}


	// Rectangle
	void Renderer::createRectangle(const entityID entityID, const uint8_t layerIndx, const glm::vec2& pos, const glm::vec2& scale, const HK::UV& uv, const float rot, const HK::Material& material) {
		const glm::mat3 transform = HK::MatTransform::translate(pos) * HK::MatTransform::rot(rot) * HK::MatTransform::scale(scale);
		createRectangle(entityID, layerIndx, transform, material, uv);
	}

	void Renderer::createRectangle(const entityID entityID, const uint8_t layerIndx, const glm::mat3& transform, const HK::Material& material, const HK::UV& uv) {
		if (m_objectsData.find(entityID) == m_objectsData.end()) {
			const std::vector<float> verticesVec = vertexDataFillVec(entityID, 4, transform, uv, RendererData::RectangleVerticesPos, material, layerIndx);
			const std::vector<unsigned int> indicesVec = addRectangleIndices();

			m_objectsData.insert({ entityID, {{verticesVec, indicesVec}, material} }).first->second.first;

			// Adding the texture handle to the queue to be sent to the GPU
			if (const auto handle = material.texture.m_handle; handle != 0)
				m_textureHandlesToGPU.push_back(handle);


			// Old layer-system
			/*auto &layerData = m_objectsData.insert({ entityID, {{verticesVec, indicesVec}, material} }).first->second.first;

			if (m_objectLayers.size() < layerIndx + 1)
				m_objectLayers.resize(layerIndx + 1);

			m_objectLayers[layerIndx].push_back(&layerData);*/
		}
	}

	// TODO: Optimize it
	const bool Renderer::removeObject(const entityID entityID) {
		auto& it = m_objectsData.find(entityID);
		if (it == m_objectsData.end())
			return false;

		// Removing a texture
		{
			auto& currentTexture = it->second.second.texture;
			if (m_uniqueTextureIDs.find(currentTexture.textureID) != m_uniqueTextureIDs.end()) {
				m_uniqueTextureIDs.erase(currentTexture.textureID);
				m_firstTextureIDDupcliat.erase(currentTexture.textureID);

				currentTexture.deleteTexture();
			}
		}

		m_lastIndexOfObjects = (int)it->second.first.second.front() - 1;
		++it;

		// Removing the itself entity from map
		m_objectsData.erase(entityID);

		// After deleting an object, the indexes following it will have an incorrect value.
		// Therefore, we recalculate the indexes of all objects following the deleted one
		for (it; it != m_objectsData.end(); ++it) {
			if (it->second.first.second.size() == 6)
				it->second.first.second = addRectangleIndices();
			else
				it->second.first.second = addTriangleIndices();
		}

		return true;
	}

	void Renderer::renderAll() {
		std::vector<float> allVertices;
		std::vector<unsigned int> allIndices;

		/*for (const auto& layer : m_objectLayers) {
			for(const auto& object: layer) {
				allVertices.insert(allVertices.end(), object->first.begin(), object->first.end());
				allIndices.insert(allIndices.end(), object->second.begin(), object->second.end());
			}
		}*/

		for (const auto& object : m_objectsData) {
			allVertices.insert(allVertices.end(), object.second.first.first.begin(), object.second.first.first.end());
			allIndices.insert(allIndices.end(), object.second.first.second.begin(), object.second.first.second.end());
		}

		// Pulling the texture handles to SSBO on the GPU
		// TODO: Optimize this
		if (!m_textureHandlesToGPU.empty()) {
			m_ssbo.bind();
			m_ssbo.addData(m_textureHandlesToGPU.data(), sizeof(uint64_t) * m_textureHandlesToGPU.size());
			m_ssbo.unbind();

			m_textureHandlesToGPU.clear();
		}

		m_Shader.bind();

		const auto p = m_currentScene->getCurrentCamera()->getComponent<HK::CameraComponent>().getProjectionMatrix();
		const auto v = m_currentScene->getCurrentCamera()->getComponent<HK::CameraComponent>().getViewMatrix();
		m_Shader.setUniform("VP", v * p);

		m_vao.bind();

		m_vbo.fillBuffer(sizeof(float) * allVertices.size(), allVertices.data());
		m_ebo.fillBuffer(sizeof(unsigned int) * allIndices.size(), allIndices.data());

		m_vao.linkVBO(m_vbo.getID(), 9);
		m_vao.linkEBO(m_ebo.getID());

		m_vao.addNewAttrib(0, 3, 0);
		m_vao.addNewAttrib(1, 2, 3);
		m_vao.addNewAttrib(2, 3, 5);
		m_vao.addNewAttrib(3, 1, 8);

		glDrawElements(GL_TRIANGLES, allIndices.size(), GL_UNSIGNED_INT, nullptr);

		m_vao.unbind();
		m_Shader.unbind();
	}

	const GLuint Renderer::getPrimitivesShaderProgram() const {
		return m_shaderProgram;
	}

	const std::vector<unsigned int> Renderer::addTriangleIndices() {
		const int lastIndex = m_lastIndexOfObjects; // the last index before the update
		m_lastIndexOfObjects += 3; // 3 is the last and largest index of all objects after the creation of this object
		return {
			lastIndex + 1u,
			lastIndex + 2u,
			lastIndex + 3u
		};
	}

	const std::vector<unsigned int> Renderer::addRectangleIndices() {
		const int lastIndex = m_lastIndexOfObjects; // the last index before the update
		m_lastIndexOfObjects += 4; // 4 is the last and largest index of all objects after the creation of this object
		return {
			lastIndex + 1u,
			lastIndex + 2u,
			lastIndex + 3u,
			lastIndex + 3u,
			lastIndex + 4u,
			lastIndex + 1u
		};
	}

	// Fill the vector with vertices data
	template<typename TPArr>
	std::vector<float> Renderer::vertexDataFillVec(const entityID entityID, const unsigned int numOfIteration, const glm::mat3& transform, const HK::UV& uv, const TPArr& templatePrimitiveArray, const HK::Material material, const uint8_t layerIndx) {
		// Old Texture system
		// TODO: Docking it
		/*float currentTextureIDIndx = -1.0f;
		if (material.texture.textureID != static_cast<unsigned int>(-1)) {
			const auto& objectDataSize = m_uniqueTextureIDs.size();
			const unsigned int currentTextureID = material.texture.textureID;
			if (const auto& it = m_uniqueTextureIDs.find(currentTextureID); it == m_uniqueTextureIDs.end()) {
				glBindTextureUnit(objectDataSize, currentTextureID);

				m_uniqueTextureIDs.insert(currentTextureID);
				m_firstTextureIDDupcliat.insert({ currentTextureID, objectDataSize });
				currentTextureIDIndx = objectDataSize;
			}
			else {
				if (const auto& it2 = m_firstTextureIDDupcliat.find(currentTextureID); it2 == m_firstTextureIDDupcliat.end()) {
					currentTextureIDIndx = objectDataSize;
				}
				else {
					currentTextureIDIndx = it2->second;
				}
			}
		}*/

		const unsigned int currentTextureID = material.texture.textureID;
		static long long int currentTextureIDIndx = -1;
		if (currentTextureIDIndx > m_textureHandlesToGPU.size() - 1)
			currentTextureIDIndx = -1;

		if (currentTextureID != (unsigned int)-1)
			currentTextureIDIndx++;

		const float convertedLayerIndx = layerIndx / LAYERS_DIVISION_COEFF;
		std::vector<float> verticesVec;

		for (uint8_t i = 0; i < numOfIteration; i++) {
			const glm::vec2 vertices = glm::vec2(transform * glm::vec3(templatePrimitiveArray[i], 1.0f));
			// Vertices
			verticesVec.push_back(vertices.x);
			verticesVec.push_back(vertices.y);
			verticesVec.push_back(convertedLayerIndx); // New. Layer-system using depth buffer. DPTH TEST

			// Texture coordinates
			verticesVec.push_back(uv.uvCoords[i].x);
			verticesVec.push_back(uv.uvCoords[i].y);

			// Object color
			verticesVec.push_back(material.color.r);
			verticesVec.push_back(material.color.g);
			verticesVec.push_back(material.color.b);

			verticesVec.push_back(currentTextureID == (unsigned int)-1 ? -1 : currentTextureIDIndx);
			//verticesVec.push_back(currentTextureID);
		}

		return verticesVec;
	}
}
