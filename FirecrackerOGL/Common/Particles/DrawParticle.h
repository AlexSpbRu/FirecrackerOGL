#pragma once


class DrawParticle {
public :
	virtual ~DrawParticle() {};
	virtual void AddParticle(CTextVertexColorBuffer& DrawBuffer) = 0;
	virtual void Draw() = 0;

};

class DrawPointParticle : public DrawParticle {
public:
	~DrawPointParticle() {};
	void AddParticle(CTextVertexColorBuffer& DrawBuffer)  override {};
	void Draw()  override {};

};