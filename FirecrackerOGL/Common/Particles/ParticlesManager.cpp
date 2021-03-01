#include "stdafx.h"

#include "Particles.h" 
#include "ParticleModel.h" 
#include "ParticlesManager.h" 

//bool CParticlesManager::CreateTexture(const std::string& ImageName)
//{
//	//if (!m_Texture.CreateTexture(ImageName, GL_LINEAR, GL_LINEAR, 1))
//	//	return false;
//	//m_Texture.Bind();
//	return true;
//}
//
//
//GLvoid	CParticlesManager::InitPaticles(float Vx, float Vy, float Vz)
//{
//	if (m_Model == nullptr) return;
//	auto num = 0;
//	size_t count = m_vParticles.size();
//	for (auto& pat : m_vParticles)
//	{
//		m_Model->InitParticle(pat, Vx, Vy, Vz);
//	}
//	GetSystemTime(&m_LastDwayTime);
//}
//
//GLvoid	CParticlesManager::AddPaticles(const CParticle& Pat)
//{
//	m_vParticles.push_back(Pat);
//}
//
//int CParticlesManager::DrawGLScene()										// Here's Where We Do All The Drawing
//{
//	SYSTEMTIME	sys_time;
//	GetSystemTime(&sys_time);
//	double Dt = (sys_time.wHour - m_LastDwayTime.wHour)*3600.0 + (sys_time.wMinute - m_LastDwayTime.wMinute)*60.0 +
//		sys_time.wSecond - m_LastDwayTime.wSecond + (sys_time.wMilliseconds - m_LastDwayTime.wMilliseconds) / 1000.0;
//	m_LastDwayTime = sys_time;
//	//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
//	glLoadIdentity();										// Reset The ModelView Matrix
//
//	for (auto& pat : m_vParticles)
//	{
//		if (pat.active)								// If The Particle Is Active
//		{
//			//float x = pat.m_fx;						// Grab Our Particle X Position
//			//float y = pat.m_fy;						// Grab Our Particle Y Position
//			//float z = pat.m_fz + m_fZoom;						// Used To Zoom Out;					// Particle Z Pos + Zoom
//			//													// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
//			//glColor4f(pat.r, pat.g, pat.b, pat.life);
//
//			//glBegin(GL_TRIANGLE_STRIP);							// Build Quad From A Triangle Strip
//			//glTexCoord2d(1, 1); glVertex3f(x + pat.m_fDrawSize, y + pat.m_fDrawSize, z); // Top Right
//			//glTexCoord2d(0, 1); glVertex3f(x - pat.m_fDrawSize, y + pat.m_fDrawSize, z); // Top Left
//			//glTexCoord2d(1, 0); glVertex3f(x + pat.m_fDrawSize, y - pat.m_fDrawSize, z); // Bottom Right
//			//glTexCoord2d(0, 0); glVertex3f(x - pat.m_fDrawSize, y - pat.m_fDrawSize, z); // Bottom Left
//			//glEnd();												// Done Building Triangle Strip
//
//			if (m_Model != nullptr)
//				m_Model->CaclParticle(pat, Dt);
//		}
//	}
//	return TRUE;											// Everything Went OK
//}

