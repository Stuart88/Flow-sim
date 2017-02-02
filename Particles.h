#ifndef Headers_H
#define Headers_H

#include <SFML\Graphics.hpp>
#include <functional>
#include <iostream>

//initial parameters for velocity fields
float flowStrength = 200;
float radius = 100;
float cylinderrotation = -100000;
float rankinevortexrotation = 10;

class ParticleEmitter : public sf::Drawable, public sf::Transformable
{

	struct particle
	{
		sf::Vector2f velocity;
		sf::Time lifetme;
	};

	sf::VertexArray vert_particles;
	std::vector<particle> particleVect;
	sf::Vector2f emitterPos;
	std::function<sf::Vector2f(sf::Vector2f)> particle_velocity;

public:

	ParticleEmitter(unsigned int count) :
		vert_particles(sf::Points, count),
		particleVect(count),
		emitterPos(0, 0)
	{}
	//initialise starting lifetime of particles to random values, to slightly avoid annoying clumping at start
	void initialise(void)
	{
		for (std::size_t i = 0; i < particleVect.size(); ++i)
		{
			particleVect[i].lifetme = sf::milliseconds((std::rand() % 5000));
		}
	}
	//set emitter position
	void setemitterPos(sf::Vector2f pos)
	{
		emitterPos = pos;
	}
	//get emitter position
	sf::Vector2f getemitterPos(void)
	{
		return sf::Vector2f(emitterPos.x, emitterPos.y);
	}
	//takes a given velocity function as an argument, e.g. setVelocityField(shearFlow);
	void setVelocityField(std::function<sf::Vector2f(sf::Vector2f)> func)
	{
		particle_velocity = func;
	}
	//update fixed emitters and mouse
	void update(const sf::Time& elapsed)
	{
		for (std::size_t i = 0; i < particleVect.size(); ++i)
		{
			sf::Vertex& v = vert_particles[i];
			particle& p = particleVect[i];
			p.lifetme -= elapsed;

			if (p.lifetme < sf::Time::Zero)
				resetparticles(i);

			p.velocity = particle_velocity(v.position);
			v.position += p.velocity*elapsed.asSeconds();
		}
	}
	//update random emitters on side walls 
	void updatehorizontal(sf::Time elapsed)
	{
		for (std::size_t i = 0; i < particleVect.size(); ++i)
		{
			sf::Vertex& v = vert_particles[i];
			particle& p = particleVect[i];
			p.lifetme -= elapsed;

			setemitterPos(sf::Vector2f(std::rand() % 800, emitterPos.y));

			if (p.lifetme < sf::Time::Zero)
				resetparticles(i);

			p.velocity = particle_velocity(v.position);
			v.position += p.velocity*elapsed.asSeconds();
		}
	}
	//update random emitters on top and bottom walls
	void updatevertical(sf::Time elapsed)
	{
		for (std::size_t i = 0; i < particleVect.size(); ++i)
		{
			sf::Vertex& v = vert_particles[i];
			particle& p = particleVect[i];
			p.lifetme -= elapsed;

			setemitterPos(sf::Vector2f(emitterPos.x, std::rand() % 600));

			if (p.lifetme < sf::Time::Zero)
				resetparticles(i);

			p.velocity = particle_velocity(v.position);
			v.position += p.velocity*elapsed.asSeconds();
		}
	}
	// set particles colour
	void setColour(sf::Color colour)
	{
		for (std::size_t i = 0; i < particleVect.size(); ++i)
		{
			vert_particles[i].color = colour;
		}
	}

private:

	void resetparticles(std::size_t index)
	{
		vert_particles[index].position = emitterPos;
		particleVect[index].lifetme = sf::milliseconds((std::rand() % 3000) + 3000);

	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the entity's transform -- combine it with the one that was passed by the caller
		states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

											// apply the texture
		states.texture = NULL;

		// you may also override states.shader or states.blendMode if you want

		// draw the vertex array
		target.draw(vert_particles, states);
	}

};

#endif