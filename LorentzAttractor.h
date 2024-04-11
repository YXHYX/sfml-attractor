#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

/*
*  We're restricting lorentz attractor
*  differential equations :
*  dx/dt = sigma * (y - x)
*  dy/dt = x * ( rho - z ) - y
*  dz/dt = x * y - beta * z
* 
* 
* 
*/

#define MAX_NUM_OF_POINTS 3000

// lower the speed/ increase timestep if system becomes too unstable
#define TIMESTEP 1
#define SPEED 10

enum class projectedPlane {XY, XZ, YZ};

class LorentzAttractor
{
private:
	// the duration that if it exceeds a value inserts the current position of head into the path
	float updatePathTime;
	projectedPlane plane;

	sf::Vector3f head, vhead; //the position and velocity of the point to update
	std::vector<sf::Vector3f> path; // rest of the body ( tail )
	
	//color of the path
	sf::Color color;
	//parameters, mess around them for fun
	float sigma, rho, beta;
public:

	bool rainbowColor; //sets a rainbow colored path

	//Copy constructor
	LorentzAttractor(const LorentzAttractor&& l) noexcept
	{
		rainbowColor = l.rainbowColor;
		updatePathTime = l.updatePathTime;
		plane = l.plane;
		head = l.head;
		vhead = l.vhead;
		color = l.color;
		sigma = l.sigma;
		rho = l.rho;
		beta = l.beta;
	}
	LorentzAttractor(const float sigma, const float rho, const float beta, sf::Color color = sf::Color::Red);
	virtual ~LorentzAttractor();

	//cursed name, but keep it cuz funny
	sf::Vector3f getHead();

	sf::Vector2f getProjectedHead(); // 2d coordinates of the head in the set plane
	void setProjectedPlane(projectedPlane p); // set the plane of which we project to
	
	void setSigma(float sigma);
	void setRho(float rho);
	void setBeta(float beta);

	//update the attractor and render
	void update(const float dt);
	void draw(sf::RenderTarget* target);

};

