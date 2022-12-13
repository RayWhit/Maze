#include "Maze3D.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <stack>
#include <random>

Maze3D::Maze3D(int mazeSize) : Maze(mazeSize) {
    this->cellSize /= 2;
    this->wallSize = this->cellSize * 0.02 + 1;
    this->mazeDimension = 3;
    playerPosition.assign(this->mazeDimension, 0);

    // create 3d vector maze
    std::vector<std::vector<std::vector<Cell>>> maze(this->mazeSize, std::vector<std::vector<Cell>>(this->mazeSize, std::vector<Cell>(this->mazeSize)));
    this->maze = maze;
}

void Maze3D::run() {
    // where the maze game is run from

    int windowWidth = 2 * (this->mazeSize * (this->cellSize + this->wallSize) + this->wallSize) + this->rightMargin;
    int windowHeight = 2 * (this->mazeSize * (this->cellSize + this->wallSize) + this->wallSize) + this->bottomMargin;

    this->window.create(sf::VideoMode(windowWidth, windowHeight, sf::VideoMode::getDesktopMode().bitsPerPixel * 10), "3D Maze", sf::Style::Close);
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

                if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]].n && (char)e.text.unicode == 'w') {
                    this->playerPosition[1]--;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]].s && (char)e.text.unicode == 's') {
                    this->playerPosition[1]++;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]].w && (char)e.text.unicode == 'a') {
                    this->playerPosition[0]--;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]].e && (char)e.text.unicode == 'd') {
                    this->playerPosition[0]++;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]].u && (char)e.text.unicode == 'k') {
                    this->playerPosition[2]++;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]].d && (char)e.text.unicode == 'i') {
                    this->playerPosition[2]--;
                }
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

void Maze3D::init() {
    // generate maze randomly

    srand(time(NULL));

    std::stack<std::vector<int>> stack;
    std::vector<int> pomV(this->mazeDimension, 0);
    stack.push(pomV);
    this->maze[0][0][0].visited = true;
    int visited = 1;

    while (visited != this->mazeSize * this->mazeSize * this->mazeSize) {
        std::string s;
        int cellCordX = stack.top()[0];
        int cellCordY = stack.top()[1];
        int cellCordZ = stack.top()[2];

        // looking for legable directions
        if (!(cellCordX == 0)) {
            if (!this->maze[cellCordX - 1][cellCordY][cellCordZ].visited) {
                s += 'w';
            }
        }
        if (!(cellCordX == this->mazeSize - 1)) {
            if (!this->maze[cellCordX + 1][cellCordY][cellCordZ].visited) {
                s += 'e';
            }
        }

        if (!(cellCordY == 0)) {
            if (!this->maze[cellCordX][cellCordY - 1][cellCordZ].visited) {
                s += 'n';
            }
        }
        if (!(cellCordY == this->mazeSize - 1)) {
            if (!this->maze[cellCordX][cellCordY + 1][cellCordZ].visited) {
                s += 's';
            }
        }

        if (!(cellCordZ == 0)) {
            if (!this->maze[cellCordX][cellCordY][cellCordZ - 1].visited) {
                s += 'd';
            }
        }
        if (!(cellCordZ == this->mazeSize - 1)) {
            if (!this->maze[cellCordX][cellCordY][cellCordZ + 1].visited) {
                s += 'u';
            }
        }

        // setting cells up
        if (s.size() < 1) {
            // if no direction is available pop back to previous position
            stack.pop();
        }
        else {
            // randomizing maze generation
            int r = rand() % s.size();

            switch (s[r]) {
            case 'n':
                pomV[0] = cellCordX;
                pomV[1] = cellCordY - 1;
                pomV[2] = cellCordZ;

                stack.push(pomV);
                this->maze[cellCordX][cellCordY - 1][cellCordZ].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ].n = true;
                this->maze[cellCordX][cellCordY - 1][cellCordZ].s = true;
                break;

            case 's':
                pomV[0] = cellCordX;
                pomV[1] = cellCordY + 1;
                pomV[2] = cellCordZ;

                stack.push(pomV);
                this->maze[cellCordX][cellCordY + 1][cellCordZ].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ].s = true;
                this->maze[cellCordX][cellCordY + 1][cellCordZ].n = true;
                break;

            case 'w':
                pomV[0] = cellCordX - 1;
                pomV[1] = cellCordY;
                pomV[2] = cellCordZ;

                stack.push(pomV);
                this->maze[cellCordX - 1][cellCordY][cellCordZ].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ].w = true;
                this->maze[cellCordX - 1][cellCordY][cellCordZ].e = true;
                break;

            case 'e':
                pomV[0] = cellCordX + 1;
                pomV[1] = cellCordY;
                pomV[2] = cellCordZ;

                stack.push(pomV);
                this->maze[cellCordX + 1][cellCordY][cellCordZ].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ].e = true;
                this->maze[cellCordX + 1][cellCordY][cellCordZ].w = true;
                break;

            case 'u':
                pomV[0] = cellCordX;
                pomV[1] = cellCordY;
                pomV[2] = cellCordZ + 1;

                stack.push(pomV);
                this->maze[cellCordX][cellCordY][cellCordZ + 1].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ].u = true;
                this->maze[cellCordX][cellCordY][cellCordZ + 1].d = true;
                break;

            case 'd':
                pomV[0] = cellCordX;
                pomV[1] = cellCordY;
                pomV[2] = cellCordZ - 1;

                stack.push(pomV);
                this->maze[cellCordX][cellCordY][cellCordZ - 1].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ].d = true;
                this->maze[cellCordX][cellCordY][cellCordZ - 1].u = true;
                break;
            }
        }
    }
}

void Maze3D::drawMaze() {
    // draw maze
    // only top and left outer walls are generated
    // while printing cells only right and bottom walls are generated
    
    int mazeWidth = this->cellSize * this->mazeSize + this->wallSize * this->mazeSize + this->wallSize;

    //first board
    // west outer walls
    sf::RectangleShape wall(sf::Vector2f(this->wallSize, mazeWidth));
    wall.setFillColor(sf::Color::Black);
    wall.setPosition(0, 0);
    this->window.draw(wall);

    // north outer walls
    wall.setSize(sf::Vector2f(mazeWidth, this->wallSize));
    wall.setFillColor(sf::Color::Black);
    wall.setPosition(0, 0);
    this->window.draw(wall);


    //second board
    // west outer walls
    wall.setSize(sf::Vector2f(this->wallSize, mazeWidth));
    wall.setFillColor(sf::Color::Black);
    wall.setPosition(mazeWidth, 0);
    this->window.draw(wall);

    // north outer walls
    wall.setSize(sf::Vector2f(mazeWidth, this->wallSize));
    wall.setFillColor(sf::Color::Black);
    wall.setPosition(mazeWidth, 0);
    this->window.draw(wall);


    //third board
    // west outer walls
    wall.setSize(sf::Vector2f(this->wallSize, mazeWidth));
    wall.setFillColor(sf::Color::Black);
    wall.setPosition(mazeWidth / 2, mazeWidth);
    this->window.draw(wall);

    // north outer walls
    wall.setSize(sf::Vector2f(mazeWidth, this->wallSize));
    wall.setFillColor(sf::Color::Black);
    wall.setPosition(mazeWidth / 2, mazeWidth);
    this->window.draw(wall);


    // draw maze cell by cell
    sf::RectangleShape cell(sf::Vector2f(this->cellSize, this->cellSize));
    cell.setFillColor(sf::Color::White);

    // first maze, XY (standard cord system)
    for (int y = 0; y < this->mazeSize; y++) {
        for (int x = 0; x < this->mazeSize; x++) {
            cell.setPosition(x * this->cellSize + x * this->wallSize + this->wallSize, y * this->cellSize + y * this->wallSize + this->wallSize);
            this->window.draw(cell);

            // southern wall for each cell
            if (!this->maze[x][y][this->playerPosition[2]].s) {
                sf::RectangleShape wall(sf::Vector2f(this->cellSize + 2 * this->wallSize, this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(cell.getPosition().x - this->wallSize, cell.getPosition().y + this->cellSize);
                this->window.draw(wall);
            }

            // eastern wall for each cell
            if (!this->maze[x][y][this->playerPosition[2]].e) {
                sf::RectangleShape wall(sf::Vector2f(this->wallSize, this->cellSize + this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(cell.getPosition().x + this->cellSize, cell.getPosition().y);
                this->window.draw(wall);
            }
        }
    }

    // second maze, XZ
    for (int z = 0; z < this->mazeSize; z++) {
        for (int x = 0; x < this->mazeSize; x++) {
            cell.setPosition(x * this->cellSize + x * this->wallSize + this->wallSize, z * this->cellSize + z * this->wallSize + this->wallSize);
            this->window.draw(cell);

            // southern wall for each cell
            if (!this->maze[x][this->playerPosition[1]][z].u) {
                sf::RectangleShape wall(sf::Vector2f(this->cellSize + 2 * this->wallSize, this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(mazeWidth + cell.getPosition().x - this->wallSize, cell.getPosition().y + this->cellSize);
                this->window.draw(wall);
            }

            // northern wall for each cell
            if (!this->maze[x][this->playerPosition[1]][z].e) {
                sf::RectangleShape wall(sf::Vector2f(this->wallSize, this->cellSize + this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(mazeWidth + cell.getPosition().x + this->cellSize, cell.getPosition().y);
                this->window.draw(wall);
            }
        }
    }

    // third maze, ZY
    for (int y = 0; y < this->mazeSize; y++) {
        for (int z = 0; z < this->mazeSize; z++) {
            cell.setPosition(z * this->cellSize + z * this->wallSize + this->wallSize, y * this->cellSize + y * this->wallSize + this->wallSize);
            this->window.draw(cell);

            // southern wall for each cell
            if (!this->maze[this->playerPosition[0]][y][z].s) {
                sf::RectangleShape wall(sf::Vector2f(this->cellSize + 2 * this->wallSize, this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(mazeWidth / 2 + cell.getPosition().x - this->wallSize, mazeWidth + cell.getPosition().y + this->cellSize);
                this->window.draw(wall);
            }

            // northern wall for each cell
            if (!this->maze[this->playerPosition[0]][y][z].u) {
                sf::RectangleShape wall(sf::Vector2f(this->wallSize, this->cellSize + this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(mazeWidth / 2 + cell.getPosition().x + this->cellSize, mazeWidth + cell.getPosition().y);
                this->window.draw(wall);
            }
        }
    }
   
    // draw axes for each plane
    // XY
    // X
    wall.setSize(sf::Vector2f(this->cellSize / 2, 2 * this->wallSize));
    wall.setFillColor(sf::Color::Red);
    wall.setPosition(cellSize / 8, mazeWidth - cellSize / 8);
    this->window.draw(wall);

    // Y
    wall.setSize(sf::Vector2f(2 * this->wallSize, this->cellSize / 2));
    wall.setFillColor(sf::Color::Green);
    wall.setPosition(cellSize / 8, mazeWidth - cellSize / 8 - this->cellSize/2);
    this->window.draw(wall);


    // XZ
    // X
    wall.setSize(sf::Vector2f(this->cellSize / 2, 2 * this->wallSize));
    wall.setFillColor(sf::Color::Red);
    wall.setPosition(mazeWidth + cellSize / 8, mazeWidth - cellSize / 8);
    this->window.draw(wall);

    // Z
    wall.setSize(sf::Vector2f(2 * this->wallSize, this->cellSize / 2));
    wall.setFillColor(sf::Color::Blue);
    wall.setPosition(mazeWidth + cellSize / 8, mazeWidth - cellSize / 8 - this->cellSize / 2);
    this->window.draw(wall);


    // YZ
    // Y
    wall.setSize(sf::Vector2f(2 * this->wallSize, this->cellSize / 2));
    wall.setFillColor(sf::Color::Green);
    wall.setPosition(mazeWidth / 2 + cellSize / 8, mazeWidth + mazeWidth - cellSize / 8 - this->cellSize / 2);
    this->window.draw(wall);

    // Z
    wall.setSize(sf::Vector2f(this->cellSize / 2, 2 * this->wallSize));
    wall.setFillColor(sf::Color::Blue);
    wall.setPosition(mazeWidth / 2 + cellSize / 8, mazeWidth + mazeWidth - cellSize / 8);
    this->window.draw(wall);
}

void Maze3D::drawStartFinish() {
    // draw start finish circles

    int mazeWidth = this->cellSize * this->mazeSize + this->wallSize * this->mazeSize + this->wallSize;
    sf::CircleShape circle(this->cellSize / 3);

    // ***************** start *************************
    circle.setFillColor(sf::Color::Red);
    
    // start in XY plane
    if (this->playerPosition[2] == 0) {
        circle.setPosition(sf::Vector2f(this->wallSize + this->cellSize / 6, this->wallSize + this->cellSize / 6));
        window.draw(circle);
    }

    // start in XZ plane
    if (this->playerPosition[1] == 0) {
        circle.setPosition(sf::Vector2f(mazeWidth + this->wallSize + this->cellSize / 6, this->wallSize + this->cellSize / 6));
        window.draw(circle);
    }

    // start in YZ plane
    if (this->playerPosition[0] == 0) {
        circle.setPosition(sf::Vector2f(mazeWidth / 2 + this->wallSize + this->cellSize / 6, mazeWidth + this->wallSize + this->cellSize / 6));
        window.draw(circle);
    }

    // **************** finish *********************
    circle.setFillColor(sf::Color::Green);

    // finish in XY plane
    if (this->playerPosition[2] == this->mazeSize - 1) {
        circle.setPosition(sf::Vector2f(this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6,
                                        this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6));
        window.draw(circle);
    }

    // finish in XZ plane
    if (this->playerPosition[1] == this->mazeSize - 1) {
        circle.setPosition(sf::Vector2f(mazeWidth + this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6,
                                        this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6));
        window.draw(circle);
    }

    // finish in YZ plane
    if (this->playerPosition[0] == this->mazeSize - 1) {
        circle.setPosition(sf::Vector2f(mazeWidth / 2 + this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6,
                                        mazeWidth + this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6));
        window.draw(circle);
    }
}

void Maze3D::drawPlayer() {
    // draw player 

    int mazeWidth = this->cellSize * this->mazeSize + this->wallSize * this->mazeSize + this->wallSize;
    sf::CircleShape player(this->cellSize / 3);
    player.setFillColor(sf::Color::Blue);

    // XY plane
    player.setPosition(sf::Vector2f(this->cellSize * (this->playerPosition[0]) + this->wallSize * (this->playerPosition[0] + 1) + this->cellSize / 6,
                                    this->cellSize * (this->playerPosition[1]) + this->wallSize * (this->playerPosition[1] + 1) + this->cellSize / 6));
    window.draw(player);

    // XZ plane
    player.setPosition(sf::Vector2f(mazeWidth + this->cellSize * (this->playerPosition[0]) + this->wallSize * (this->playerPosition[0] + 1) + this->cellSize / 6,
                                    this->cellSize * (this->playerPosition[2]) + this->wallSize * (this->playerPosition[2] + 1) + this->cellSize / 6));
    window.draw(player);

    // ZY plane
    player.setPosition(sf::Vector2f(mazeWidth / 2 + this->cellSize * (this->playerPosition[2]) + this->wallSize * (this->playerPosition[2] + 1) + this->cellSize / 6,
                                    mazeWidth + this->cellSize * (this->playerPosition[1]) + this->wallSize * (this->playerPosition[1] + 1) + this->cellSize / 6));
    window.draw(player);
}

void Maze3D::drawStats() {
    // draw stats - player position

    int mazeWidth = this->cellSize * this->mazeSize + this->wallSize * this->mazeSize + this->wallSize;

    sf::Font font;
    font.loadFromFile("ARIAL.TTF");
    if (!font.loadFromFile("ARIAL.TTF")){
        std::cout << "Font loading error";
    }

    // draw x cord
    sf::Text text("X", font, this->rightMargin / 6);
    text.setPosition(2 * mazeWidth + this->rightMargin / 10, 0);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);
    window.draw(text);

    text.setString(std::to_string(playerPosition[0]));
    if (playerPosition[0] < 10) {
        text.setPosition(2 * mazeWidth + this->rightMargin / 10, text.getCharacterSize());
    }
    else {
        text.setPosition(2 * mazeWidth + this->rightMargin / 10 - text.getCharacterSize() / 4, text.getCharacterSize());
    }
    window.draw(text);


    // y cord
    text.setString("Y");
    text.setFillColor(sf::Color::Green);
    text.setPosition(2 * mazeWidth + 3 * this->rightMargin / 10, 0);
    window.draw(text);

    text.setString(std::to_string(playerPosition[1]));
    if (playerPosition[1] < 10) {
        text.setPosition(2 * mazeWidth + 3 * this->rightMargin / 10, text.getCharacterSize());
    }
    else {
        text.setPosition(2 * mazeWidth + 3 * this->rightMargin / 10 - text.getCharacterSize() / 4, text.getCharacterSize());
    }
    window.draw(text);
    

    // z cord
    text.setString("Z");
    text.setFillColor(sf::Color::Blue);
    text.setPosition(2 * mazeWidth + 10*this->rightMargin / 20, 0);
    window.draw(text);

    text.setString(std::to_string(playerPosition[2]));
    if (playerPosition[2] < 10) {
        text.setPosition(2 * mazeWidth + 10*this->rightMargin / 20, text.getCharacterSize());
    }
    else {
        text.setPosition(2 * mazeWidth + 10*this->rightMargin / 20 - text.getCharacterSize() / 4, text.getCharacterSize());
    }
    window.draw(text);


    // win screen
    if (playerPosition[0] == mazeSize - 1 && playerPosition[1] == mazeSize - 1 && playerPosition[2] == mazeSize - 1) {
        text.setFillColor(sf::Color::Red);
        text.setCharacterSize(this->windowWidth / 5);
        text.setPosition(this->mazeSize * this->cellSize / 10, this->mazeSize * this->cellSize / 4);
        text.setString("You Win!");
        window.draw(text);
    }
}