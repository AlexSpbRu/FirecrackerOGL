#pragma once

#include <functional>

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
	CParticleModel() {}
	virtual ~CParticleModel() {};
	virtual void InitParticle( CParticle& Particle, float Vx, float Vy, float Vz, std::function< float(float) > distributionFunction) = 0;    // задается начальное направление движения
	virtual void CaclParticle( CParticle& Particle, float Dt) = 0;
};

class CFireworkParticleModel : public CParticleModel {

	float		faiding = 1.0f;				//  faiding coefficient
	float		blinkAmp = 1.0f;			//  amplitude of blink
	float		blinkFrec = 1.0f;			//	frequency of blink
	float		resistance = 1.0f;			//  resistance coefficient
	//
	//
	float		m_Faiding = 1.0f;			//  faiding coefficient
	float		m_BlinkAmp = 1.0f;			//  amplitude of blink
	float		m_BlinkFrec = 1.0f;			//	frequency of blink
	float		m_Resistance = 1.0f;		//  resistance coefficient
public:
	CFireworkParticleModel() {}
	~CFireworkParticleModel() {}
	void InitParticle(CParticle& Particle, float Vx, float Vy, float Vz, std::function< float(float) > distributionFunction) override;    // задается начальное направление движения
	void CaclParticle(CParticle& Particle, float Dt) override;
	//  managing firework
	float		getFaiding() {
		return m_Faiding;
	}

	void		setFaiding(float Faiding) {
		m_Faiding = Faiding;
	}

	float		getBlinkAmp() {
		return m_BlinkAmp;
	}

	void		setBlinkAmp(float BlinkAmp) {
		m_BlinkAmp = BlinkAmp;
	}

	float		getBlinkFrec() {
		return m_BlinkFrec;
	}

	void		setBlinkFrec(float BlinkFrec) {
		m_BlinkFrec = BlinkFrec;
	}

	float		getResistance() {
		return m_Resistance;
	}

	void		setResistance(float Resistance) {
		m_Resistance = Resistance;
	}
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

	void InitParticle(CParticle& Particle, float Vx, float Vy, float Vz, std::function< float(float) > distributionFunction) override;
	void CaclParticle(CParticle& Particle, float Dt) override;
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
	void InitParticle(CParticle& Particle, float Vx, float Vy, float Vz, std::function< float(float) > distributionFunction) override;
	void CaclParticle(CParticle& Particle, float Dt) override;
	
};
