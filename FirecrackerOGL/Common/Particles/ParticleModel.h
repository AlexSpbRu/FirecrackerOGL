#pragma once

struct CMass
{
	float		m_fX = 0.0f;
	float		m_fY = 0.0f;
	float		m_fZ = 0.0f;
	float		m_fMass = 1.0f;
	CMass() = default;
	CMass(float X, float Y, float Z, float Mass) : m_fX(X), m_fY(Y), m_fZ(Z), m_fMass(Mass) {};
};


class CParticleModel
{
public:
	CParticleModel() {};
	virtual void InitParticle( CParticle& Particle, float Vx, float Vy, float Vz) = 0;    // задается начальное направление движения
	virtual void CaclParticle( CParticle& Particle, double Dt) = 0;
};

class CModelGravitation : public CParticleModel
{
	float		m_fXspeed = 0.0f;					
	float		m_fYspeed = 0.0f;					
	float		m_fZspeed = 0.0f;					
	float						m_GravityConst = 1.0f;
	size_t						m_CountOfMass = 0;
	std::vector< CMass >		m_vMasses;
	bool						m_bADD = false;		// Increase m_GravityConst
	bool						m_bSUBTRACT = false;
public:
	CModelGravitation() {};
	virtual void AddMass(float X, float Y, float Z, float Mass) {
		m_vMasses.emplace_back(CMass(X, Y, Z, Mass));
	}

	void InitParticle(CParticle& Particle, float Vx, float Vy, float Vz) override;
	void CaclParticle(CParticle& Particle, double Dt) override;
};

class CModelStream : public CParticleModel
{
	const GLfloat m_Colors[12][3] =		// Rainbow Of Colors
	{
		{ 1.0f,0.5f,0.5f },{ 1.0f,0.75f,0.5f },{ 1.0f,1.0f,0.5f },{ 0.75f,1.0f,0.5f },
	{ 0.5f,1.0f,0.5f },{ 0.5f,1.0f,0.75f },{ 0.5f,1.0f,1.0f },{ 0.5f,0.75f,1.0f },
	{ 0.5f,0.5f,1.0f },{ 0.75f,0.5f,1.0f },{ 1.0f,0.5f,1.0f },{ 1.0f,0.5f,0.75f }
	};
	float		m_fXspeed = 0.0f;
	float		m_fYspeed = 0.0f;
	float		m_fZspeed = 0.0f;

	bool		m_bRainbow = true;					// Rainbow Mode?
	
	float		m_fSlowdown = 2.0f;					// Slow Down Particles
													//float		m_fZoom = -40.0f;					// Used To Zoom Out
	GLuint		m_iCol = 0;							// Current Color Selection
	GLuint		m_IDelay = 0;						// Rainbow Effect Delay
													
	
public:
	CModelStream() {};
	virtual void AddMass(float X, float Y, float Z, float Mass) {};
	void InitParticle(CParticle& Particle, float Vx, float Vy, float Vz) override;
	void CaclParticle(CParticle& Particle, double Dt) override;
	
};
