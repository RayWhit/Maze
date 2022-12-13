#include "Maze2D.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <stack>
#include <random>


Maze2D::Maze2D(int mazeSize)  : Maze(mazeSize) {
    /*this->mazeSize = mazeSize;
    this->cellSize = 750 / this->mazeSize;
    this->wallSize = this->cellSize * 0.02 + 1;
    this->playerPosition = std::make_pair(4, 4);
    this->rightMargin = 200;
    this->bottomMargin = 50;*/

    this->mazeDimension = 2;
    playerPosition.assign(this->mazeDimension, 0);
    std::vector<std::vector<Cell>> maze(this->mazeSize, std::vector<Cell>(this->mazeSize, Cell()));
    this->maze = maze;
}

void Maze2D::run() {
	// where the maze game is run from
    
    int windowWidth = this->mazeSize * (this->cellSize + this->wallSize) + this->wallSize + this->rightMargin;
    int windowHeight = this->mazeSize * (this->cellSize + this->wallSize) + this->wallSize + this->bottomMargin;
    

    this->window.create(sf::VideoMode(windowWidth, windowHeight, sf::VideoMode::getDesktopMode().bitsPerPixel*10), "2D Maze", sf::Style::Close);
    this->window.clear(sf::Color::White);

    init();

    while (window.isOpen()) {

        sf::Event e;
        while (window.pollEvent(e)) {
            switch (e.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                if (e.key.code == sf::Keyboard::Escape)
                    window.close();
                break;

            case sf::Event::TextEntered:

                // player movement
                if (maze[playerPosition[0]][playerPosition[1]].n && (char)e.text.unicode == 'w') {
                    this->playerPosition[1]--;
                }
                else if (maze[playerPosition[0]][playerPosition[1]].s && (char)e.text.unicode == 's') {
                    this->playerPosition[1]++;
                }
                else if (maze[playerPosition[0]][playerPosition[1]].w && (char)e.text.unicode == 'a') {
                    this->playerPosition[0]--;
                }
                else if (maze[playerPosition[0]][playerPosition[1]].e && (char)e.text.unicode == 'd') {
                    this->playerPosition[0]++;
                }

                // misc buttons
                
                break;
            }
        }

        this->window.clear(sf::Color::White);
        drawMaze();
        drawStartFinish();
        drawPlayer();
        drawStats();
        this->window.display();
    }
}



void Maze2D::init() {
    // generate maze randomly

    srand(time(NULL));
    this->playerPosition.assign(2, 0);
    std::stack<std::pair<int, int>> stack;

    // difficulty A
    //stack.push(std::make_pair(0, 0));
    //this->maze[0][0].visited = true;

    // difficulty B
    //stack.push(std::make_pair(this->mazeSize - 1, this->mazeSize - 1));
    //this->maze[this->mazeSize - 1][this->mazeSize - 1].visited = true;

    // difficulty C
    int randX = rand() % mazeSize;
    int randY = rand() % mazeSize;

    stack.push(std::make_pair(randX, randY));
    this->maze[randX][randY].visited = true;

    int visited = 1;
   

    while(visited != this->mazeSize * this->mazeSize) {
        std::string s;
        int cellCordX = stack.top().first;
        int cellCordY = stack.top().second;

        // looking for legable directions

        if (cellCordX != 0) {
            if (!this->maze[cellCordX - 1][cellCordY].visited) {
                s += 'w';
            }
        }

        if (cellCordX != this->mazeSize - 1) {
            if (!this->maze[cellCordX + 1][cellCordY].visited) {
                s += 'e';
            }
        }

        if (cellCordY != 0) {
            if (!this->maze[cellCordX][cellCordY - 1].visited) {
                s += 'n';
            }
        }

        if (cellCordY != this->mazeSize - 1) {
            if (!this->maze[cellCordX][cellCordY + 1].visited) {
                s += 's';
            }
        }

        // setting cells up
        if (s.size() < 1) {
            stack.pop();
        }
        else {
            int r = rand() % s.size();
            //std::cout << s[r];

            switch (s[r]) {
            case 'n':
                stack.push(std::make_pair(cellCordX, cellCordY - 1));
                this->maze[cellCordX][cellCordY - 1].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY].n = true;
                this->maze[cellCordX][cellCordY - 1].s = true;
                break;

            case 's':
                stack.push(std::make_pair(cellCordX, cellCordY + 1));
                this->maze[cellCordX][cellCordY + 1].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY].s = true;
                this->maze[cellCordX][cellCordY + 1].n = true;
                break;

            case 'w':
                stack.push(std::make_pair(cellCordX - 1, cellCordY));
                this->maze[cellCordX - 1][cellCordY].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY].w = true;
                this->maze[cellCordX - 1][cellCordY].e = true;
                break;

            case 'e':
                stack.push(std::make_pair(cellCordX + 1, cellCordY));
                this->maze[cellCordX + 1][cellCordY].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY].e = true;
                this->maze[cellCordX + 1][cellCordY].w = true;
                break;
            }
        }
    }
}

void Maze2D::drawMaze() {
    // draw full maze

    int mazeWidth = this->cellSize * this->mazeSize + this->wallSize * this->mazeSize + this->wallSize;
    
    // east west outer walls
    sf::RectangleShape wall1(sf::Vector2f(this->wallSize, this->cellSize * this->mazeSize + this->wallSize * this->mazeSize + this->wallSize));
    wall1.setFillColor(sf::Color::Black);
    wall1.setPosition(0, 0);
    this->window.draw(wall1);
    wall1.setPosition(this->mazeSize * this->cellSize + this->mazeSize * this->wallSize, 0);
    this->window.draw(wall1);

    // north south outer walls
    sf::RectangleShape wall2(sf::Vector2f(this->cellSize * this->mazeSize + this->wallSize * this->mazeSize + this->wallSize, this->wallSize));
    wall2.setFillColor(sf::Color::Black);
    wall2.setPosition(0, 0);
    this->window.draw(wall2);
    wall2.setPosition(0, this->mazeSize * this->cellSize + this->mazeSize * this->wallSize);
    this->window.draw(wall2);

    
    // draw maze
    sf::RectangleShape cell(sf::Vector2f(this->cellSize, this->cellSize));
    cell.setFillColor(sf::Color::White);

    for (int y = 0; y < this->mazeSize; y++) {
        for (int x = 0; x < this->mazeSize; x++) {
            cell.setPosition(x * this->cellSize + x * this->wallSize + this->wallSize, y * this->cellSize + y * this->wallSize + this->wallSize);
            this->window.draw(cell);
            
            // southern wall for each cell
            if (!this->maze[x][y].s) {
                sf::RectangleShape wall(sf::Vector2f(this->cellSize + 2*this->wallSize, this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(cell.getPosition().x - this->wallSize, cell.getPosition().y + this->cellSize);
                this->window.draw(wall);
            }

            // northern wall for each cell
            if (!this->maze[x][y].e) {
                sf::RectangleShape wall(sf::Vector2f(this->wallSize, this->cellSize + this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(cell.getPosition().x + this->cellSize, cell.getPosition().y);
                this->window.draw(wall);
            }
        }
    }

    // XY
    // X
    wall1.setSize(sf::Vector2f(this->cellSize / 2, 2 * this->wallSize));
    wall1.setFillColor(sf::Color::Red);
    wall1.setPosition(cellSize / 8, mazeWidth - cellSize / 8);
    this->window.draw(wall1);

    // Y
    wall1.setSize(sf::Vector2f(2 * this->wallSize, this->cellSize / 2));
    wall1.setFillColor(sf::Color::Green);
    wall1.setPosition(cellSize / 8, mazeWidth - cellSize / 8 - this->cellSize / 2);
    this->window.draw(wall1);
}

void Maze2D::drawStartFinish() {
    // draw start finish circles

    sf::CircleShape start(this->cellSize / 3);
    // draw start
    start.setPosition(sf::Vector2f(this->wallSize + this->cellSize/6, this->wallSize + this->cellSize / 6));
    start.setFillColor(sf::Color::Red);
    window.draw(start);

    //draw finish
    sf::CircleShape finish(this->cellSize / 3);
    finish.setPosition(sf::Vector2f(this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6,
                                    this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6));
    finish.setFillColor(sf::Color::Green);
    window.draw(finish);
}

void Maze2D::drawPlayer() {
    // draw player

    sf::CircleShape player(this->cellSize / 3);
    player.setPosition(sf::Vector2f(this->cellSize * (this->playerPosition[0]) + this->wallSize * (this->playerPosition[0] + 1) + this->cellSize / 6,
                                    this->cellSize * (this->playerPosition[1]) + this->wallSize * (this->playerPosition[1] + 1) + this->cellSize / 6));
    player.setFillColor(sf::Color::Blue);
    window.draw(player);
}

void Maze2D::drawStats() {
    // draw stats and text to console

    sf::Font font;
    font.loadFromFile("ARIAL.TTF");
    if (!font.loadFromFile("ARIAL.TTF"))
    {
        std::cout << "Font loading error";
    }

    // draw x cord
    sf::Text text("X", font, this->rightMargin / 6);
    text.setPosition(this->mazeSize * (this->cellSize + this->wallSize) + this->rightMargin / 10, 0);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);
    window.draw(text);

    text.setString(std::to_string(playerPosition[0]));
    if (playerPosition[0] < 10) {
        text.setPosition(this->mazeSize * (this->cellSize + this->wallSize) + this->rightMargin / 10, text.getCharacterSize());
    }
    else {
        text.setPosition(this->mazeSize * (this->cellSize + this->wallSize) + this->rightMargin / 10 - text.getCharacterSize() / 4, text.getCharacterSize());
    }
    window.draw(text);
    

    // y cord
    text.setString("Y");
    text.setFillColor(sf::Color::Green);
    text.setPosition(this->mazeSize * (this->cellSize + this->wallSize) + 3*this->rightMargin / 10, 0);
    window.draw(text);

    text.setString(std::to_string(playerPosition[1]));
    if (playerPosition[1] < 10) {
        text.setPosition(this->mazeSize * (this->cellSize + this->wallSize) + 3*this->rightMargin / 10, text.getCharacterSize());
    }
    else {
        text.setPosition(this->mazeSize * (this->cellSize + this->wallSize) + 3*this->rightMargin / 10 - text.getCharacterSize() / 4, text.getCharacterSize());
    }
    window.draw(text);

    // win screen
    if (playerPosition[0] == mazeSize - 1 && playerPosition[1] == mazeSize - 1) {
        text.setFillColor(sf::Color::Red);
        text.setCharacterSize(this->mazeSize * cellSize / 5);
        text.setPosition(this->mazeSize * this->cellSize / 10, this->mazeSize * this->cellSize / 4);
        text.setString("You Win!");
        window.draw(text);
    }
}

Maze2D::~Maze2D() {

}
