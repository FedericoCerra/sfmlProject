#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
#include <optional>

// Struttura per rappresentare un oggetto rimbalzante
struct BouncingObject {
    sf::CircleShape shape;
    sf::Vector2f velocity;

    BouncingObject(float radius, sf::Vector2f position, sf::Vector2f velocity, sf::Color color)
        : velocity(velocity) {
        shape.setRadius(radius);
        shape.setPosition(position);
        shape.setFillColor(color);
        shape.setOrigin(sf::Vector2f(radius, radius)); // Imposta l'origine al centro
    }

    void update(float deltaTime, const sf::Vector2u& windowSize) {
        // Muovi l'oggetto
        shape.move(velocity * deltaTime);

        // Rimbalza ai bordi della finestra
        sf::Vector2f position = shape.getPosition();
        float radius = shape.getRadius();

        if (position.x - radius < 0 || position.x + radius > windowSize.x) {
            velocity.x = -velocity.x;
        }
        if (position.y - radius < 0 || position.y + radius > windowSize.y) {
            velocity.y = -velocity.y;
        }
    }
};
 
int main() {
    // Creazione della finestra
    sf::RenderWindow window;
    window.create(sf::VideoMode(sf::Vector2u(1920u, 1080u)), "SFML Crazy Program");
    window.setFramerateLimit(60);

    // Font per il testo
    sf::Font font;
    if (!font.openFromFile("opensans.ttf")) {
        std::cerr << "Errore: impossibile caricare il font." << std::endl;
        return 1;
    }

    // Testo per mostrare il numero di oggetti
    sf::Text objectCountText(font, sf::String(""), 24); // Ordine corretto: font, stringa, dimensione
    objectCountText.setFillColor(sf::Color(255, 255, 255)); // Bianco
    objectCountText.setPosition(sf::Vector2f(10.f, 10.f));

    // Vettore di oggetti rimbalzanti
    std::vector<BouncingObject> bouncingObjects;

    // Generatore di numeri casuali
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> radiusDist(20.f, 50.f);
    std::uniform_real_distribution<float> velocityDist(-200.f, 200.f);
    std::uniform_int_distribution<int> colorDist(0, 255);

    // Clock per il tempo
    sf::Clock clock;

    // Loop principale
    while (window.isOpen()) {
        // Tempo trascorso dall'ultimo frame
        float deltaTime = clock.restart().asSeconds();

        // Gestione degli eventi
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>()) {
                // Aggiungi un nuovo oggetto rimbalzante al clic del mouse
                float radius = radiusDist(gen);
                sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                sf::Vector2f velocity(velocityDist(gen), velocityDist(gen));
                sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));

                bouncingObjects.emplace_back(radius, position, velocity, color);
                bouncingObjects.emplace_back(radius, position, -velocity, color);

            }
        }

        // Cancella la finestra con un colore di sfondo
        window.clear(sf::Color(0, 0, 0)); // Sfondo nero

        // Aggiorna e disegna gli oggetti rimbalzanti
        for (auto& obj : bouncingObjects) {
            obj.update(deltaTime, window.getSize());
            window.draw(obj.shape);
        }

        // Aggiorna il testo con il numero di oggetti
        objectCountText.setString("Oggetti: " + std::to_string(bouncingObjects.size()));
        window.draw(objectCountText);

        // Mostra tutto
        window.display();
    }

    return 0;
}