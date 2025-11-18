#ifndef ZOMBIE_GRAPHIC_H
#define ZOMBIE_GRAPHIC_H

#include "input.h"
#include "box2ddef.h"
#include "textureview.h"

#include <sdl/batch.h>
#include <sdl/shader.h>
#include <sdl/gpu.h>
#include <sdl/gpuutil.h>

#include <SDL3/SDL_gpu.h>
#include <glm/gtx/rotate_vector.hpp>

#include <concepts>
#include <span>

namespace zombie {

	template <sdl::VertexType Vertex>
	class GraphicBuffer {
	public:
		void bindBuffers(SDL_GPURenderPass* renderPass) {
			SDL_GPUBufferBinding vertexBinding{
				.buffer = vertexBuffer_.get(),
				.offset = 0
			};
			SDL_BindGPUVertexBuffers(
				renderPass,
				0,
				&vertexBinding,
				1
			);

			SDL_GPUBufferBinding indexBinding{
				.buffer = indexBuffer_.get(),
				.offset = 0
			};
			SDL_BindGPUIndexBuffer(
				renderPass,
				&indexBinding,
				SDL_GPU_INDEXELEMENTSIZE_32BIT
			);
		}

		void uploadToGpu(SDL_GPUDevice* gpuDevice, SDL_GPUCommandBuffer* commandBuffer, sdl::Batch<Vertex>& batch) {
			auto indices_ = batch.indices();
			auto vertices_ = batch.vertices();

			SDL_GPUBuffer* indexBuffer = indexBuffer_.get(gpuDevice, SDL_GPU_BUFFERUSAGE_INDEX, indices_);
			SDL_GPUBuffer* vertexBuffer = vertexBuffer_.get(gpuDevice, SDL_GPU_BUFFERUSAGE_VERTEX, vertices_);

			SDL_GPUTransferBuffer* vertexTransferBuffer = vertexTransferBuffer_.get(gpuDevice, vertices_);
			SDL_GPUTransferBuffer* indexTransferBuffer = indexTransferBuffer_.get(gpuDevice, indices_);

			sdl::gpuCopyPass(commandBuffer, [&](SDL_GPUCopyPass* copyPass) {
				SDL_GPUTransferBufferLocation vertexLocation{
					.transfer_buffer = vertexTransferBuffer,
					.offset = 0
				};

				SDL_GPUBufferRegion vertexRegion{
					.buffer = vertexBuffer,
					.offset = 0,
					.size = static_cast<Uint32>(vertices_.size() * sizeof(sdl::Vertex))
				};

				SDL_UploadToGPUBuffer(
					copyPass,
					&vertexLocation,
					&vertexRegion,
					false);

				SDL_GPUTransferBufferLocation indexLocation{
					.transfer_buffer = indexTransferBuffer,
					.offset = 0
				};

				SDL_GPUBufferRegion indexRegion{
					.buffer = indexBuffer,
					.offset = 0,
					.size = static_cast<Uint32>(indices_.size() * sizeof(uint32_t))
				};

				SDL_UploadToGPUBuffer(copyPass,
					&indexLocation,
					&indexRegion,
					false);
			});
		}

		void draw(SDL_GPURenderPass* renderPass, sdl::Batch<Vertex>& batch) {
			SDL_DrawGPUIndexedPrimitives(
				renderPass,
				static_cast<Uint32>(batch.indices().size()),
				1,
				0,
				0,
				0
			);
		}

		sdl::TransferBuffer vertexTransferBuffer_;
		sdl::TransferBuffer indexTransferBuffer_;
		sdl::Buffer vertexBuffer_;
		sdl::Buffer indexBuffer_;
	};

	class Graphic {
	public:
		static constexpr glm::vec2 NoTexture{-1.0f, -1.0f};

		const glm::mat4& getMatrix() const {
			return matrix_;
		}

		void setMatrix(const glm::mat4& mat) {
			matrix_ = mat;
		}

		void addRectangle(const glm::vec2& pos, const glm::vec2& size, Color color) {
			glm::vec3 pos3{pos, 0.0f};

			batch_.startBatch();
			batch_.insert({
				sdl::Vertex{pos3, NoTexture, color},
				sdl::Vertex{{pos3.x + size.x, pos3.y, pos3.z}, NoTexture, color},
				sdl::Vertex{{pos3.x + size.x, pos3.y + size.y, pos3.z}, NoTexture, color},
				sdl::Vertex{{pos3.x, pos3.y + size.y, pos3.z}, NoTexture, color}
			});

			batch_.insertIndices({
				0, 1, 2,
				2, 3, 0 
			});
		}

		void addRectangleImage(const glm::vec2& pos, const glm::vec2& size, const TextureView& textureView, Color color = sdl::color::White) {
			glm::vec3 pos3{pos, 0.0f};

			auto& texPos = textureView.position;
			auto& texSize = textureView.tex;

			batch_.startBatch();
			batch_.insert({
				sdl::Vertex{{pos3.x, pos3.y, pos3.z}, {texPos.x, texPos.y}, color},
				sdl::Vertex{{pos3.x + size.x, pos3.y, pos3.z}, {texPos.x + texSize.x, texPos.y}, color},
				sdl::Vertex{{pos3.x + size.x, pos3.y + size.y, pos3.z}, {texPos.x + texSize.x, texPos.y + texSize.y}, color},
				sdl::Vertex{{pos3.x, pos3.y + size.y, pos3.z}, {texPos.x, texPos.y + texSize.y}, color}
			});

			batch_.insertIndices({
				0, 1, 2,
				2, 3, 0 
			});
		}

		void clear() {
			batch_.clear();
		}

		void addLine(const glm::vec2& p1, const glm::vec2& p2, float width, Color color) {
			auto dp = 0.5f * width * glm::rotate(glm::normalize(p2 - p1), Pi / 2);

			glm::vec3 p1_3d{p1, 0.0f};
			glm::vec3 p2_3d{p2, 0.0f};
			glm::vec3 dp_3d{dp, 0.0f};

			batch_.startBatch();
			batch_.insert({
				sdl::Vertex{p1_3d - dp_3d, NoTexture, color},
				sdl::Vertex{p2_3d - dp_3d, NoTexture, color},
				sdl::Vertex{p2_3d + dp_3d, NoTexture, color},
				sdl::Vertex{p1_3d + dp_3d, NoTexture, color}
			});

			batch_.insertIndices({
				0, 1, 2,
				2, 3, 0 
			});
		}

		void addCircle(const glm::vec2& center, float radius, Color color, unsigned int iterations = 30, float startAngle = 0) {
			batch_.startBatch();

			// Add center vertex
			batch_.insert({sdl::Vertex{glm::vec3{center, 0.0f}, NoTexture, color}});

			// Add perimeter vertices
			for (unsigned int i = 0; i <= iterations; ++i) {
				float angle = startAngle + (2.0f * Pi * i) / iterations;
				glm::vec2 pos = center + radius * glm::vec2{std::cos(angle), std::sin(angle)};
				batch_.insert({sdl::Vertex{glm::vec3{pos, 0.0f}, NoTexture, color}});
			}

			// Create triangles from center to perimeter
			for (unsigned int i = 0; i < iterations; ++i) {
				batch_.insertIndices({
					0, i + 1, i + 2
				});
			}
		}

		void addCircleOutline(const glm::vec2& center, float radius, float width, Color color, unsigned int iterations = 30, float startAngle = 0) {
			batch_.startBatch();

			float innerRadius = radius - width * 0.5f;
			float outerRadius = radius + width * 0.5f;

			// Add vertices for inner and outer circles
			for (unsigned int i = 0; i <= iterations; ++i) {
				float angle = startAngle + (2.0f * Pi * i) / iterations;
				glm::vec2 direction{std::cos(angle), std::sin(angle)};

				glm::vec2 innerPos = center + innerRadius * direction;
				glm::vec2 outerPos = center + outerRadius * direction;

				batch_.insert({
					sdl::Vertex{glm::vec3{innerPos, 0.0f}, NoTexture, color},
					sdl::Vertex{glm::vec3{outerPos, 0.0f}, NoTexture, color}
				});
			}

			// Create quad strips between inner and outer circles
			for (unsigned int i = 0; i < iterations; ++i) {
				unsigned int baseIndex = i * 2;
				batch_.insertIndices({
					baseIndex, baseIndex + 1, baseIndex + 3,
					baseIndex + 3, baseIndex + 2, baseIndex
				});
			}
		}

		void addPixelLine(std::initializer_list<glm::vec2> points, Color color) {
			addPixelLine(points.begin(), points.end(), color);
		}

		void addPixelLine(std::input_iterator auto begin, std::input_iterator auto end, Color color) {
			batch_.startBatch();

			for (auto it = begin; it != end; ++it) {
				batch_.pushBack({{*it, 0}, NoTexture, color});
			}

			const auto size = std::distance(begin, end);
			for (int i = 1; i < size; ++i) {
				batch_.pushBackIndex(i - 1);
				batch_.pushBackIndex(i);
			}
		}

		void addPolygon(std::initializer_list<glm::vec2> points, Color color) {
			addPolygon(points.begin(), points.end(), color);
		}

		void addPolygon(std::input_iterator auto begin, std::input_iterator auto end, Color color) {
			batch_.startBatch();
			for (auto it = begin; it != end; ++it) {
				batch_.pushBack({{*it, 0.f}, NoTexture, color});
			}
			const auto size = std::distance(begin, end);
			for (unsigned int i = 1; i < size - 1; ++i) {
				batch_.insertIndices({0, i, i + 1});
			}
		}

		void addPixel(const glm::vec2& point, Color color, float size = 1.f) {
			addRectangle(point - glm::vec2{size * 0.5f}, glm::vec2{size}, color);
		}

		void bindBuffers(SDL_GPURenderPass* renderPass) {
			graphicBuffer_.bindBuffers(renderPass);
		}

		void uploadToGpu(SDL_GPUDevice* gpuDevice, SDL_GPUCommandBuffer* commandBuffer) {
			graphicBuffer_.uploadToGpu(gpuDevice, commandBuffer, batch_);
		}

		void draw(SDL_GPURenderPass* renderPass) {
			graphicBuffer_.draw(renderPass, batch_);
		}

	private:
		glm::mat4 matrix_{1.0f};
		sdl::Batch<sdl::Vertex> batch_;
		GraphicBuffer<sdl::Vertex> graphicBuffer_;
	};

}

#endif
