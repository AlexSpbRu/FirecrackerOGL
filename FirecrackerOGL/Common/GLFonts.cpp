#include "stdafx.h"

#include "Math3D.h"
#include "Shader.h"

#include "utils.h"

#include "Textures.h"
#include "OGLBuffers.h"


#include "GLFonts.h"


//////-----------------------------------------------------------------------------------------------------------

COGLESFont::COGLESFont() : m_Texture(CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Font))
{

}


GLvoid	COGLESFont::glPrintES(GLfloat X, GLfloat Y, const char *fmt, ...)
{
	char		text[512];
	va_list		ap;

	if (fmt == NULL)
		return;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	auto sz = strlen(text);
	m_vVertexBuf.reserve(m_vVertexBuf.size()+sz*6);
	m_vTextureBuf.reserve(m_vTextureBuf.size()+sz*6);
	m_vColorBuf.reserve(m_vColorBuf.size()+sz*6);
	//
	GLfloat  xpos = X, ypos = Y;
	m_Texture->Bind();
	for (auto pos = text; *pos != 0; ++pos) {
		auto chPos = *pos - 32;
		float cx = float(chPos % 16) / 16.0f;						// X Position Of Current Character
		float cy = float(chPos / 16) / 16.0f;						// Y Position Of Current Character
		m_vTextureBuf.emplace_back(cx, 1.0f - cy - 0.0625f);
		m_vVertexBuf.emplace_back(xpos, ypos - m_fSize, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);

		m_vTextureBuf.emplace_back(cx, 1.0f - cy);
		m_vVertexBuf.emplace_back(xpos, ypos, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);

		m_vTextureBuf.emplace_back(cx + 0.0625f, 1.0f - cy - 0.0625f);
		m_vVertexBuf.emplace_back(xpos + m_fSize, ypos - m_fSize, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);

		m_vTextureBuf.emplace_back(cx + 0.0625f, 1.0f - cy);
		m_vVertexBuf.emplace_back(xpos + m_fSize, ypos, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);

		m_vTextureBuf.emplace_back(cx + 0.0625f, 1.0f - cy - 0.0625f);
		m_vVertexBuf.emplace_back(xpos + m_fSize, ypos - m_fSize, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);

		m_vTextureBuf.emplace_back(cx, 1.0f - cy);
		m_vVertexBuf.emplace_back(xpos, ypos, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);
		xpos += m_fSize;
	}
}

void	COGLESFont::AddToBuffer(CTextVertexColorBuffer& Bufer)
{
	const GLfloat*	vert = GetVertexBufData();
	const GLfloat*	tex = GetTextureBufData();
	const GLfloat*	col = GetColorBufData();
	Bufer.AddVertex(vert, vert + GetVertexBufSize());
	Bufer.AddTexture(tex, tex + GetTextureBufSize());
	Bufer.AddColor(col, col + GetColorBufSize());
	m_vVertexBuf.clear();
	m_vTextureBuf.clear();
	m_vColorBuf.clear();
}

//#endif