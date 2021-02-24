#include "stdafx.h"

#include <ctime>
#include <chrono>
//#include <windows.h>
#include <Mmsystem.h>
//#include <timeAPI.h>
#ifdef OPENGLES20
#pragma comment ( lib, "libGLESv2.lib" )
#endif
//#include "Math3D.h"
#include "Matrix.h"
#include "Shader.h"

#include "utils.h"
#include "OGLBuffers.h"

#include "Textures.h"
#include "FontImage.h"
//#include "Particles.h"


#include "OGLControl.h"
#include "Scene.h"


struct	 STimer		 											
{
	__int64				frequency;									// Timer Frequency
	double				resolution;									// Timer Resolution
//	unsigned long		mm_timer_start;								// Multimedia Timer Start Value
//	unsigned long		mm_timer_elapsed;							// Multimedia Timer Elapsed Time
//	bool				performance_timer;							// Using The Performance Timer?
	__int64				performance_timer_start;					// Performance Timer Start Value
	__int64				performance_timer_elapsed;					// Performance Timer Elapsed Time
};

STimer	gameTimer;

void TimerInit(void)										// Initialize Our Timer (Get It Ready)
{
	memset(&gameTimer, 0, sizeof(gameTimer));						// Clear Our Timer Structure

															// Check To See If A Performance Counter Is Available
															// If One Is Available The Timer Frequency Will Be Updated
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&gameTimer.frequency))
	{		// Get The Current Time And Store It In performance_timer_start
		QueryPerformanceCounter((LARGE_INTEGER *)&gameTimer.performance_timer_start);
		// Calculate The Timer Resolution Using The Timer Frequency
		gameTimer.resolution = (float)(((double)1.0f) / ((double)gameTimer.frequency));
		// Set The Elapsed Time To The Current Time
		gameTimer.performance_timer_elapsed = gameTimer.performance_timer_start;
	}
}

double TimerGetTime()										// Get Time In Milliseconds
{
	__int64 time;											// time Will Hold A 64 Bit Integer

	QueryPerformanceCounter((LARGE_INTEGER *)&time);	// Grab The Current Performance Time
														// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
	return ((time - gameTimer.performance_timer_start) * gameTimer.resolution)*1000.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

SFontSerial fonts[] = {
#include "alphabet_free_png.h"
#include "digits_font.h"
};

constexpr auto FontsCount = sizeof(fonts) / sizeof(SFontSerial);

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CPlaySound :: play_sound(const std::string& FileName, EPlayType  Type) {
	unsigned int soundFlag = SND_SYNC;
	if (Type == EPlayType::Loop)
		soundFlag |= SND_LOOP;
	PlaySound( FileName.c_str(), NULL, soundFlag );
}


int CPlaySound :: playSound(const std::string& FileName, EPlayType  Type) {
	//auto fut = std::async(std::launch::async, &CPlaySound::play_sound, this, FileName, Type);
	std::thread th(&CPlaySound::play_sound, this, FileName, Type);
	th.detach();
	return 0;
}

void CPlaySound::stopPlay(int Id) {
	PlaySound(NULL, NULL, 0);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////

CScene::CScene(int Width, int Height) : m_ControlPanel(CTextureManager::EType::Panel)/*m_btnPause(CTextureManager::EType::Pause, CTextureManager::EType::Start),
							m_ControlSpeed( CTextureManager::EType::Speed,  CTextureManager::EType::Speed), 
							m_PanelButton(CTextureManager::EType::PanelBtn, CTextureManager::EType::PanelBtn),
							,
							m_ScoreControl(CTextureManager::EType::Digits), m_StageControl(CTextureManager::EType::Digits), m_LevelControl(CTextureManager::EType::Digits),
							m_StartPanel(CTextureManager::EType::StartPanel), m_StartButton(CTextureManager::EType::PanelBtn, CTextureManager::EType::PanelBtn),
							m_GamePanel(CTextureManager::EType::None )*/
{
	m_fMaxHeight = Height;
	m_fMaxWidth = Width;
	//			конкретные размеры элементов
	m_fSmallFont = m_fSmallFontConst*Height;
	m_fMediumFont = m_fMediumFontConst*Height;
	m_fLargeFont = m_fLargeFontConst*Height;
	m_fControlHeight = m_fControlHeightConst*Height;
	m_fGameObjectSize = m_fGameObjectSizeConst*Height;
	
	//
	for (auto& b : m_keys)
		b = false;
	//std::time_t result = std::time(NULL);
	m_CurrentTime = std::chrono::steady_clock::now();
	srand((unsigned)time(NULL));

	TimerInit();
	//
	GetAppDirectory(m_AppPath);
	//
	WIN32_FIND_DATA	FindFileData;
	//		поиск и чтение всех каталогов
	HANDLE	hFind;
	std::string		PATH = m_AppPath + "data\\Backgrounds\\";
	std::string		file = PATH + "*.*";

	//CGLTexture		Background;
	//	поиск файлов в текущей директории
	hFind = ::FindFirstFile(file.c_str(), &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		while (hFind != INVALID_HANDLE_VALUE)
		{
			if (!(FindFileData.dwFileAttributes &  (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN)))
			{
				std::string path = FindFileData.cFileName;
//				IO_TRACE("\n Find file - %s\\%s \n", file.c_str(), path.c_str());
				m_BackNames.emplace_back(PATH +path);
			}
			if (!::FindNextFile(hFind, &FindFileData))
				break;
		}
		::FindClose(hFind);
	}
	m_iCountBack = m_BackNames.size();
	//
	m_vUsedBack.resize(m_iCountBack);
	for (auto& c : m_vUsedBack)  
		c = 0;
	//
	CFontImageManager::GetInstance().InitFonts(fonts, FontsCount);
	//
	m_ControlPanel.SetColor({ 1.0f, 1.0f, 1.0f, 0.5f }, { 1.0f, 1.0f, 1.0f, 0.5f });

	/*m_PanelText.SetFont(0);
	m_PanelText.SetText(L"Pause");
	m_PanelText.SetAlign(EHorAlign::Center, EVertAlign::No);
	m_PanelText.SetPosition(Vec3{ 0.0f, 0.25f, -2.0f }, 0.6f*m_fControlHeightConst, m_fControlHeightConst);
	m_ControlPanel.AddControl(&m_PanelText);

	m_PanelButton.SetFont(0);
	m_PanelButton.SetText(L"Continue");
	m_PanelButton.SetAlign(EHorAlign::Center, EVertAlign::No);
	m_PanelButton.SetColor({ 1.0f, 1.0f, 1.0f, 0.5f }, { 0.5f, 0.5f, 0.5f, 0.5f });
	m_PanelButton.SetPosition(Vec3{ 0.0f, 0.35f, -2.0f }, 0.5f, m_fControlHeightConst, ERotation::R0);

	m_ControlPanel.AddControl(&m_PanelButton);*/
	m_ControlPanel.SetVisible(true);
	m_ControlPanel.SetPosition(Vec3{ 0.0f, 0.0f, -2.0f }, 0.75f, 0.15f, ERotation::R0);
	m_ControlPanel.SetAlign(EHorAlign::Center, EVertAlign::Down);
		
	GLint maxTexture;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexture);
	CEnumeratorTimer<timer_0>::PrintTimeStart();
	//
	CMouseNotifier::GetInstance().registerObserver(this);
	CKeyboardNotifier::GetInstance().registerObserver(this);
	//
	COGLControlManager::GetInstance().registerObserver(this);
	//
	CWindowResizeNotifier::GetInstance().registerObserver(this);
	//
	m_bPause = true;
}

CScene :: ~CScene()
{
}

size_t CScene::GetNewBackgroundNum()
{
	if (m_iCountBack == 0)
		return 0;
	//
	std::string strPath;
	size_t tc = GetTickCount();
	size_t num = GetTickCount() % (m_iCountBack - 1);
	size_t pos = num;
	for (; pos != m_iCountBack && m_vUsedBack[pos] != 0; ++pos);
	if (pos == m_iCountBack)
		for (pos = 0; pos != num && m_vUsedBack[pos] != 0; ++pos);
	m_vUsedBack[pos] += 1;
	num = pos;
	
	return num;

}

#ifndef OPENGL_SHADER
void	CScene::InitGL() noexcept
{
	glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);					// Black Background
	glClearDepth(1.0f);										// Depth Buffer Setup
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);					// Set Line Antialiasing
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);										// Enable Blending
															//glBlendFunc(GL_ONE, GL_ONE);
															//glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// Type Of Blending To Use
															//
	m_LightAmbient = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_LightDiffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_LightSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_LightPosition = { 300.0f, 300.0f, 500.0f, 1.0f };
	//COGLDrawWithLight::Init();
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, m_LightAmbient.data());		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, m_LightDiffuse.data());		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION, m_LightPosition.data());		// Position The Light
	glLightfv(GL_LIGHT1, GL_SPECULAR, m_LightSpecular.data());
	glEnable(GL_LIGHT1);											// Enable Light One
																	//
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	//
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//
	CMatrix::setIdentityM(m_ProjMatrix);
}
#else
void	CScene::InitGLES() noexcept
{
	char vShaderStr[] =
		"attribute vec4 vPosition;"
		"attribute vec2 vtexCoord;"
		"attribute vec4 vColor;"
		"uniform mat4 u_ProjMatrix;"
		"uniform mat4 u_ModelMatrix;"
		"varying vec2 v_texCoord;"
		"varying vec4 v_texColor;"
		"void main()"
		"{"
		"   gl_Position = u_ProjMatrix*u_ModelMatrix*vPosition;"
		"   v_texCoord = vtexCoord;"
		"   v_texColor = vColor;"
		"}";
	
	char fShaderStr[] =
		//"precision mediump float;"
		"varying vec2 v_texCoord;"
		"varying vec4 v_texColor;"
		"vec4 v_textureColor;"
		"uniform sampler2D s_texture;"
		"uniform int b_UseTexture;"
		"void main()"
		"{"
		"   if( b_UseTexture != 0 ) { "
		"   v_textureColor = texture2D(s_texture, v_texCoord);"
		"   gl_FragColor = v_textureColor*v_texColor;"
		"   }  else "
		"       gl_FragColor = v_texColor;"
		//"   gl_FragColor = mix(v_textureColor, v_texColor, v_textureColor.a);"
		"}";
	
	GLuint vertexShader;
	GLuint fragmentShader;
	GLint linked;
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	// Load the vertex/fragment shaders
	m_Shader = new Shader();
	if( m_Shader->create_program(vShaderStr,fShaderStr) ) {
		// Store the program object
		auto	iprogramm = m_Shader->getShader();
		m_iVertShAttr = glGetAttribLocation(iprogramm, "vPosition");
		m_iTexCoordLoc = glGetAttribLocation(iprogramm, "vtexCoord");
		m_iColorLoc = glGetAttribLocation(iprogramm, "vColor");
		// Get the sampler location
		m_iSamplerLoc = glGetUniformLocation(iprogramm, "s_texture");
		//
		m_iMatrix = glGetUniformLocation(iprogramm, "u_Matrix");
	}
	m_DrawBuffer.SetShader(m_Shader);
	m_DrawBuffer.SetProjectMatrix(&m_ProjMatrix);
	//  подготовка буфера
	//m_DrawBuffer.Translate(-1.0f, -1.0f, -0.0f);
	//m_DrawBuffer.Scale( 2.0f/m_fMaxWidth, 2.0f / m_fMaxHeight, 1.0f);
//	CMatrix::setIdentityM(m_ProjMatrix);
//	CMatrix::translateM(m_ProjMatrix, -1.0f, -1.0f, -0.0f);
//	CMatrix::scaleM(m_ProjMatrix, 2.0f/m_fMaxWidth, 2.0f / m_fMaxHeight, 0.1f);
	//
	glEnable(GL_BLEND);										// Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// Type Of Blending To Use
}
#endif


void	CScene::ViewPanel(bool Visible)
{
	
}

bool	CScene::Init(int Width, int Height) noexcept
{
	if (m_iCountBack == 0)
		return false;
	void InitAPI();
	InitAPI();

	//
	//FindSuitableBackground();


	//
	//m_Line = CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Line);
	//strPath = m_AppPath + "Data\\Game\\line.png";
	//if (!m_Line.CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 1))
	//

	InitGLES();

	return true;
}


void	CScene::GetWindowSize(int& Width, int& Height) noexcept
{
	//Width = 130 + m_fGameFieldX + ( m_iHorSize -1 )*m_SellWidth;
	//Height = 80 + m_fGameFieldY + (  m_iVertSize -1 )*m_SellHeight;
}

void	CScene::SetMaxSize(int Width, int Height)  noexcept {
	m_fMaxWidth = (GLfloat)Width;
	m_fMaxHeight = (GLfloat)Height;
	/*CMatrix::setIdentityM(m_ProjMatrix);
	CMatrix::translateM(m_ProjMatrix, -1.0f, -1.0f, -0.0f);
	CMatrix::scaleM(m_ProjMatrix, 2.0f / m_fMaxWidth, 2.0f / m_fMaxHeight, 0.1f);*/
};



bool	CScene::DrawCSceneES()  noexcept
{
	//  временная задержка, регулирующая скорость игры
	float start = TimerGetTime();
	//while (TimerGetTime() < start + float(m_iSteps[m_iSpeed] * 2.0f)) {}
	//
	{
		timer_0	tt("DrawCScene");

		glEnable(GL_TEXTURE_2D);
		//glLoadIdentity();
		m_CurrentTime = std::chrono::steady_clock::now();
		//  рисование сцены
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
		//   Рисуем фон
		GLfloat vertex[] = { 10.0f, 10.0f, -4.0f,		   m_fMaxWidth - 10.0f, 10.0f, -4.0f,
			10.0f, m_fMaxHeight - 10.0f, -4.0f, m_fMaxWidth - 10.0f, m_fMaxHeight - 10.0f, -4.0f };
		GLfloat tex[] = { 0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f };
		unsigned int index[] = { 2, 1, 0, 2, 3, 1 };
		m_DrawBuffer.AddVertex(vertex, vertex + sizeof(vertex) / sizeof(GLfloat));
		m_DrawBuffer.AddTexture(tex, tex + sizeof(tex) / sizeof(GLfloat));
		m_DrawBuffer.AddIndex(index, index + sizeof(index) / sizeof(unsigned int));
		m_DrawBuffer.SetColor(SOGLColor{ 1.0f, 1.0f, 1.0f, 0.5f });
		m_DrawBuffer.DrawBuffer(CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Background), GL_TRIANGLES);

		//if ( false) {
		COGLControlManager::GetInstance().DrawControlsES(m_DrawBuffer);
		//	return true;
		//}

		/*GLfloat		fSellHeight = m_SellHeight, fSellWidth = m_SellWidth;
		GLfloat		fMaxWidth = m_fMaxWidth, fMaxHeight = m_fMaxHeight;*/

		
		m_bFilled = true;										// Set Filled To True Before Testing

		m_DrawBuffer.LoadIdentity();


		//{
		//timer_0	tt("Draw Background");
		//for (size_t ix = 0; ix < m_iHorSize; ix++) {	// Loop From Left To Right
		//	for (size_t iy = 0; iy < m_iVertSize; iy++) {// Loop From Top To Bottom

		//		if (ix < m_iHorSize - 1 && iy < m_iVertSize - 1)					// If In Bounds, Fill In Traced Boxes
		//		{
		//			// Are All Sides Of The Box Traced?
		//			if (m_vHorLine[ix][iy]() && m_vHorLine[ix][iy + 1]() && m_vVertLine[ix][iy]() && m_vVertLine[ix + 1][iy]())
		//			{

		//				m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, fMaxHeight - ( m_fGameFieldY + iy * fSellHeight), -0.5f);
		//				m_DrawBuffer.AddTexture(float(ix) / m_iHorSize, 1.0f - float(iy) / m_iVertSize);
		//				m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, fMaxHeight - (m_fGameFieldY + iy * fSellHeight), -0.5f);
		//				m_DrawBuffer.AddTexture(float(ix + 1) / m_iHorSize, 1.0f - float(iy) / m_iVertSize);
		//				m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight), -0.5f);
		//				m_DrawBuffer.AddTexture(float(ix + 1) / m_iHorSize, 1.0f - (float(iy + 1) / m_iVertSize));

		//				m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, fMaxHeight - (m_fGameFieldY + iy * fSellHeight), -0.5f);
		//				m_DrawBuffer.AddTexture(float(ix) / m_iHorSize, 1.0f - float(iy) / m_iVertSize);
		//				m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight), -0.5f);
		//				m_DrawBuffer.AddTexture(float(ix + 1) / m_iHorSize, 1.0f - (float(iy + 1) / m_iVertSize));
		//				m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight), -0.5f);
		//				m_DrawBuffer.AddTexture(float(ix) / m_iHorSize, 1.0f - float(iy + 1) / m_iVertSize);
		//				m_DrawBuffer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		//			}
		//		}
		//	}
		//}
		//m_DrawBuffer.DrawBuffer(&m_Background, GL_TRIANGLES);
		//}
		//
		//{
		//timer_0	tt("Draw Line");
		//GLfloat		texCoordHor[] = { 0.9f, 1.0f,	0.9f, 0.0f,		0.1f, 0.0f,		0.9f, 1.0f,		0.1f, 0.0f,		0.1f, 1.0f};
		//GLfloat		texCoordVert[] = {0.1f, 1.0f,   0.9f, 0.0f,		0.1f, 0.0f,		0.1f, 1.0f,		0.9f, 1.0f,		0.9f, 0.0f};
		//SOGLColor  lineColor;
		//for (size_t ix = 0; ix < m_iHorSize; ix++) {	// Loop From Left To Right
		//	for (size_t iy = 0; iy < m_iVertSize; iy++) {// Loop From Top To Bottom
		//		//m_Line->Bind();
		//	  	lineColor = { 0.0f, 0.5f, 1.0f, 0.25f };

		//		if (ix < m_vHorLine.size() && iy < m_vHorLine[ix].size()) {								// Dont Draw To Far Right

		//			if (m_vHorLine[ix][iy]() == 1)					// Has The Horizontal Line Been Traced
		//				lineColor = { 1.0f, 1.0f, 1.0f, 0.25f };
		//			else if (m_vHorLine[ix][iy]() == 0)				// If A Horizontal Line Isn't Filled
		//				m_bFilled = false;							// filled Becomes False
		//																
		//			m_DrawBuffer.AddTexture(texCoordHor, texCoordHor + sizeof(texCoordHor) / sizeof(GLfloat));
		//			m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight - 5), -0.4f);
		//			m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);
		//			m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);

		//			m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight - 5), -0.4f);
		//			m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);
		//			m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight - 5), -0.4f);

		//			m_DrawBuffer.SetColor(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
		//		}

		//		lineColor = { 0.0f, 0.5f, 1.0f, 0.25f };					// Set Line Color To Blue

		//		if (ix < m_vVertLine.size() && iy < m_vVertLine[ix].size()) {		// Dont Draw To Far Down
		//			timer_0	tt("Draw Line");
		//			if (m_vVertLine[ix][iy]() == 1)				// Has The Horizontal Line Been Traced
		//				lineColor = { 1.0f, 1.0f, 1.0f, 0.25f };	// If So, Set Line Color To White
		//			else if (m_vVertLine[ix][iy]() == 0)			// If A Verticle Line Isn't Filled
		//				m_bFilled = false;						// filled Becomes False

		//			m_DrawBuffer.AddTexture(texCoordVert, texCoordVert + sizeof(texCoordVert) / sizeof(GLfloat));
		//			m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth + 5, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);	// Top Right
		//			m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth - 5, m_fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight - 5), -0.4f);	// Bottom Left
		//			m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth - 5, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);	// Top Left

		//			m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth + 5, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);
		//			m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth + 5, m_fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight - 5), -0.4f);	// Bottom Right
		//			m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth - 5, m_fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight - 5), -0.4f);

		//			m_DrawBuffer.SetColor(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
		//		}
		//	}
		//}
		//m_DrawBuffer.DrawBuffer(m_Line, GL_TRIANGLES);
		//}
		//glDisable(GL_TEXTURE_2D);
		//		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		m_DrawBuffer.LoadIdentity();
		//
		COGLControlManager::GetInstance().DrawControlsES(m_DrawBuffer);
		//

	}
	return true;
}


GLvoid	CScene::ProcessKeyboard() noexcept
{
	//if (m_keys['A'] && !m_bAp)	{							// If 'A' Key Is Pressed And Not Held
	//
	//	m_bAp = true;										// ap Becomes TRUE
	//	m_bAnti = !m_bAnti;									// Toggle Antialiasing
	//} else
	//if (!m_keys['A'])  {								// If 'A' Key Has Been Released
	//
	//	m_bAp = false;										// ap Becomes FALSE
	//}
	//

}


void	CScene::ResizeCScene(GLsizei width, GLsizei height) noexcept
{
	if (height == 0)										// Prevent A Divide By Zero By
		height = 1;											// Making Height Equal One

	glViewport(0, 0, width, height);						// Reset The Current Viewport
#ifndef OPENGL_SHADER
	glMatrixMode(GL_PROJECTION);							// Select The Projection Matrix
	glLoadIdentity();										// Reset The Projection Matrix
	glOrtho(0.0f, width, height, 0.0f, -5.0f, 5.0f);		// Create Ortho 640x480 View (0,0 At Top Left)
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 10.0f, -2000.0f);
	glMatrixMode(GL_MODELVIEW);								// Select The Modelview Matrix
#else
	CMatrix::setIdentityM(m_ProjMatrix);
	CMatrix::orthoM( m_ProjMatrix, 0.0f, width, 0.0f, height, -5.0f, 5.0f );
	COGLControlManager::GetInstance().SetWindowSize(width, height);
#endif
	SetMaxSize(width, height);
	//
	auto sz = min(width, height);
	m_fSmallFont = m_fSmallFontConst*sz;
	m_fMediumFont = m_fMediumFontConst*sz;
	m_fLargeFont = m_fLargeFontConst*sz;
	m_fControlHeight = m_fControlHeightConst*sz;
	m_fGameObjectSize = m_fGameObjectSizeConst*sz;
	IO_TRACE("SmallFont = %f width = %f  height = %f \n", m_fSmallFont, width, height);
	//m_ControlPanel.SetPosition( Vec3{ width/2.0f - 256.0f, height / 2.0f - 256.0f, -2.0f }, Vec3{ width / 2.0f + 256.0f, height / 2.0f + 256.0f, -2.0f }, ERotation::R0 );
	//
	//FindSuitableBackground();
	//m_StartPanel.SetPosition(Vec3{ 0.0f, 0.0f, -2.0f }, Vec3{ m_fMaxWidth, m_fMaxHeight, -2.0f }, ERotation::R90);
	
}


void	CScene::onLMouseDown(int X, int Y) noexcept
{
	m_bMouseDown = true;
	
	//COGLControl* contr = COGLControlManager::GetInstance().OnLMouseDown(Point);

}

void	CScene::onLMouseUp(int X, int Y) noexcept
{
	m_bMouseDown = false;
	
}

void	CScene::onMouseMove(int X, int Y) noexcept
{
	//int dX = m_iXprev - X;
	//int dY = m_iYprev - Y;
	//if (m_bMouseDown  && ( abs(dX) >= 4 || abs(dY) >= 4 )) {
	//	m_player.m_bMouseRIGHT = false;
	//	m_player.m_bMouseLEFT = false;
	//	m_player.m_bMouseDOWN = false;
	//	m_player.m_bMouseUP = false;
	//	
	//	
	//	if (abs(dX) > abs(dY)) {  //  двигаем по горизонтали
	//		if (dX < 0)
	//			m_player.m_bMouseRIGHT = true;
	//		else
	//			m_player.m_bMouseLEFT = true;
	//	}
	//	else {
	//		if (dY < 0)
	//			m_player.m_bMouseDOWN = true;
	//		else
	//			m_player.m_bMouseUP = true;
	//	}
	//	m_iXprev = X;
	//	m_iYprev = Y;
	//}
	
}

//   IControlEvent

void	CScene::onButtonDown(const COGLButton* Control)  noexcept
{
	/*if (Control == &m_btnPause) {
		m_bPause = true;
		ViewPanel(m_bPause, m_ePlayState);
	}*/
}


void	CScene::onButtonUp(const COGLButton* Control)  noexcept
{
	/*if (Control == &m_btnPause || Control == &m_PanelButton) {
		m_bPause = false;
		ViewPanel(m_bPause, m_ePlayState);
		m_btnPause.SetPressed(m_bPause);
		PlayStateTreatment();
	}*/
	
}


void	CScene::onLButtonDown(const COGLControl* Control)  noexcept
{
	/*if (Control == &m_ControlSpeed) {
		m_iSpeed++;
		if (m_iSpeed == m_iNumSpeeds)
			m_iSpeed = 4;
		m_ControlSpeed.SetColor(m_SpeedColor[m_iSpeed], m_SpeedColor[m_iSpeed]);
		for (auto& enemy : m_vEnemy)				
			enemy.ResetObject(true);

	}*/
}


void	CScene::onLButtonUp(const COGLControl* Control)  noexcept
{
}

void	CScene::onMouseMove(const COGLControl* Control)  noexcept
{
	if (m_bMouseDown) return;
	/*CGamePlayer::m_bMouseRIGHT = false;
	CGamePlayer::m_bMouseLEFT = false;
	CGamePlayer::m_bMouseDOWN = false;
	CGamePlayer::m_bMouseUP = false;*/
	/*if (Control == &m_ControlArrowLeft)
		m_player.m_bMouseLEFT = true;
	else if (Control == &m_ControlArrowRight)
		m_player.m_bMouseRIGHT = true;
	else if (Control == &m_ControlArrowUp)
		m_player.m_bMouseUP = true;
	else if (Control == &m_ControlArrowDown)
		m_player.m_bMouseDOWN = true;*/
}

void	CScene::onMouseLeave(const COGLControl* Control)  noexcept
{
	if (m_bMouseDown) return;
	/*if (Control == &m_ControlArrowLeft)
		m_player.m_bMouseLEFT = false;
	else if (Control == &m_ControlArrowRight)
		m_player.m_bMouseRIGHT = false;
	else if (Control == &m_ControlArrowUp)
		m_player.m_bMouseUP = false;
	else if (Control == &m_ControlArrowDown)
		m_player.m_bMouseDOWN = false;*/
}

void	CScene::onResizeWindow(float Width, float Height)   noexcept
{
	ResizeCScene(Width, Height);
}