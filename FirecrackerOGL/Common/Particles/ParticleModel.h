#pragma once

#include <functional>
#include <ctime>
#include <random>


//----------------------------------------------

template <typename T>
class CDistributionValue {
protected :
	T	minValue;
	T	maxValue;
public:
	CDistributionValue() = default;
	CDistributionValue(T  MinVal, T MaxVal) : minValue(MinVal), maxValue(MaxVal) {}
	virtual ~CDistributionValue() {}

	virtual T operator()() = 0;
//	std::function<T()> operator &() { return &CDistributionValue<T>::operator(); }
};


template <typename T>
class CRandomDistributionValue : public CDistributionValue<T> {
public:
	CRandomDistributionValue() = default;
	CRandomDistributionValue(T  MinVal, T MaxVal) : CDistributionValue(MinVal, MaxVal)	{
		std::srand(std::time(0));
	}


	T operator()() override {
		return CDistributionValue::minValue + static_cast<T>(std::rand()) / RAND_MAX * CDistributionValue::maxValue;
	}
};

template <typename T>
class CNormalDistributionValue : public CDistributionValue<T> {
	std::default_random_engine generator;
	std::normal_distribution<T> distribution;
public:
	CNormalDistributionValue() = default;
	CNormalDistributionValue(T  mean, T dispertion) : CDistributionValue<T>(mean, dispertion), distribution(mean, dispertion) {
	}

	T operator()()  override {
		return distribution(generator);
	}
};

//----------------------------------------------

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
	using init_func = std::function< float() >;
	using initializers = std::vector<init_func>;

protected :

	initializers	m_phisicalInits;
	initializers	m_visualInits;

	void setInitializers(initializers& Initializers, std::initializer_list < init_func >& distributionFunctions) {
		Initializers.resize(distributionFunctions.size());
		int pos = 0;
		for (auto& func : distributionFunctions) {
			Initializers[pos] = std::move(func);
		}
	}


	template<class... Args>
	using are_same = std::conjunction<std::is_same<init_func, Args>...>;

	template <  typename Head,  typename...  Args, 
		class = typename std::enable_if_t< are_same<std::function<float()>, Head>::value ||
										   are_same<std::function<float()>, Head>::value , void> >
	void setInitializersPos(initializers& Initializers, Head head,  Args... args) {
		Initializers.emplace_back(head);
		setInitializersPos< Args... >(Initializers, args...);
	}
	template <  typename Head>
	void setInitializersPos(initializers& Initializers, Head head) {
		Initializers.emplace_back(head);
	}

	//typename std::enable_if_t<>;
	template <typename... Args >
	void setInitializers(initializers& Initializers, Args... args) {
		int count = sizeof...(Args);
		Initializers.resize(count);
		setInitializersPos< Args... >(Initializers, args...);
	}

public:

	CParticleModel() {}
	virtual ~CParticleModel() {};

	virtual  void setPhisicalValuesDF(std::initializer_list< init_func >&  distributionFunctions) {
		setInitializers(m_phisicalInits, distributionFunctions);
	}

	template <typename... Args>
	void setPhisicalValuesDF(Args... args) {
		setInitializers<Args...>(m_phisicalInits, args ...);
	}

	virtual  void setVisualValuesDF(std::initializer_list< init_func >&  distributionFunctions) {
		setInitializers(m_visualInits, distributionFunctions);
	}

	

	virtual void InitParticle( CParticle& Particle, const Vec3&  Position, const Vec3& Speed) = 0;    // задается начальное направление движения
	
	virtual void CaclParticle( CParticle& Particle, float Dt) = 0;
};



class CFireworkParticleModel : public CParticleModel {
	//  average data for initialization of perticles
	float		m_Faiding = 1.0f;			//  faiding coefficient
	float		m_FaidingDispersion = 0.1f;
	float		m_BlinkAmp = 1.0f;			//  amplitude of blink
	float		m_BlinkAmpDispersion = 0.1f;
	float		m_BlinkFrec = 1.0f;			//	frequency of blink
	float		m_BlinkFrecDispersion = 0.1f;
	float		m_Resistance = 1.0f;		//  resistance coefficient
	float		m_ResistanceDispersion = 0.1f;
	//	distribution function for initializing value of start speed and  resistance
	//std::function< float(float) >	phisicalValuesDF = [](float x) { return x; }; 
	//	distribution function for initializing of faiding and blinking values
	//std::function< float(float) >	visualValuesDF = [](float x) { return x; };

public:
	CFireworkParticleModel() {}
	~CFireworkParticleModel() {}
	// задание начальных  параметров частицы с использованием функции distributionFunction. 
	/*void setPhisicalValuesDF(std::function< float(float) >  distributionFunction) {
		phisicalValuesDF = distributionFunction;
	}
	void setVisualValuesDF(std::function< float(float) >  distributionFunction) {
		visualValuesDF = distributionFunction;
	}*/
	void InitParticle(CParticle& Particle, const Vec3&  Position, const Vec3& Speed) override;
	void CaclParticle(CParticle& Particle, float Dt) override;
	//  managing firework
	float		getFaiding() {
		return m_Faiding;
	}

	void		setFaiding(float Faiding, float Dispersion) {
		m_Faiding = Faiding;
		m_FaidingDispersion = Dispersion ;
	}

	float		getBlinkAmp() {
		return m_BlinkAmp;
	}

	void		setBlinkAmp(float BlinkAmp, float Dispersion) {
		m_BlinkAmp = BlinkAmp;
		m_BlinkAmpDispersion = Dispersion;
	}

	float		getBlinkFrec() {
		return m_BlinkFrec;
	}

	void		setBlinkFrec(float BlinkFrec, float Dispersion) {
		m_BlinkFrec = BlinkFrec;
		m_BlinkFrecDispersion = Dispersion;
	}

	float		getResistance() {
		return m_Resistance;
	}

	void		setResistance(float Resistance, float Dispersion) {
		m_Resistance = Resistance;
		m_ResistanceDispersion = Dispersion;
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

	void InitParticle(CParticle& Particle, const Vec3&  Position, const Vec3& Speed) override;
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
	void InitParticle(CParticle& Particle, const Vec3&  Position, const Vec3& Speed) override;
	void CaclParticle(CParticle& Particle, float Dt) override;
	
};
