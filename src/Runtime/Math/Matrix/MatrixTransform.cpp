#include "MatrixTransform.h"

class MatTransform : public HKCR::IMatTransform {
public:
	const glm::mat3 translate(const glm::vec2& pos) override {
		glm::mat3 m = glm::mat3(1.0f);
		m[2] = glm::vec3(pos, 0.0f);

		return m;
	}

	const glm::mat3 scale(const glm::vec2& scale) override {
		const glm::mat3 m = glm::mat3(
			scale.x, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f,
			0.0f, 0.0f, 1.0f
		);

		return m;
	}

	const glm::mat3 rot(const float angle) override {
		const glm::mat3 m = glm::mat3(
			cos(angle), -sin(angle), 0.0f,
			sin(angle), cos(angle), 0.0f,
			0.0f, 0.0f, 1.0f
		);

		return m;
	}

	void _deleteMatTransform() const override {
		delete this;
	}
};

DLL_EXPORT HKCR::IMatTransform* _createMatTransform() {
	return new MatTransform;
}