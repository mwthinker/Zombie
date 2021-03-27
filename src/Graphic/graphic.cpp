#include "graphic.h"

#include <sdl/graphic.h>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/component_wise.hpp>

namespace sdlg = sdl::graphic::indexed;

namespace graphic {

	Graphic::BatchData::BatchData(BatchView&& batchView, int matrixIndex)
		: batchView{batchView}
		, matrixIndex{matrixIndex} {
	}
	
	Graphic::BatchData::BatchData(BatchView&& batchView, const sdl::TextureView& texture, int matrixIndex)
		: texture{texture}
		, batchView{batchView}
		, matrixIndex{matrixIndex} {
	}

	Graphic::Graphic() {
		matrixes_.emplace_back(1.f);
	}

	void Graphic::draw(const sdl::Shader& shader) {
		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_DEPTH_TEST);
		shader.setTextureId(1);

		int currentMatrix = currentMatrix_;
		currentMatrix_ = -1;
		if (batch_.getSize() > 0) {
			shader.useProgram();
			bind(shader);
			batch_.uploadToGraphicCard();
			shader.setMatrix(matrixes_.front());
			
			for (auto& batchData : batches_) {
				draw(shader, batchData);
			}
		}
		currentMatrix_ = currentMatrix;
	}

	void Graphic::addRectangle(const glm::vec2& pos, const glm::vec2& size, sdl::Color color) {
		batches_.emplace_back(sdlg::addRectangle(batch_, pos, size, color), currentMatrix_);
	}

	void Graphic::addRectangleImage(const glm::vec2& pos, const glm::vec2& size, const sdl::TextureView& textureView) {
		batches_.emplace_back(sdlg::addRectangleImage(batch_, pos, size, textureView), textureView, currentMatrix_);
	}

	void Graphic::addFilledHexagon(const glm::vec2& center, float radius, sdl::Color color, float startAngle) {
		addCircle(center, radius, color, 6, startAngle);
	}

	void Graphic::addHexagonImage(const glm::vec2& center, float radius, const sdl::TextureView& textureView, float startAngle) {
		batches_.emplace_back(sdlg::addHexagonImage(batch_, center, radius, textureView, startAngle), textureView, currentMatrix_);
	}

	void Graphic::addHexagon(const glm::vec2& center, float innerRadius, float outerRadius, sdl::Color color, float startAngle) {
		batches_.emplace_back(sdlg::addHexagon(batch_, center, innerRadius, outerRadius, color, startAngle), currentMatrix_);
	}

	void Graphic::addCircle(const glm::vec2& center, float radius, sdl::Color color, const int iterations, float startAngle) {
		batches_.emplace_back(sdlg::addCircle(batch_, center, radius, color, iterations, startAngle), currentMatrix_);
	}

	void Graphic::bind(const sdl::Shader& shader) {
		if (!initiated_) {
			initiated_ = true;
			vao_.generate();
			vao_.bind();
			batch_.bind();
			shader.setVertexAttribPointer();
		} else {
			vao_.bind();
		}
	}

	void Graphic::draw(const sdl::Shader& shader, const BatchData& batchData) {
		if (const auto& texture = batchData.texture; texture) {
			shader.setTextureId(1);
			glBindTexture(GL_TEXTURE_2D, texture);
		} else {
			shader.setTextureId(-1);
		}
		if (currentMatrix_ != batchData.matrixIndex) {
			currentMatrix_ = batchData.matrixIndex;
			shader.setMatrix(matrixes_[currentMatrix_]);
		}
		batch_.draw(batchData.batchView);
	}

	void Graphic::clearDraw() {
		batch_.clear();
		batches_.clear();
		matrixes_.clear();
		matrixes_.emplace_back(1.f);
		currentMatrix_ = 0;
	}

	void Graphic::pushMatrix() {
		currentMatrix_ = static_cast<int>(matrixes_.size());
		matrixes_.push_back(matrixes_.back());
	}

	void Graphic::pushMatrix(const glm::mat4& model) {
		currentMatrix_ = static_cast<int>(matrixes_.size());
		matrixes_.push_back(model);
	}

	void Graphic::multMatrix(const glm::mat4& model) {
		matrixes_.back() = matrixes_.back() * model;
	}

	void Graphic::setMatrix(const glm::mat4& model) {
		matrixes_[0] = model;
	}

}
