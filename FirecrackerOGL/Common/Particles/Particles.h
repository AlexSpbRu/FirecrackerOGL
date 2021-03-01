#pragma once

#include "Math3D.h"

class	CParticle				//  Structure For Particle
{
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

	SOGLColor getColor() {
		return m_Color;
	}

	void setColor(const SOGLColor& Color) {
		m_Color = Color;
	}

	Vec3 getPosition() {
		return m_Pos;
	}

	void setPosition(const Vec3& Pos) {
		m_Pos = Pos;
	}

	Vec3 getSpeed() {
		return m_Speed;
	}

	void setSpeed(const Vec3& Speed) {
		m_Speed =  Speed;
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
};


class	CGravityParticle : public CParticle			//  Structure For Particle
{
public:
	float		life;					// Particle Life
	float		fade;					// Fade Speed
										//
	float		m_fx = 0.0f;			// X Position
	float		m_fy = 0.0f;			// Y Position
	float		m_fz = 0.0f;			// Z Position
										//
	float		xg;						// X Gravity
	float		yg;						// Y Gravity
	float		zg;						// Z Gravity
										//									//
	float		m_fMass = 0.0f;
	float		m_fVx = 0.0f;
	float		m_fVy = 0.0f;
	float		m_fVz = 0.0f;
	//
	//float		m_fDrawSize = 0.5f;
	CGravityParticle() {};
	virtual ~CGravityParticle() {};
};

