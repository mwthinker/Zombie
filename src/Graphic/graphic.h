#ifndef VINLANDVIKINGS_VIN_GRAPHIC_H
#define VINLANDVIKINGS_VIN_GRAPHIC_H

#include <sdl/shader.h>
#include <sdl/vertex.h>
#include <sdl/sprite.h>
#include <sdl/batch.h>
#include <sdl/vertexarrayobject.h>
#include <sdl/textureview.h>

#include <array>
#include <functional>

namespace graphic {

	class Graphic {
	public:
		Graphic();

		void setMatrix(const glm::mat4& matrix);

		const glm::mat4& currentMatrix() const {
			return matrixes_.back();
		}

		void pushMatrix();
		void pushMatrix(const glm::mat4& matrix);

		void multMatrix(const glm::mat4& matrix);

		void addFilledHexagon(const glm::vec2& center, float radius, sdl::Color color, float startAngle = 0);

		void addRectangle(const glm::vec2& pos, const glm::vec2& size, sdl::Color color);

		void addRectangleImage(const glm::vec2& pos, const glm::vec2& size, const sdl::TextureView& textureView);

		void addCircle(const glm::vec2& center, float radius, sdl::Color color, const int iterations = 40, float startAngle = 0);

		void addHexagonImage(const glm::vec2& center, float radius, const sdl::TextureView& textureView, float startAngle = 0);
		
		void addHexagon(const glm::vec2& center, float innerRadius, float outerRadius, sdl::Color color, float startAngle = 0);

		void draw(const sdl::Shader& shader);

		void clearDraw();

	private:
		using Batch = sdl::Batch<sdl::Vertex>;
		using BatchView = sdl::BatchView<sdl::Vertex>;

		struct BatchData {
			BatchData() = default;
			BatchData(BatchView&& batchView, int matrixIndex);
			BatchData(BatchView&& batchView, const sdl::TextureView& texture, int matrixIndex);

			BatchView batchView;
			GLuint texture{};
			int matrixIndex{};
		};

		void bind(const sdl::Shader& shader);

		void draw(const sdl::Shader& shader, const BatchData& batchData);
		
		Batch batch_{GL_DYNAMIC_DRAW};
		BatchView lastView_;
		std::vector<BatchData> batches_;
		std::vector<glm::mat4> matrixes_;
		sdl::VertexArrayObject vao_;
		int currentMatrix_{};
		bool initiated_{};
	};

}

#endif
