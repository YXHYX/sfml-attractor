#include "LorentzAttractor.h"

LorentzAttractor::LorentzAttractor(const float sigma, const float rho, const float beta, sf::Color color)
	: sigma(sigma), rho(rho), beta(beta), color(color)
{
	this->head.x = 1;
	this->head.y = 1;
	this->head.z = 1;
	this->path.clear();

	this->path.push_back(this->head);
	this->rainbowColor = false;
}

LorentzAttractor::~LorentzAttractor()
{
	this->path.clear();
}

sf::Vector3f LorentzAttractor::getHead()
{
	return this->head;
}

sf::Vector2f LorentzAttractor::getProjectedHead()
{
	switch (this->plane)
	{
	case projectedPlane::XY:
		return sf::Vector2f(this->head.x, this->head.y);
	case projectedPlane::XZ:
		return sf::Vector2f(this->head.x, this->head.z);
	case projectedPlane::YZ:
		return sf::Vector2f(this->head.y, this->head.z);
	default:
		return sf::Vector2f(this->head.x, this->head.y);
	}
}

void LorentzAttractor::setProjectedPlane(projectedPlane p)
{
	this->plane = p;
}

void LorentzAttractor::setSigma(float sigma)
{
	this->sigma = sigma;
}

void LorentzAttractor::setRho(float rho)
{
	this->rho = rho;
}

void LorentzAttractor::setBeta(float beta)
{
	this->beta = beta;
}

void LorentzAttractor::update(const float dt)
{
	//increase the updatePathTime as time goes on
	this->updatePathTime += dt;

	for(int i = 0; i < TIMESTEP; i++)
	{
		this->vhead.x = this->sigma * (this->head.y - this->head.x) * (dt * SPEED) / TIMESTEP;
		this->vhead.y = ((this->rho - this->head.z) * this->head.x - this->head.y) * (dt*SPEED) / TIMESTEP;
		this->vhead.z = (this->head.x * this->head.y - this->beta * this->head.z)* (dt * SPEED) / TIMESTEP;

		this->head.x += this->vhead.x * (dt * SPEED) / TIMESTEP;
		this->head.y += this->vhead.y * (dt * SPEED) / TIMESTEP;
		this->head.z += this->vhead.z * (dt * SPEED) / TIMESTEP;
	}

	if (this->path.size() > MAX_NUM_OF_POINTS)
		this->path.pop_back();

	//insert new position and reset the updatePathTime
	if (this->updatePathTime > 0.04)
	{
		this->path.insert(this->path.begin(), this->head);
		this->updatePathTime = 0;
	}
	
	//std::cout << "HEAD COORDINATS :  " << this->head.x << "  //  " << this->head.y << std::endl;
}

void LorentzAttractor::draw(sf::RenderTarget* target)
{
	sf::VertexArray vao(sf::LineStrip, this->path.size());
	vao.clear();
	
	int size = this->path.size();
	for (int i = 0; i < size; i++)
	{
		sf::Color lineColor = color;
		if(rainbowColor)
		{
			lineColor.r = 127 * std::sin(i ) + 127;
			lineColor.g = 127 * std::cos(i ) + 127;
			lineColor.b = 127 * std::sin(-i) + 127;
		}
		//std::cout << "POINT COORDINATES "<< i << " :  " << this->head.x << "  //  " << this->head.y << "  //  " << this->head.z << std::endl;
		sf::Vertex v;
		switch(plane)
		{
		case projectedPlane::XY:

			v = sf::Vertex(sf::Vector2f(path[i].x, path[i].y), lineColor);
			vao.append(v);
			break;
		case projectedPlane::XZ:

			v = sf::Vertex(sf::Vector2f(path[i].x, path[i].z), lineColor);
			vao.append(v);
			break;
		case projectedPlane::YZ:

			v = sf::Vertex(sf::Vector2f(path[i].y, path[i].z), lineColor);
			vao.append(v);
			break;
		}
	}
	target->draw(vao);
}
