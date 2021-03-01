#pragma once
template < class Particle, class Model, class Draw >
class CParticlesManager
{
											
	SYSTEMTIME		m_LastDwayTime;
protected:
	Model						m_Model;
	Draw						m_Draw;
	std::vector< Particle >		m_vParticles; 
public:

	CParticlesManager() {}

	void SetCountParticles(int Count) {
		m_vParticles.resize(Count);
	}
	
	void	InitPaticles(float Vx, float Vy, float Vz) {
		for (auto& pat : m_vParticles) {
			m_Model.InitParticle(pat, Vx, Vy, Vz, [](float x) { return x; } );
		}
	}

	int DrawGLScene() {}
};

//class CParticlesManager
//{
//	const GLfloat m_Colors[12][3] =		// Rainbow Of Colors
//	{
//		{ 1.0f,0.5f,0.5f },{ 1.0f,0.75f,0.5f },{ 1.0f,1.0f,0.5f },{ 0.75f,1.0f,0.5f },
//	{ 0.5f,1.0f,0.5f },{ 0.5f,1.0f,0.75f },{ 0.5f,1.0f,1.0f },{ 0.5f,0.75f,1.0f },
//	{ 0.5f,0.5f,1.0f },{ 0.75f,0.5f,1.0f },{ 1.0f,0.5f,1.0f },{ 1.0f,0.5f,0.75f }
//	};
//	//    временные
//	bool		rainbow = true;					// Rainbow Mode?
//	bool		sp = false;						// Spacebar Pressed?
//	bool		rp = false;						// Enter Key Pressed?
//
//	float		slowdown = 2.0f;				// Slow Down Particles
//	float		xspeed = 0.0f;					// Base X Speed (To Allow Keyboard Direction Of Tail)
//	float		yspeed = 0.0f;					// Base Y Speed (To Allow Keyboard Direction Of Tail)
//	float		zspeed = 0.0f;					// Base Y Speed (To Allow Keyboard Direction Of Tail)
//	float		m_fZoom = -20.0f;				// Used To Zoom Out
//	GLuint		col = 0;						// Current Color Selection
//	GLuint		delay = 0;						// Rainbow Effect Delay
//												//
////	CGLTexture		m_Texture;
//	SYSTEMTIME		m_LastDwayTime;
//protected:
//	CParticleModel * m_Model = nullptr;
//	std::vector< CParticle >		m_vParticles;
//public:
//	CParticlesManager(size_t Count) : m_vParticles(Count) { };
//	void SetModel(CParticleModel*	Model) {
//		m_Model = Model;
//	}
//	bool CreateTexture(const std::string& ImageName);
//	//	GLvoid	ProcessKeyboard(const bool* Keys, size_t CountKeys);
//	GLvoid	InitPaticles(float Vx, float Vy, float Vz);
//	GLvoid	AddPaticles(const CParticle& Pat);
//	int DrawGLScene();
//};

