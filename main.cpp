#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 100;
const int BALL_SIZE = 20;
const float BALL_SPEED = 5.0f;
const float PADDLE_SPEED = 5.0f;
const int GOAL_HEIGHT = 200;


int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Pong");
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(
        (desktop.size.x - WINDOW_WIDTH) / 2,
        (desktop.size.y - WINDOW_HEIGHT) / 2
    ));

    window.setVerticalSyncEnabled(true);

    sf::Shader fxaaShader;
    if (!fxaaShader.loadFromFile("fxaa.frag", sf::Shader::Type::Fragment)) {
    }
    sf::RenderTexture renderTexture(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));

    sf::RectangleShape leftGoal(sf::Vector2f(5, GOAL_HEIGHT));
    leftGoal.setFillColor(sf::Color::Red);
    leftGoal.setPosition(sf::Vector2f(0, (WINDOW_HEIGHT - GOAL_HEIGHT) / 2));

    sf::RectangleShape rightGoal(sf::Vector2f(5, GOAL_HEIGHT));
    rightGoal.setFillColor(sf::Color::Red);
    rightGoal.setPosition(sf::Vector2f(WINDOW_WIDTH - 5, (WINDOW_HEIGHT - GOAL_HEIGHT) / 2));

    sf::RectangleShape leftPaddle(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    leftPaddle.setFillColor(sf::Color::White);
    leftPaddle.setPosition(sf::Vector2f(50, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2));

    sf::RectangleShape rightPaddle(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    rightPaddle.setFillColor(sf::Color::White);
    rightPaddle.setPosition(sf::Vector2f(WINDOW_WIDTH - 50 - PADDLE_WIDTH, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2));

    sf::CircleShape ball(BALL_SIZE / 2);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2));
    sf::Vector2f ballVelocity(BALL_SPEED, BALL_SPEED);

    int leftScore = 0;
    int rightScore = 0;
    sf::Font font;
    if (!font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        // Fallback
    }
    sf::Text scoreText(font, to_string(leftScore) + " - " + to_string(rightScore), 30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - 50, 10));

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && leftPaddle.getPosition().y > 0) {
            leftPaddle.move(sf::Vector2f(0, -PADDLE_SPEED));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && leftPaddle.getPosition().y < WINDOW_HEIGHT - PADDLE_HEIGHT) {
            leftPaddle.move(sf::Vector2f(0, PADDLE_SPEED));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && rightPaddle.getPosition().y > 0) {
            rightPaddle.move(sf::Vector2f(0, -PADDLE_SPEED));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && rightPaddle.getPosition().y < WINDOW_HEIGHT - PADDLE_HEIGHT) {
            rightPaddle.move(sf::Vector2f(0, PADDLE_SPEED));
        }

        // Update ball
        ball.move(ballVelocity);

        
        if (ball.getPosition().y <= 0 || ball.getPosition().y >= WINDOW_HEIGHT - BALL_SIZE) {
            ballVelocity.y = -ballVelocity.y;
        }

        // Ball collision with paddles
        if (ball.getGlobalBounds().findIntersection(leftPaddle.getGlobalBounds())) {
            ballVelocity.x = BALL_SPEED;
        }
        if (ball.getGlobalBounds().findIntersection(rightPaddle.getGlobalBounds())) {
            ballVelocity.x = -BALL_SPEED;
        }

        // Score
        float goalTop = (WINDOW_HEIGHT - GOAL_HEIGHT) / 2;
        float goalBottom = goalTop + GOAL_HEIGHT;
        if (ball.getPosition().x < 0 && ball.getPosition().y >= goalTop && ball.getPosition().y + BALL_SIZE <= goalBottom) {
            rightScore++;
            ball.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2));
            ballVelocity = sf::Vector2f(BALL_SPEED, BALL_SPEED);
        }
        else if (ball.getPosition().x < -BALL_SIZE) {
            ball.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2));
            ballVelocity = sf::Vector2f(BALL_SPEED, BALL_SPEED);
        }
        if (ball.getPosition().x > WINDOW_WIDTH - BALL_SIZE && ball.getPosition().y >= goalTop && ball.getPosition().y + BALL_SIZE <= goalBottom) {
            leftScore++;
            ball.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2));
            ballVelocity = sf::Vector2f(-BALL_SPEED, BALL_SPEED);
        }
        else if (ball.getPosition().x > WINDOW_WIDTH) {
            ball.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2));
            ballVelocity = sf::Vector2f(-BALL_SPEED, BALL_SPEED);
        }

        // Update score text
        scoreText.setString(to_string(leftScore) + " - " + to_string(rightScore));

        // Draw to render texture
        renderTexture.clear(sf::Color::Black);
        renderTexture.draw(leftGoal);
        renderTexture.draw(rightGoal);
        renderTexture.draw(leftPaddle);
        renderTexture.draw(rightPaddle);
        renderTexture.draw(ball);
        renderTexture.draw(scoreText);
        renderTexture.display();

        // Apply FXAA and draw to window
        window.clear();
        sf::Sprite sprite(renderTexture.getTexture());
        fxaaShader.setUniform("texture", renderTexture.getTexture());
        fxaaShader.setUniform("resolution", sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        window.draw(sprite, &fxaaShader);
        window.display();

        // Frame rate
        sf::sleep(sf::milliseconds(16)); // ~60 FPS
    }

    return 0;
}
