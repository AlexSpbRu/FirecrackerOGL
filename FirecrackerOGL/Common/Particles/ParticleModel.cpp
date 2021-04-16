#include "stdafx.h"

#include "Shader.h"
#include "Math3D.h"
#include "utils.h"
#include "Textures.h"
#include "OGLBuffers.h"
#include "Particles.h" 
#include "ParticleModel.h" 


void CFireworkParticleModel::InitParticle(CParticle& Particle, const Vec3&  Position, const Vec3& Speed) {
}


void CFireworkParticleModel::CaclParticle(CParticle& Particle, float Dt) {
}





void CModelGravitation::InitParticle( CParticle& particle, const Vec3&  Position, const Vec3& Speed)
{
	CGravityParticle& Particle = dynamic_cast<CGravityParticle&>(particle);
	Particle.setActive( true );									// Make All The Particles Active
	Particle.life = 1.0f;									// Give All The Particles Full Life
	Particle.fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Speed
	Particle.setColor(SOGLColor(float(rand() % 50) / 50.0f, float(rand() % 50) / 50.0f, float(rand() % 50) / 50.0f, 1.0f));
	//Particle.color.r = float(rand() % 50) / 50.0f;				// Select Red Rainbow Color
	//Particle.color.g = float(rand() % 50) / 50.0f;				// Select Green Rainbow Color
	//Particle.color.b = float(rand() % 50) / 50.0f;				// Select Blue Rainbow Color
	Particle.m_Speed.x = Speed.x + float((rand() % 10) - 6.0f) / 5.0f;		// Random Speed On X Axis
	Particle.m_Speed.y = Speed.y + float((rand() % 10) - 5.0f) / 5.0f;		// Random Speed On Y Axis
	Particle.m_Speed.z = Speed.z;// +float((rand() % 10) - 5.0f)*5.0f;		// Random Speed On Z Axis
	Particle.xg = 0.0f;										// Set Horizontal Pull To Zero
	Particle.yg = -0.8f;										// Set Vertical Pull Downward
	Particle.zg = 0.0f;
	Particle.m_fMass = 1.0f;
	Particle.m_Pos.x = 0.0f;
	Particle.m_Pos.y = 0.0f;
	Particle.m_Pos.z = 0.0f;
}

void CModelGravitation::CaclParticle(CParticle& particle, float Dt)
{
	if (!particle.getActive())
		return;
	CGravityParticle& Particle = dynamic_cast<CGravityParticle&>(particle);
	float ax = 0.0f;
	float ay = 0.0f;
	float az = 0.0f;
	float minDist = 1000000.0f;
	for (auto& mass : m_vMasses) {
		float dx = Particle.m_Pos.x - mass.m_fX;
		float dy = Particle.m_Pos.y - mass.m_fY;
		float dz = Particle.m_Pos.z - mass.m_fZ;
		float dist = sqrtf(dx*dx + dy * dy + dz * dz);
		float Xang = -dx / (dist + 0.00001f);
		float Yang = -dy / (dist + 0.00001f);
		float Zang = -dz / (dist + 0.00001f);
		float F = mass.m_fMass*m_GravityConst / (Particle.m_fMass*dist*dist);
		ax += Xang * F;
		ay += Yang * F;
		az += Zang * F;
		minDist = min(minDist, dist);
	}
	Particle.life = minDist;
	Particle.m_Speed.x += ax * Dt;
	Particle.m_Speed.y += ay * Dt;
	Particle.m_Speed.z += az * Dt;
	Particle.m_Pos.x += Particle.m_Speed.x*Dt;
	Particle.m_Pos.y += Particle.m_Speed.y*Dt;
	Particle.m_Pos.z += Particle.m_Speed.z*Dt;

	//
	if (Particle.life > 100.0f || Particle.life < 0.1f) { //  частица улетела или сгорела
		InitParticle(Particle, Vec3(), Vec3( m_fXspeed, m_fYspeed, 0.0f ) );
	}
}



void CModelStream::InitParticle(CParticle& particle, const Vec3&  Position, const Vec3& Speed)
{
	CGravityParticle& Particle = dynamic_cast<CGravityParticle&>(particle);
	static auto num = 0;
	Particle.setActive( true );									// Make All The Particles Active
	Particle.life = 1.0f;									// Give All The Particles Full Life
	Particle.fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Speed
	Particle.setColor(SOGLColor(m_Colors[num % 12][0], m_Colors[num % 12][1], m_Colors[num % 12][2], 1.0f));
	//Particle.color.r = m_Colors[num % 12][0];				// Select Red Rainbow Color
	//Particle.color.g = m_Colors[num % 12][1];				// Select Green Rainbow Color
	//Particle.color.b = m_Colors[num % 12][2];				// Select Blue Rainbow Color
	Particle.m_Speed.x = float((rand() % 50) - 26.0f)*10.0f;		// Random Speed On X Axis
	Particle.m_Speed.y = float((rand() % 50) - 25.0f)*10.0f;		// Random Speed On Y Axis
	Particle.m_Speed.z = float((rand() % 50) - 25.0f)*10.0f;		// Random Speed On Z Axis
	Particle.xg = 0.0f;										// Set Horizontal Pull To Zero
	Particle.yg = -0.8f;										// Set Vertical Pull Downward
	Particle.zg = 0.0f;										// Set Pull On Z Axis To Zero
	num++;

}


void CModelStream::CaclParticle(CParticle& particle, float Dt)
{
	if (particle.getActive())							// If The Particle Is Active
	{
		CGravityParticle& Particle = dynamic_cast<CGravityParticle&>(particle);
		Particle.m_Pos.x += Particle.m_Speed.x / (m_fSlowdown * 1000);// Move On The X Axis By X Speed
		Particle.m_Pos.y += Particle.m_Speed.y / (m_fSlowdown * 1000);// Move On The Y Axis By Y Speed
		Particle.m_Pos.z += Particle.m_Speed.z / (m_fSlowdown * 1000);// Move On The Z Axis By Z Speed

		Particle.m_Speed.x += Particle.xg;			// Take Pull On X Axis Into Account
		Particle.m_Speed.y += Particle.yg;			// Take Pull On Y Axis Into Account
		Particle.m_Speed.z += Particle.zg;			// Take Pull On Z Axis Into Account
		Particle.life -= Particle.fade;		// Reduce Particles Life By 'Fade'

		if (Particle.life < 0.0f)					// If Particle Is Burned Out
		{
			Particle.life = 1.0f;					// Give It New Life
			Particle.fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Value
			Particle.m_Pos.x = 0.0f;						// Center On X Axis
			Particle.m_Pos.y = 0.0f;						// Center On Y Axis
			Particle.m_Pos.z = 0.0f;						// Center On Z Axis
			Particle.m_Speed.x = m_fXspeed + float((rand() % 60) - 32.0f);	// X Axis Speed And Direction
			Particle.m_Speed.y = m_fYspeed + float((rand() % 60) - 30.0f);	// Y Axis Speed And Direction
			Particle.m_Speed.z = float((rand() % 60) - 30.0f);	// Z Axis Speed And Direction
			Particle.setColor(SOGLColor(m_Colors[m_iCol][0], m_Colors[m_iCol][1], m_Colors[m_iCol][2], 1.0f));
			//Particle.color.r = m_Colors[m_iCol][0];			// Select Red From Color Table
			//Particle.color.g = m_Colors[m_iCol][1];			// Select Green From Color Table
			//Particle.color.b = m_Colors[m_iCol][2];			// Select Blue From Color Table
		}
		
	}
}