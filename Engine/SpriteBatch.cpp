#include "SpriteBatch.h"

#include <algorithm>

namespace Engine {


	SpriteBatch::SpriteBatch(): _vbo(0), _vao(0){

	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init(){
		createVertexArray();

	}

	void SpriteBatch::begin(GlyphSortType sortType){
		_sortType = sortType;
	}

	void SpriteBatch::end(){
		sortGlyphs();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color){
		Glyph* newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

		newGlyph->BottomRight.color = color;
		newGlyph->BottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->BottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph);
	}

	void SpriteBatch::renderBatch(){

	}

	void SpriteBatch::createVertexArray(){

		if(_vao == 0){
			glGenVertexArrays(1, &_vao);
		}

		glBindVertexArray(_vao);

		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//This is the postion attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

		//This is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));

		//This is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

		glBindVertexArray(0);

	}

	void SpriteBatch::sortGlyphs(){
		
		switch (_sortType)
		{
		case GlyphSortType::NONE:
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
			break;
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
			break;
		default:
			break;
		}
		
	}

	bool SpriteBatch::compareFrontToBack(Glyph * a, Glyph * b){
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph * a, Glyph * b){
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph * a, Glyph * b){
		return (a->texture < b->texture);
	}

}
