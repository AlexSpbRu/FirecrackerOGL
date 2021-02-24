#include "stdafx.h"

#include "Particles.h" 
#include "ParticleModel.h" 

//GLvoid	CModel::ProcessKeyboard(const bool* Keys, size_t CountKeys)   // управление потоком частиц
//{
//
//	// If Up Arrow And Y Speed Is Less Than 200 Increase Upward Speed
//	if (Keys[VK_UP] && (m_fYspeed<200)) m_fYspeed += 1.0f;
//	// If Down Arrow And Y Speed Is Greater Than -200 Increase Downward Speed
//	if (Keys[VK_DOWN] && (m_fYspeed>-200)) m_fYspeed -= 1.0f;
//	// If Right Arrow And X Speed Is Less Than 200 Increase Speed To The Right
//	if (Keys[VK_RIGHT] && (m_fXspeed<200)) m_fXspeed += 1.0f;
//	// If Left Arrow And X Speed Is Greater Than -200 Increase Speed To The Left
//	if (Keys[VK_LEFT] && (m_fXspeed>-200)) m_fXspeed -= 1.0f;
//}

void CModelGravitation::InitParticle( CParticle& particle, float Vx, float Vy, float Vz)
{
	CGravityParticle& Particle = dynamic_cast<CGravityParticle&>(particle);
	Particle.active = true;									// Make All The Particles Active
	Particle.life = 1.0f;									// Give All The Particles Full Life
	Particle.fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Speed
	Particle.color.r = float(rand() % 50) / 50.0f;				// Select Red Rainbow Color
	Particle.color.g = float(rand() % 50) / 50.0f;				// Select Green Rainbow Color
	Particle.color.b = float(rand() % 50) / 50.0f;				// Select Blue Rainbow Color
	Particle.m_fVx = Vx + float((rand() % 10) - 6.0f) / 5.0f;		// Random Speed On X Axis
	Particle.m_fVy = Vy + float((rand() % 10) - 5.0f) / 5.0f;		// Random Speed On Y Axis
	Particle.m_fVz = Vz;// +float((rand() % 10) - 5.0f)*5.0f;		// Random Speed On Z Axis
	Particle.xg = 0.0f;										// Set Horizontal Pull To Zero
	Particle.yg = -0.8f;										// Set Vertical Pull Downward
	Particle.zg = 0.0f;
	Particle.m_fMass = 1.0f;
	Particle.m_fx = 0.0f;
	Particle.m_fy = 0.0f;
	Particle.m_fz = 0.0f;
}

void CModelGravitation::CaclParticle(CParticle& particle, double Dt)
{
	if (!particle.active)
		return;
	CGravityParticle& Particle = dynamic_cast<CGravityParticle&>(particle);
	float ax = 0.0f;
	float ay = 0.0f;
	float az = 0.0f;
	float minDist = 1000000.0f;
	for (auto& mass : m_vMasses) {
		float dx = Particle.m_fx - mass.m_fX;
		float dy = Particle.m_fy - mass.m_fY;
		float dz = Particle.m_fz - mass.m_fZ;
		float dist = sqrtf(dx*dx + dy * dy + dz * dz);
		float Xang = -dx / (dist + 0.00001);
		float Yang = -dy / (dist + 0.00001);
		float Zang = -dz / (dist + 0.00001);
		float F = mass.m_fMass*m_GravityConst / (Particle.m_fMass*dist*dist);
		ax += Xang * F;
		ay += Yang * F;
		az += Zang * F;
		minDist = min(minDist, dist);
	}
	Particle.life = minDist;
	Particle.m_fVx += ax * Dt;
	Particle.m_fVy += ay * Dt;
	Particle.m_fVz += az * Dt;
	Particle.m_fx += Particle.m_fVx*Dt;
	Particle.m_fy += Particle.m_fVy*Dt;
	Particle.m_fz += Particle.m_fVz*Dt;

	//
	if (Particle.life > 100.0f || Particle.life < 0.1f) { //  частица улетела или сгорела
		InitParticle(Particle, m_fXspeed, m_fYspeed, 0.0f);
	}
}



void CModelStream::InitParticle(CParticle& particle, float Vx, float Vy, float Vz)
{
	CGravityParticle& Particle = dynamic_cast<CGravityParticle&>(particle);
	static auto num = 0;
	Particle.active = true;									// Make All The Particles Active
	Particle.life = 1.0f;									// Give All The Particles Full Life
	Particle.fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Speed
	Particle.color.r = m_Colors[num % 12][0];				// Select Red Rainbow Color
	Particle.color.g = m_Colors[num % 12][1];				// Select Green Rainbow Color
	Particle.color.b = m_Colors[num % 12][2];				// Select Blue Rainbow Color
	Particle.m_fVx = float((rand() % 50) - 26.0f)*10.0f;		// Random Speed On X Axis
	Particle.m_fVy = float((rand() % 50) - 25.0f)*10.0f;		// Random Speed On Y Axis
	Particle.m_fVz = float((rand() % 50) - 25.0f)*10.0f;		// Random Speed On Z Axis
	Particle.xg = 0.0f;										// Set Horizontal Pull To Zero
	Particle.yg = -0.8f;										// Set Vertical Pull Downward
	Particle.zg = 0.0f;										// Set Pull On Z Axis To Zero
	num++;

}


void CModelStream::CaclParticle(CParticle& particle, double Dt)
{
	if (particle.active)							// If The Particle Is Active
	{
		CGravityParticle& Particle = dynamic_cast<CGravityParticle&>(particle);
		Particle.m_fx += Particle.m_fVx / (m_fSlowdown * 1000);// Move On The X Axis By X Speed
		Particle.m_fy += Particle.m_fVy / (m_fSlowdown * 1000);// Move On The Y Axis By Y Speed
		Particle.m_fz += Particle.m_fVz / (m_fSlowdown * 1000);// Move On The Z Axis By Z Speed

		Particle.m_fVx += Particle.xg;			// Take Pull On X Axis Into Account
		Particle.m_fVy += Particle.yg;			// Take Pull On Y Axis Into Account
		Particle.m_fVz += Particle.zg;			// Take Pull On Z Axis Into Account
		Particle.life -= Particle.fade;		// Reduce Particles Life By 'Fade'

		if (Particle.life < 0.0f)					// If Particle Is Burned Out
		{
			Particle.life = 1.0f;					// Give It New Life
			Particle.fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Value
			Particle.m_fx = 0.0f;						// Center On X Axis
			Particle.m_fy = 0.0f;						// Center On Y Axis
			Particle.m_fz = 0.0f;						// Center On Z Axis
			Particle.m_fVx = m_fXspeed + float((rand() % 60) - 32.0f);	// X Axis Speed And Direction
			Particle.m_fVy = m_fYspeed + float((rand() % 60) - 30.0f);	// Y Axis Speed And Direction
			Particle.m_fVz = float((rand() % 60) - 30.0f);	// Z Axis Speed And Direction
			Particle.color.r = m_Colors[m_iCol][0];			// Select Red From Color Table
			Particle.color.g = m_Colors[m_iCol][1];			// Select Green From Color Table
			Particle.color.b = m_Colors[m_iCol][2];			// Select Blue From Color Table
		}
		
	}
}