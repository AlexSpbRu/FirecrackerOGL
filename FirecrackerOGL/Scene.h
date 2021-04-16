#pragma once

#include <thread>
#include <future>

#include "Observable.h"

 


//-------------------------------------------------------------------------------------

class CPlaySound : public Singleton< CPlaySound >
{
	std::vector<  std::future<int> >	m_fuThreads;
public:
	enum class EPlayType {
		Once,
		Loop
	};
protected :
	void	play_sound(const std::string& FileName, EPlayType  Type);
public :

	int playSound(const std::string& FileName, EPlayType  Type);
	void stopPlay(int Id);
};

class CScene;


class CScene : public IKeyboardEvent, public IMouseEvent,  public IControlEvent, public IResizeWindow
{
protected :

	//	относительные размеры элементов интерфейса и игровых объектов ( относительно размеров окна / экрана	).
	const	float		m_fSmallFontConst = 0.025f;
	const	float		m_fMediumFontConst = 0.035f;
	const	float		m_fLargeFontConst = 0.07f;

	const	float		m_fControlHeightConst = 0.07f;

	const	float		m_fGameObjectSizeConst = 0.025f;
	//			конкретные размеры элементов
	float		m_fSmallFont;
	float		m_fMediumFont;
	float		m_fLargeFont;
	float		m_fControlHeight;
	float		m_fGameObjectSize;
	//
public :
	inline  float	SmallFont() { return m_fSmallFont; };
	inline  float	MediumFont() { return m_fMediumFont; };
	inline  float	LargeFont() { return m_fLargeFont; };
	inline  float	ControlHeight() { return m_fControlHeight; };
	inline  float	GameObjectSize() { return m_fGameObjectSize; };

	inline	float	SizeX(float X) { return m_fMaxWidth*X; };
	inline	float	SizeY(float Y) { return m_fMaxHeight*Y; };
protected:
	//
	std::string		m_AppPath;
	bool			m_keys[256];					// Array Used For The Keyboard Routine
	//
	std::vector<GLfloat>	m_LightAmbient = { 0.9f, 0.9f, 0.9f, 1.0f };
	std::vector<GLfloat>	m_LightDiffuse = { 1.0f, 1.0f,  1.0f, 1.0f };
	std::vector<GLfloat>	m_LightSpecular = { 1.0f, 1.0f,  1.0f, 1.0f };
	std::vector<GLfloat>	m_LightPosition = { 500.0f, 500.0f, 2000.0f, 1.0f };
	//
	size_t			m_iCountBack = 28;
	std::vector< char >	m_vUsedBack;
	size_t			GetNewBackgroundNum();
	std::vector< std::string >		m_BackNames;
	using steady_time = std::chrono::time_point<std::chrono::steady_clock>;
	steady_time		m_CurrentTime;
	//

	bool			m_bPause = false;
	//
	GLfloat			m_fMaxWidth = 1280.0f;		//  размер окна
	GLfloat			m_fMaxHeight = 1024.0f;
	//
	
	bool			m_bAp = true;										
	bool			m_bAnti = false;
	//
	size_t			m_iScore = 0;
	//
	bool			m_bMouseDown = false;
	//
	bool			m_btextureEnable;
	bool			m_balphaEnable;
	//
	CTextVertexColorBuffer	m_DrawBuffer;
	const size_t			m_iVertexSize	= 3;  //   размерности
	const size_t			m_iColorSize	= 4;
	const size_t			m_iTextureSize	= 3;
	//
	SOGLMatrix				m_ProjMatrix;
	SOGLMatrix				m_ModelMatrix;
	SOGLMatrix				m_ResultMatrix;
	//
	//CFontImage				m_FontES;
	//CFontImageManager		m_FontsManager;
	//
	Shader*					m_Shader = nullptr;
	GLuint					m_iVertShAttr = 0;		//  координата вершины
	GLuint					m_iTexCoordLoc = 0;		//	текстурная координгата
	GLuint					m_iColorLoc = 0;		//	цвет вершины
	GLuint					m_iSamplerLoc = 0;		//	номер текстуры
	GLuint					m_iMatrix = 0;			//	матрица преобразования кординат координгата
	
	//  Start speed
	float		m_fVx = 0.1f;						
	float		m_fVy = 0.1f;
	float		m_fVz = 0.0f;

 	//std::vector<CFireworkParticle>  fwp;
	CParticlesManager< CFireworkParticle, CFireworkParticleModel, DrawPointParticle >	m_ParticleManager;
	//   элементы управления

	//COGLButton				m_ControlSpeed;
	//COGLRoundProgressBar	m_ControlHourglass;
	////
	//COGLPanel				m_SplashPanel;				//  заставка
	////
	//COGLPanel				m_StartPanel;				//  стартовая панель с кнопками управления
	//COGLButton				m_StartButton;				//  элементы управления на панели
	////
	//COGLPanel				m_GamePanel;				//  фиктивная панель, на которой расположены контролы на игровом поле

	//COGLStaticText			m_GameNameContr;
	//COGLStaticText			m_ScoreContr;
	//COGLStaticText			m_StageContr;
	//COGLStaticText			m_LeveContrl;
	////
	COGLPanel				m_ControlPanel;
	//COGLStaticText			m_PanelText;				// элементы управления на панели
	//COGLButton				m_PanelButton;
	////
	//
	//COGLButton				m_btnPause;
	//

	void		InitGLES() noexcept;
		
	void		ViewPanel(bool Visible);
public:
	CScene( int Width, int Height );
	~CScene();
	bool		Init( int Width, int Height ) noexcept ;
	void		GetWindowSize(int& Width, int& Height)  noexcept;
	void		SetMaxSize( int Width, int Height )  noexcept;

	inline GLfloat		sizeX(GLfloat X) { return X*m_fMaxWidth; };
	inline GLfloat		sizeY(GLfloat Y) { return Y*m_fMaxHeight; };
	

	float		GetMaxHeight() { return m_fMaxHeight; };
	float		GetMaxWidth() { return m_fMaxWidth; };

	bool		DrawCSceneES() noexcept;

	void		ProcessKeyboard() noexcept;
	//
	void		ResizeCScene(GLsizei width, GLsizei height) noexcept ;
	
	//
	void	onKeyDown(char Key)  noexcept {
		//m_keys[Key] = true;
		//if ( Key == ' ' && ( m_ePlayState == EPlayState::Normal || m_bPause ) ) {									// If Spacebar Is Being Pressed
		//   // Pause
		//	m_bPause = !m_bPause;
		//	ViewPanel(m_bPause, m_ePlayState);
		//	if( !m_bPause )
		//		PlayStateTreatment();
		//	m_btnPause.SetPressed( m_bPause );
		//}
		//IO_TRACE("Key Down : %d\n", Key);
	}
	void	onKeyUp(char Key)  noexcept {
		m_keys[Key] = false;
		//IO_TRACE("Key Up : %d\n", Key);
	}
	//	 IMouseEvent
	void	onMouseMove(int X, int Y)  noexcept;
	void	onLMouseDown(int X, int Y)  noexcept;
	void	onLMouseUp(int X, int Y)  noexcept;
	//   IButtonEvent
	void	onButtonDown(const COGLButton* Control)  noexcept;
	void	onButtonUp(const COGLButton* Control)  noexcept;
	//   IControlEvent
	void	onLButtonDown(const COGLControl* Control)  noexcept;
	void	onLButtonUp(const COGLControl* Control)  noexcept;
	void	onMouseMove(const COGLControl* Control)  noexcept;
	void	onMouseLeave(const COGLControl* Control)  noexcept;
	//	IResizeWindow
	void	onResizeWindow(float Width, float Height)  noexcept;
};
