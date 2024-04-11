
//2024/04/11
/*
* Simulation of attractors in SFML
* had nothing better to do
*/

#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "LorentzAttractor.h"

int main() 
{
	sf::RenderWindow window(sf::VideoMode(800,600), "SFML Attractor");
	int numOfAttractors;
	std::cout << "Enter the number of lorentz attractors: ";
	std::cin >> numOfAttractors;
	std::cout << "\nPlease specify sigma, rho and beta:";
	
	std::vector<LorentzAttractor> attractors;

	for(int i = 0; i < numOfAttractors; i++)
	{
		float sigma, rho, beta;
		sf::Color color;
		std::cout << "\n Sigma: ";
		std::cin >> sigma;
		std::cout << " | Rho: ";
		std::cin >> rho;
		std::cout << " | Beta: ";
		std::cin >> beta;
		//attractors.push_back(LorentzAttractor(sigma, rho, beta, color));
		attractors[i].rainbowColor = true;
	}
	std::cout << "\nUse arrow keys to navigate through, I O P keys to change plane of view, number keys to select which attractor to follow\n";
	sf::Clock clock;
	float dt =0.0001;

	sf::View view;
	while (window.isOpen()) 
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
			
			//zoom in n out
			if (e.type == sf::Event::MouseWheelScrolled)
				view.zoom(e.mouseWheelScroll.delta < 0 ? 1 + dt*10 : 1 - dt * 10);

			//show different views of the attractors from different planes
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
			{		
				for (auto& e : attractors)
					e.setProjectedPlane(projectedPlane::XY);
				std::cout << "PLANE XY" << std::endl;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			{		
				for (auto& e : attractors)
					e.setProjectedPlane(projectedPlane::XZ);
				std::cout << "PLANE XZ" << std::endl;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{
				for (auto& e : attractors)
					e.setProjectedPlane(projectedPlane::YZ);
				std::cout << "PLANE YZ" << std::endl;
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				view.move(0, -1000.* dt);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				view.move(0, 1000 * dt);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				view.move(-1000 * dt, 0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				view.move(1000 * dt, 0);

			//set the center of view to that of the 1st attractor
			for (int i = 0; i < numOfAttractors; i++)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(sf::Keyboard::Num0+i)))
					view.setCenter(attractors[i].getProjectedHead());
			}
		}
		
		for(auto &e : attractors)
			e.update(dt);
		
		//set the window to the current view
		window.setView(view);
		
		//render attractors
		window.clear(sf::Color::Black);
		for (auto& e : attractors)
			e.draw(&window);
		window.display();
		
		dt = clock.restart().asSeconds();
	}
	return 0;
}