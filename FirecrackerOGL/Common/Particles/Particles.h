#pragma once

#include "Math3D.h"

class	CParticle				//  Structure For Particle
{
public:

	bool		active = true;			// Active (Yes/No)
	//
	SOGLColor   m_Color;
	//
	Vec3		m_Pos;

	Vec3		m_Speed;
public :
	
	CParticle() = default;
	virtual ~CParticle() {};
	//

	bool getActive() {
		return active;
	}

	void setActive(bool Active) {
		active = Active;
	}

	virtual SOGLColor getColor() {
		return m_Color;
	}

	virtual CParticle& setColor(const SOGLColor& Color) {
		m_Color = Color;
		return *this;
	}

	Vec3 getPosition() {
		return m_Pos;
	}

	CParticle& setPosition(const Vec3& Pos) {
		m_Pos = Pos;
		return *this;
	}

	Vec3 getSpeed() {
		return m_Speed;
	}

	CParticle& setSpeed(const Vec3& Speed) {
		m_Speed =  Speed;
		return *this;
	}
};


class CFireworkParticle : public CParticle {
public :
	float		faiding = 0.0f;				//  faiding coefficient
	float		currentFaiding = 0.0f;		//  
	float		blinkAmp = 0.0f;			//  amplitude of blink
	float		blinkFrec = 0.0f;			//	frequency of blink
	float		blinkStart = 0.0f;			//	
	float		resistance = 0.0f;			//  resistance coefficient
	CFireworkParticle() = default;
	virtual ~CFireworkParticle() = default;

	SOGLColor getColor() override {
		return m_Color;
	}

	CFireworkParticle& setColor(const SOGLColor& Color)  override {
		m_Color = Color;
		return *this;
	}

	CFireworkParticle&	setFaiding( float Faiding) {
		faiding = Faiding;
		return *this;
	}

	CFireworkParticle&	setBlinkAmp(float BlinkAmp) {
		blinkAmp = BlinkAmp;
		return *this;
	}

	CFireworkParticle&	setBlinkFrec(float  BlinkFrec) {
		blinkFrec = BlinkFrec;
		return *this;
	}

	CFireworkParticle&	setBlinkStart(float  BlinkStart) {
		blinkStart = BlinkStart;
		return *this;
	}

	CFireworkParticle&	setResistance(float  Resistance) {
		resistance = Resistance;
		return *this;
	}
};


class	CGravityParticle : public CParticle			//  Structure For Particle
{
public:
	float		life;					// Particle Life
	float		fade;					// Fade Speed
										//
	float		xg;						// X Gravity
	float		yg;						// Y Gravity
	float		zg;						// Z Gravity
										//	
	float		m_fMass = 0.0f;
	//
	//float		m_fDrawSize = 0.5f;
	CGravityParticle() {};
	virtual ~CGravityParticle() {};
};

