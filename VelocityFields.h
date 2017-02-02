#ifndef VelocityFields_H
#define VelocityFields_H

sf::Vector2f velocitySum(sf::Vector2f& position, std::function<sf::Vector2f(sf::Vector2f)> func)
{
	return func(position); //can now return multiple things, fuck yeah
}

sf::Vector2f noFlow(const sf::Vector2f& position)
{
	return sf::Vector2f(0, 0);
}

sf::Vector2f uniformFlow(const sf::Vector2f& position)
{
	return(sf::Vector2f(flowStrength, 0));
}

sf::Vector2f shearFlow(const sf::Vector2f& position)
{
	return sf::Vector2f(-0.01*flowStrength*(position.y-600), 0);
}

sf::Vector2f stagnationPointFlow(const sf::Vector2f& position)
{
	return sf::Vector2f(0.01*flowStrength*(position.x - 400), -0.01*flowStrength*(position.y - 300));
}

sf::Vector2f sphereFlow(const sf::Vector2f& position)
{
	return sf::Vector2f(0.5*flowStrength*(((powf(radius, 3)) / (powf((powf(position.x - 400, 2) + powf(position.y - 300, 2)), 1.5))) + 2) - (3 * powf(radius, 3)*flowStrength*powf(position.x - 400, 2)) / (2 * powf((powf(position.x - 400, 2) + powf(position.y - 300, 2)), 2.5)), -(3 * powf(radius, 3)*flowStrength*(position.x - 400)*(position.y - 300)) / (2 * powf((powf(position.x - 400, 2) + powf(position.y - 300, 2)), 2.5)));
}

sf::Vector2f vortex(const sf::Vector2f& position)
{
	float angle = atan2f(position.x - 400, position.y - 300);
	return sf::Vector2f((-flowStrength*(position.x - 400)) / (sqrtf(powf(position.x - 400, 2) + powf(position.y - 300, 2))) - (flowStrength*(position.y - 300)) / (sqrtf(powf(position.x - 400, 2) + powf(position.y - 300, 2))), (flowStrength*(position.x - 400)) / (sqrtf(powf(position.x - 400, 2) + powf(position.y - 300, 2))) - (flowStrength*(position.y - 300)) / (sqrtf(powf(position.x - 400, 2) + powf(position.y - 300, 2))));
}

sf::Vector2f bathplugVortex(const sf::Vector2f& position)
{
	if (sqrtf(powf(position.x - 400, 2) + powf(position.y - 300, 2)) <= radius)
	{
		return sf::Vector2f((-flowStrength*(position.x - 400)) / (sqrtf(powf(position.x - 400, 2) + powf(position.y - 300, 2))) - (rankinevortexrotation*(position.y - 300)) / 2, ((rankinevortexrotation*(position.x - 400)) / 2) - (flowStrength*(position.y - 300)) / (sqrtf(powf(position.x - 400, 2) + powf(position.y - 300, 2))));
		//return sf::Vector2f(0, 0);
	}
	else
		return sf::Vector2f((-powf(radius, 2) * rankinevortexrotation*(position.y - 300)) / (2 * (powf(position.x - 400, 2) + powf(position.y - 300, 2))) - (flowStrength*(position.x - 400)) / (sqrtf(powf(position.x - 400, 2) + powf(position.y - 300, 2))), (powf(radius, 2) * rankinevortexrotation*(position.x - 400)) / (2 * (powf(position.x - 400, 2) + powf(position.y - 300, 2))) - (flowStrength*(position.y - 300)) / (sqrtf(powf(position.x - 400, 2) + powf(position.y - 300, 2))));
}

sf::Vector2f liftonCylinder(const sf::Vector2f& position)
{
	float x = position.x - 400;
	float y = position.y - 300;
	float xysquare = powf(x, 2) + powf(y, 2);

	return sf::Vector2f((2 * 3.14*powf(radius, 2)*flowStrength*(y*y - x*x) + xysquare*(2 * 3.14*flowStrength*xysquare - cylinderrotation*y)) / (2 * 3.14*powf(xysquare, 2)), (x*(cylinderrotation*xysquare - 4 * 3.14*powf(radius, 2)*flowStrength*y)) / (2 * 3.14*powf(xysquare, 2)));
}

#endif // !VelocityFields_H
