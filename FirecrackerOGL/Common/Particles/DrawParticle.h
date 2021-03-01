#pragma once


class DrawParticle {
public :
	virtual ~DrawParticle() {};
	virtual void AddParticle() = 0;
	virtual void Draw() = 0;

};

class DrawPointParticle : public DrawParticle {
public:
	virtual ~DrawPointParticle() {};
	virtual void AddParticle() {};
	virtual void Draw() {};

};