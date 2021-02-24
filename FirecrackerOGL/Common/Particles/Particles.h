#pragma once

#include "Math3D.h"

class	CParticle				//  Structure For Particle
{
public :
	bool		active = true;			// Active (Yes/No)
	//
	SOGLColor   color;
	//
	float		m_fx = 0.0f;						// X Position
	float		m_fy = 0.0f;						// Y Position
	float		m_fz = 0.0f;						// Z Position
	//
	float		m_fVx = 0.0f;
	float		m_fVy = 0.0f;
	float		m_fVz = 0.0f;
	//
	CParticle() {};
	virtual ~CParticle() {};
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

