#include "Particles.h"
#include "VelocityFields.h"
#include <iostream>


int main()
{

	// INITIALISATION

	sf::Clock clock;
	sf::Clock democlockVield;
	sf::Clock democlockColours;
	sf::Clock democlockStreamlines;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Flow Sim");
	window.setFramerateLimit(60);

	//fonts and textures for words/sprites
	sf::Font font;
	font.loadFromFile("Welbut__.ttf");
	sf::Text Texts[15];
	sf::Text &flowSpeedText = Texts[2];
	sf::Text &radiusText = Texts[1];
	sf::Text &cylinderRotationText = Texts[3];
	sf::Text &vortexRotationText = Texts[0];

	
	for (std::size_t i = 0; i < 15;++i)
	{
		Texts[i].setFont(font);
		Texts[i].setCharacterSize(12);
		if (i < 3)
			Texts[i].setPosition(5, 585 - (i * 13));
		if (3 < i && i < 12)
			//Texts[i].setPosition(5, 546);
		{
			Texts[i].setPosition(10, 10);
			Texts[i].setCharacterSize(40);
		}

	}
	Texts[3].setPosition(5, 585);
	Texts[12].setPosition(5, 520);
	Texts[13].setPosition(280, 585);
	Texts[14].setPosition(5, 580);

	Texts[4].setString("No flow");
	Texts[5].setString("Uniform flow");
	Texts[6].setString("Shear flow");
	Texts[7].setString("Stagnation point flow");
	Texts[8].setString("Flow past a sphere (cross-section)");
	Texts[9].setString("Basic vortex");
	Texts[10].setString("Rankine vortex");
	Texts[11].setString("Flow past a rotating cylinder");
	Texts[12].setString("Streamlines only: On");
	Texts[13].setString("Press H for help           ESC to quit          Press I for flow info");
	Texts[14].setString("DEMO MODE: ON  - Press any key");

	sf::Texture infoTexture;
	infoTexture.loadFromFile("Info.png");
	sf::Sprite infoSprite;
	infoSprite.setTexture(infoTexture);
	infoSprite.setPosition(100, 100);

	sf::Texture infoTextures[8];
	infoTextures[0].loadFromFile("_0No_field.png");
	infoTextures[1].loadFromFile("_1uniform_flow.png");
	infoTextures[2].loadFromFile("_2Shearflow.png");
	infoTextures[3].loadFromFile("_3Stagnationpointflow.png");
	infoTextures[4].loadFromFile("_4Sphereflow.png");
	infoTextures[5].loadFromFile("_5Basicvortex.png");
	infoTextures[6].loadFromFile("_6Rankinevortex.png");
	infoTextures[7].loadFromFile("_7Cylinderflow.png");

	sf::Sprite infoSprites[8];
	for (std::size_t i = 0;i < 8;++i)
	{
		infoSprites[i].setTexture(infoTextures[i]);
		infoSprites[i].setPosition(440, 410);
	}
	
	//for button press toggles
	bool infoDraw = 0;// "H"
	bool flowinfoDraw = 0;// "I"
	bool streamlinesOnly = 0;// "S"
	bool demoOn = 1;

	int streamlinesColourSelecter = 0; //cycles through 0-7 to choose colour from array below - Button press "C"
	sf::Color Colours[7] =
	{
		sf::Color::White, sf::Color::Red, sf::Color::Green,
		sf::Color::Blue, sf::Color::Yellow, sf::Color::Cyan,
		sf::Color::Magenta
	};
	

	

	//central circle overlay for rotational fields
	sf::CircleShape radiusCircle;
	radiusCircle.setRadius(radius);
	radiusCircle.setOrigin(radius, radius);
	radiusCircle.setPosition(400, 300);
	radiusCircle.setOutlineThickness(1);
	radiusCircle.setOutlineColor(sf::Color::Blue);
	radiusCircle.setFillColor(sf::Color::Transparent);

	//velocity fields
	int VfieldNum = 1;
	std::vector <std::function<sf::Vector2f(sf::Vector2f)>> Vfields;
	Vfields.push_back(noFlow);
	Vfields.push_back(uniformFlow);
	Vfields.push_back(shearFlow);
	Vfields.push_back(stagnationPointFlow);
	Vfields.push_back(sphereFlow);
	Vfields.push_back(vortex);
	Vfields.push_back(bathplugVortex);
	Vfields.push_back(liftonCylinder);

	//mousepoint particle emitter
	ParticleEmitter mousepointEmitter(1000);
	mousepointEmitter.setColour(sf::Color::Yellow);
	//fixed emitters
	ParticleEmitter emitPointsfixed[36] =
	{
		(500), (500), (500), (500), (500), (500), (500), (500), (500), (500),
		(500),													       (500),
		(500),                                                         (500),
		(500),                                                         (500),
		(500),                                                         (500),
		(500),         /*    Screen          */                        (500),
		(500),                                                         (500),
		(500),                                                         (500),
		(500),                                                         (500),
		(500), (500), (500), (500), (500), (500), (500), (500), (500), (500)
	};
	
	//random emitters (positions randomises on walls so emission happens everwhere
	ParticleEmitter emitPoints[4] = { 500, 500, 500, 500 };
	emitPoints[0].setemitterPos(sf::Vector2f(400, 0));
	emitPoints[1].setemitterPos(sf::Vector2f(400, 601));
	emitPoints[2].setemitterPos(sf::Vector2f(-1, 300));
	emitPoints[3].setemitterPos(sf::Vector2f(800, 300));
	emitPoints[0].setVelocityField(Vfields[1]);
	emitPoints[1].setVelocityField(Vfields[1]);
	emitPoints[2].setVelocityField(Vfields[1]);
	emitPoints[3].setVelocityField(Vfields[1]);



	//attach fixed emitters to walls and corners
	for (int i = 0; i < 36; ++i)
	{
		if (i < 10)
			emitPointsfixed[i].setemitterPos(sf::Vector2f(i*(70) + 11, 0));
		if ((i > 9) && (i < 25) && i % 2 == 0)
			emitPointsfixed[i].setemitterPos(sf::Vector2f(-1, (i - 9)*(600 / 16 -1)));
		if ((i > 9) && (i < 25) && i % 2 != 0)
			emitPointsfixed[i].setemitterPos(sf::Vector2f(800, (i - 10) * (600 / 16) +1));
		if (i > 25)
			emitPointsfixed[i].setemitterPos(sf::Vector2f((i - 25) * 70 + 11, 601));
	}

	//initialise velocity field for emitted particles to act in
	mousepointEmitter.setVelocityField(Vfields[1]);
	for (std::size_t i = 0; i < 36; ++i)
	{
		emitPointsfixed[i].setVelocityField(Vfields[1]);
	}

	//initialise particle lifetimes for first wave (gets around issue of massive clump of particles at start)
	mousepointEmitter.initialise();
	for (std::size_t i = 0; i < 36; ++i)
	{
		if (i < 4)
			emitPoints[i].initialise();
		emitPointsfixed[i].initialise();
	}

	
	


	//MAIN WINDOW LOOP
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//KEYPRESSES
			if (event.type == sf::Event::KeyPressed)
			{
				//For any key pressed, turn off demo mode (if turned on)
				if (demoOn == 1)
				{   
					demoOn = 0;
					continue;
				}
				//Turn on Demo mode
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
					if (demoOn == 0)
					{
						demoOn = 1;
						flowinfoDraw = 0;
						infoDraw = 0;
					}
					else
						demoOn = 0;
				}
				//Toggle streamlines
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					if (streamlinesOnly == 0)
						streamlinesOnly = 1;
					else
						streamlinesOnly = 0;
				}
				//View info screen
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
				{
					if (infoDraw == 0)
						infoDraw = 1;
					else
						infoDraw = 0;
				}
				//View flow info
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
				{
					if (flowinfoDraw == 0)
						flowinfoDraw = 1;
					else
						flowinfoDraw = 0;
				}
				//Streamlines Colour
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				{
					++streamlinesColourSelecter;
					if (streamlinesColourSelecter > 6)
						streamlinesColourSelecter = 0;
					for (std::size_t i = 0; i < 36; ++i)
					{
						emitPointsfixed[i].setColour(Colours[streamlinesColourSelecter]);
					}
				}
				//No flow
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
				{
					VfieldNum = 0;
					mousepointEmitter.setVelocityField(Vfields[0]);

					for (std::size_t i = 0; i < 36; ++i)
					{
						if (i < 4)
							emitPoints[i].setVelocityField(Vfields[0]);
						emitPointsfixed[i].setVelocityField(Vfields[0]);
					}
				}
				// uniform flow
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
				{
					VfieldNum = 1;
					mousepointEmitter.setVelocityField(Vfields[1]);

					for (std::size_t i = 0; i < 36; ++i)
					{
						if (i < 4)
							emitPoints[i].setVelocityField(Vfields[1]);
						emitPointsfixed[i].setVelocityField(Vfields[1]);
					}
				}
				//shear flow
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
				{
					VfieldNum = 2;
					mousepointEmitter.setVelocityField(Vfields[2]);

					for (std::size_t i = 0; i < 36; ++i)
					{
						if (i < 4)
							emitPoints[i].setVelocityField(Vfields[2]);
						emitPointsfixed[i].setVelocityField(Vfields[2]);
					}
				}
				//stagnation point flow
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
				{
					VfieldNum = 3;
					mousepointEmitter.setVelocityField(Vfields[3]);

					for (std::size_t i = 0; i < 36; ++i)
					{
						if (i < 4)
							emitPoints[i].setVelocityField(Vfields[3]);
						emitPointsfixed[i].setVelocityField(Vfields[3]);
					}
				}
				//Sphere flow
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
				{
					VfieldNum = 4;
					mousepointEmitter.setVelocityField(Vfields[4]);

					for (std::size_t i = 0; i < 36; ++i)
					{
						if (i < 4)
							emitPoints[i].setVelocityField(Vfields[4]);
						emitPointsfixed[i].setVelocityField(Vfields[4]);
					}
				}
				//Vortex flow
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
				{
					VfieldNum = 5;
					mousepointEmitter.setVelocityField(Vfields[5]);

					for (std::size_t i = 0; i < 36; ++i)
					{
						if (i < 4)
							emitPoints[i].setVelocityField(Vfields[5]);
						emitPointsfixed[i].setVelocityField(Vfields[5]);
					}
				}
				//Bathplug vortex (Rankine vortex)
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
				{
					VfieldNum = 6;
					mousepointEmitter.setVelocityField(Vfields[6]);

					for (std::size_t i = 0; i < 36; ++i)
					{
						if (i < 4)
							emitPoints[i].setVelocityField(Vfields[6]);
						emitPointsfixed[i].setVelocityField(Vfields[6]);
					}
				}
				//Lift on rotating cylinder
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
				{
					VfieldNum = 7;
					mousepointEmitter.setVelocityField(Vfields[7]);

					for (std::size_t i = 0; i < 36; ++i)
					{
						if (i < 4)
							emitPoints[i].setVelocityField(Vfields[7]);
						emitPointsfixed[i].setVelocityField(Vfields[7]);
					}
				}
				//set defaults (Reset)
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
				{
					if (VfieldNum == 0)//no flow
						continue;
					if (VfieldNum == 1 || VfieldNum == 2)//uniform and shear flows
						flowStrength = 200;
					if (VfieldNum == 3) // stagnation point flow
						flowStrength = 200;
					if (VfieldNum == 4) // sphere flow
					{
						radius = 120;
						flowStrength = 200;
						radiusCircle.setRadius(radius);
						radiusCircle.setOrigin(radius, radius);
						radiusCircle.setPosition(400, 300);
					}
					if (VfieldNum == 5) //plain vortex
						flowStrength = 100;
					if (VfieldNum == 6)//rankine vortex
					{
						radius = 50;
						rankinevortexrotation = 15;
						flowStrength = 100;
						radiusCircle.setRadius(radius);
						radiusCircle.setOrigin(radius, radius);
						radiusCircle.setPosition(400, 300);
					}
					if (VfieldNum == 7) // rotating cylinder
					{
						radius = 50;
						cylinderrotation = -100000;
						flowStrength = 200;
						radiusCircle.setRadius(radius);
						radiusCircle.setOrigin(radius, radius);
						radiusCircle.setPosition(400, 300);
					}
				}
				//Cycle through flows
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					++VfieldNum;
					if (VfieldNum > 7)
						VfieldNum = 0;

					//reset to best flow for given field (equivalent to pressing "R")
					if (VfieldNum == 1 || VfieldNum == 2)//uniform and shear flows
						flowStrength = 200;
					if (VfieldNum == 3) // stagnation point flow
						flowStrength = 200;
					if (VfieldNum == 4) // sphere flow
					{
						radius = 120;
						flowStrength = 200;
						radiusCircle.setRadius(radius);
						radiusCircle.setOrigin(radius, radius);
						radiusCircle.setPosition(400, 300);
					}
					if (VfieldNum == 5) //plain vortex
						flowStrength = 100;
					if (VfieldNum == 6)//rankine vortex
					{
						radius = 50;
						rankinevortexrotation = 15;
						flowStrength = 100;
						radiusCircle.setRadius(radius);
						radiusCircle.setOrigin(radius, radius);
						radiusCircle.setPosition(400, 300);
					}
					if (VfieldNum == 7) // rotating cylinder
					{
						radius = 50;
						cylinderrotation = -100000;
						flowStrength = 200;
						radiusCircle.setRadius(radius);
						radiusCircle.setOrigin(radius, radius);
						radiusCircle.setPosition(400, 300);
					}
			
					//update all points and emitters with new velocity field
					mousepointEmitter.setVelocityField(Vfields[VfieldNum]);
					for (std::size_t i = 0; i < 36; ++i)
					{
						if (i < 4)
							emitPoints[i].setVelocityField(Vfields[VfieldNum]);
						emitPointsfixed[i].setVelocityField(Vfields[VfieldNum]);
					}
					
				}
				//quit
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					if (infoDraw == 1)
						infoDraw = 0;
					else if (flowinfoDraw == 1)
						flowinfoDraw = 0;
					else
						window.close();
				}
					
				
			}
		}


		//KEYPRESSES WHICH NEED TO ACT INSTANTLY

		//increase Rankine vortex rotation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && VfieldNum == 6 && rankinevortexrotation < 50)
			rankinevortexrotation += 0.1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && VfieldNum == 6 && rankinevortexrotation > -50)
			rankinevortexrotation -= 0.1;
		//increase cylinder rotoation
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && VfieldNum == 7 && cylinderrotation > -500000)
			cylinderrotation -= 1000;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && VfieldNum == 7 && cylinderrotation < 500000)
			cylinderrotation += 1000;
		//change radius of cylinder/rankine/sphere
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && radius < 200 && (VfieldNum == 4 || VfieldNum == 6 || VfieldNum == 7))
		{
			++radius;
			radiusCircle.setRadius(radius);
			radiusCircle.setOrigin(radius, radius);
			radiusCircle.setPosition(400, 300);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && radius > 0 && (VfieldNum == 4 || VfieldNum == 6 || VfieldNum == 7))
		{
			--radius;
			radiusCircle.setRadius(radius);
			radiusCircle.setOrigin(radius, radius);
			radiusCircle.setPosition(400, 300);

		}
		//change flow strength
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma) && flowStrength > 0)
			--flowStrength;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period) && flowStrength < 400)
			++flowStrength;

		
		//UPDATES

		flowSpeedText.setString("Flow rate: " + std::to_string((int)flowStrength));
		cylinderRotationText.setString("Cylinder rotation: " + std::to_string((int)cylinderrotation / 1000));
		vortexRotationText.setString("Vorticity: " + std::to_string((int)rankinevortexrotation));
		radiusText.setString("Radius: " + std::to_string((int)radius));

		//set "mouse point emitter" to follow mouse co-rds
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		mousepointEmitter.setemitterPos(window.mapPixelToCoords(mouse));
		
		
		//update state of all particle emission points
		sf::Time elapsed = clock.restart();
		mousepointEmitter.update(elapsed);
		emitPoints[0].updatehorizontal(elapsed);
		emitPoints[1].updatehorizontal(elapsed);
		emitPoints[2].updatevertical(elapsed);
		emitPoints[3].updatevertical(elapsed);
		for (std::size_t i = 0; i < 36; ++i)
		{
			emitPointsfixed[i].update(elapsed);
		}

		// DEMO MODE
		if (demoOn == 1)
		{
			sf::Time demotimeV = democlockVield.getElapsedTime();
			sf::Time demotimeC = democlockColours.getElapsedTime();
			sf::Time demotimeS = democlockStreamlines.getElapsedTime();
			int timeV = demotimeV.asSeconds();
			int timeC = demotimeC.asSeconds();
			int timeS = demotimeS.asSeconds();
			
			if (timeC % 7 == 0 && timeC != 0)
			{
				democlockColours.restart();
				++streamlinesColourSelecter;
				if (streamlinesColourSelecter > 6)
					streamlinesColourSelecter = 0;
				for (std::size_t i = 0; i < 36; ++i)
				{
					emitPointsfixed[i].setColour(Colours[streamlinesColourSelecter]);
				}
			}
			
			if (timeV % 5 == 0 && timeV != 0)
			{
				democlockVield.restart();
				++VfieldNum;
				if (VfieldNum > 7)
					VfieldNum = 1;

				//reset to best flow for given field (equivalent to pressing "R")
				if (VfieldNum == 1 || VfieldNum == 2)//uniform and shear flows
					flowStrength = 200;
				if (VfieldNum == 3) // stagnation point flow
					flowStrength = 200;
				if (VfieldNum == 4) // sphere flow
				{
					radius = 120;
					flowStrength = 200;
					radiusCircle.setRadius(radius);
					radiusCircle.setOrigin(radius, radius);
					radiusCircle.setPosition(400, 300);
				}
				if (VfieldNum == 5) //plain vortex
					flowStrength = 100;
				if (VfieldNum == 6)//rankine vortex
				{
					radius = 50;
					rankinevortexrotation = 15;
					flowStrength = 100;
					radiusCircle.setRadius(radius);
					radiusCircle.setOrigin(radius, radius);
					radiusCircle.setPosition(400, 300);
				}
				if (VfieldNum == 7) // rotating cylinder
				{
					radius = 50;
					cylinderrotation = -100000;
					flowStrength = 200;
					radiusCircle.setRadius(radius);
					radiusCircle.setOrigin(radius, radius);
					radiusCircle.setPosition(400, 300);
				}

				//update all points and emitters with new velocity field
				mousepointEmitter.setVelocityField(Vfields[VfieldNum]);
				for (std::size_t i = 0; i < 36; ++i)
				{
					if (i < 4)
						emitPoints[i].setVelocityField(Vfields[VfieldNum]);
					emitPointsfixed[i].setVelocityField(Vfields[VfieldNum]);
				}
			}

			if (timeS % 10 == 0 && timeS != 0)
			{
				democlockStreamlines.restart();
				if (streamlinesOnly == 0)
					streamlinesOnly = 1;
				else
					streamlinesOnly = 0;
			}
		}



		//DRAW ALL ENTITIES

		window.clear();
		
		if (demoOn == 0)//draw mouse point emitter
			window.draw(mousepointEmitter);
		
		//"streamlines only" toggle
		if (streamlinesOnly == 0)
		{
			for (std::size_t i = 0; i < 4; ++i)
			{
				window.draw(emitPoints[i]);
			}
		}
		else if(demoOn == 0) //draw "Streamlines 
		{
			window.draw(Texts[12]);
		}
		//draw fixed emit points on edges
		for (std::size_t i = 0; i < 36; ++i)
		{
			window.draw(emitPointsfixed[i]);
		}
		//draw centre circle for relevant fields
		if (VfieldNum == 4 || VfieldNum == 6 || VfieldNum == 7)
		{
			window.draw(radiusCircle);
		}
		//draw relevant text for each velocity flow
		if (VfieldNum != 0 && demoOn == 0)
		{
			window.draw(flowSpeedText);
		}
		if (VfieldNum == 0)
		{
			window.draw(Texts[4]);
		}
		if (VfieldNum == 1)
		{
			window.draw(Texts[5]);
		}
		if (VfieldNum == 2)
		{
			window.draw(Texts[6]);
		}
		if (VfieldNum == 3)
		{
			window.draw(Texts[7]);
		}
		if (VfieldNum == 5)
		{
			window.draw(Texts[9]);
		}
		if ((VfieldNum == 4 || VfieldNum == 6 || VfieldNum == 7))
		{
			if (VfieldNum == 4)
				window.draw(Texts[8]);//Sphere flow
			if (VfieldNum == 6)
				window.draw(Texts[10]);//Rankie vortex
			if (VfieldNum == 7 )
				window.draw(Texts[11]);//rotating cylinder
			if(demoOn==0)
				window.draw(radiusText);
		}
		if (VfieldNum == 6 && demoOn == 0)
		{
			window.draw(vortexRotationText);
		}
		if (VfieldNum == 7 && demoOn == 0)
		{
			window.draw(cylinderRotationText);
		}
		//Draw text at bottom of screen "Press H for help etc..."
		if(demoOn == 0)
			window.draw(Texts[13]);
		//draw info screens (flow equations)
		for (size_t i = 0;i < 8;++i)
		{
			if (VfieldNum == i && flowinfoDraw == 1)
				window.draw(infoSprites[i]);
		}
		//draw help screen
		if (infoDraw == 1)
		{
			window.draw(infoSprite);
		} 

		//draw 'demo on' text
		if (demoOn == 1)
			window.draw(Texts[14]);
		
		
		window.display();

		// DEMO MODE LOOOP

		

	}



}
