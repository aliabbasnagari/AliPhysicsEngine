#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

int main()
{

	float KS = 10.0f; // Spring constant
	float L0 = 50.0f; // Natural length of the spring
	float M = 1.0f; // Mass of the each point
	float G = 9.8f; // Gravitational Field

	Vector2f bottomBallPos = Vector2f(960.0f, 540.0f + L0); // Position of the top ball
	Vector2f middleBallPos = Vector2f(1000.0f, 540.0f); // Position of the middle ball
	Vector2f topBallPos = Vector2f(1040.0f, 540.0f - L0); // Position of the top ball

	auto middleVelocity = Vector2f(0.0f, 0.0f); // Velocity of the middle ball
	auto bottomVelocity = Vector2f(0.0f, 0.0f); // Velocity of the bottom ball

	sf::CircleShape topBall(15.f);
	topBall.setFillColor(sf::Color(200, 10, 20));
	topBall.setPosition(topBallPos);

	sf::CircleShape middleBall(15.f);
	middleBall.setFillColor(sf::Color(10, 200, 20));
	middleBall.setPosition(middleBallPos);


	sf::CircleShape bottomBall(15.f);
	bottomBall.setFillColor(sf::Color(10, 20, 200));
	bottomBall.setPosition(bottomBallPos);


	float t = 0.0f;
	float dt = 0.01f;
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "AliPhysicsEngine");

	// Start the game loop
	while (window.isOpen())
	{
		auto L = middleBallPos - topBallPos; // Vector from top ball to middle ball
		auto magnitudeL = sqrt(L.x * L.x + L.y * L.y); // Magnitude of the vector L
		auto Lhat = L / magnitudeL; // Unit vector in the direction of L
		auto s = magnitudeL - L0; // Spring stretch
		auto F_spring = -KS * s * Lhat; // Spring force

		auto L2 = bottomBallPos - middleBallPos; // Vector from middle ball to bottom ball
		auto magnitudeL2 = sqrt(L2.x * L2.x + L2.y * L2.y); // Magnitude of the vector L2
		auto Lhat2 = L2 / magnitudeL2; // Unit vector in the direction of L2
		auto s2 = magnitudeL2 - L0; // Spring stretch for the second spring
		auto F_spring2 = -KS * s2 * Lhat2; // Spring force for the second spring

		// F_gravity = -mass * gravity * vector(0,1,0)
		auto F_gravity = Vector2f(0.0f, M * G); // Gravitational force acting on the middle ball

		auto magnitudeMiddleVel = sqrt(middleVelocity.x * middleVelocity.x + middleVelocity.y * middleVelocity.y); // Magnitude of the velocity of the middle ball
		auto magnitudeBottomVel = sqrt(bottomVelocity.x * bottomVelocity.x + bottomVelocity.y * bottomVelocity.y); // Magnitude of the velocity of the middle ball

		auto energy = 0.5f * M * (magnitudeMiddleVel * magnitudeMiddleVel) + 0.5f * M * (magnitudeBottomVel * magnitudeBottomVel) + 0.5 * KS * (s * s) + 0.5 * KS * (s2 * s2); // Total energy of the system

		auto force = F_spring + F_gravity - F_spring2; // Total force acting on the middle ball
		auto force2 = F_spring2 + F_gravity; // Total force acting on the bottom ball

		middleVelocity = middleVelocity + (force / M * dt); // Update the velocity of the middle ball
		middleBallPos = middleBallPos + middleVelocity * dt; // Update the position of the middle ball

		bottomVelocity = bottomVelocity + (force2 / M * dt); // Update the velocity of the bottom ball
		bottomBallPos = bottomBallPos + bottomVelocity * dt; // Update the position of the bottom ball

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();
		}

		middleBall.setPosition(middleBallPos);
		bottomBall.setPosition(bottomBallPos);


		// Clear screen
		window.clear();

		window.draw(topBall);
		window.draw(middleBall);
		window.draw(bottomBall);


		// Update the window
		window.display();

		t = t + dt; // Increment time
	}

	return EXIT_SUCCESS;
}