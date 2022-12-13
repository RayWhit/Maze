#include "Maze.h"
#include "Maze2D.h"
#include "Maze3D.h"
#include "Maze4D.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <stack>
#include <random>



Maze::Maze(int mazeSize) {
    this->mazeSize = mazeSize;
    this->cellSize = 750 / this->mazeSize;
    this->wallSize = this->cellSize * 0.02 + 1;
    this->playerPosition;
    this->rightMargin = 200;
    this->bottomMargin = 50;
    this->mazeDimension = 0;

    this->windowWidth = this->mazeSize * (this->cellSize + this->wallSize) + this->wallSize + this->rightMargin;
    this->windowHeight = this->mazeSize * (this->cellSize + this->wallSize) + this->wallSize + this->bottomMargin;
}

void Maze::run() {
    this->window.create(sf::VideoMode(1080, 720, sf::VideoMode::getDesktopMode().bitsPerPixel * 10), "2D Maze", sf::Style::Close);
    this->window.clear(sf::Color::White);

    // text creation
    sf::Font font;
    font.loadFromFile("ARIAL.TTF");
    if (!font.loadFromFile("ARIAL.TTF"))
    {
        std::cout << "Font loading error";
    }

    sf::Text text("", font);
    text.setString(
        "Welcome to Ray's maze\n"
        "To play select the number of dimensions for your maze \n"
        "(2, 3, 4)"
        /*"\n\n\n\n\n"
        "Set the maze up:   Dimensions      Size\n\n"
        "\t\t\t\t\t\t\t\t\t\t+  \t           +"
    
        */
    
    );

    text.setPosition(0, 0);
    text.setFillColor(sf::Color::Black);

    /*
    // button creation
    sf::RectangleShape plusButton1(sf::Vector2f(50, 50));
    plusButton1.setOutlineColor(sf::Color::Black);
    plusButton1.setOutlineThickness(2);
    plusButton1.setPosition(304, 311);

    sf::RectangleShape plusButton2(sf::Vector2f(50, 50));
    plusButton2.setOutlineColor(sf::Color::Black);
    plusButton2.setOutlineThickness(2);
    plusButton2.setPosition(458, 311);
    */



    while (window.isOpen()) {

        sf::Event e;
        while (window.pollEvent(e)) {
            switch (e.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::TextEntered:
                if ((char)e.text.unicode == '2') {
                    this->window.close();
                    Maze2D maze2D(this->mazeSize);
                    maze2D.run();
                    this->window.create(sf::VideoMode(1080, 720, sf::VideoMode::getDesktopMode().bitsPerPixel * 10), "2D Maze", sf::Style::Close);
                }
                else if ((char)e.text.unicode == '3') {
                    this->window.close();
                    Maze3D maze3D(this->mazeSize);
                    maze3D.run();
                    this->window.create(sf::VideoMode(1080, 720, sf::VideoMode::getDesktopMode().bitsPerPixel * 10), "2D Maze", sf::Style::Close);
                }
                else if ((char)e.text.unicode == '4') {
                    this->window.close();
                    Maze4D maze4D(this->mazeSize);
                    maze4D.run();
                    this->window.create(sf::VideoMode(1080, 720, sf::VideoMode::getDesktopMode().bitsPerPixel * 10), "2D Maze", sf::Style::Close);
                }

                break;


            }
        }




        this->window.clear(sf::Color::White);

       
        
        //this->window.draw(plusButton1);
        //this->window.draw(plusButton2);
        //this->window.draw(plusButton1);
        //this->window.draw(plusButton1);
        this->window.draw(text);


        this->window.display();
    }
}

Maze::~Maze() {

}
