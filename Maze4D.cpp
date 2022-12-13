#include "Maze4D.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <stack>
#include <random>


Maze4D::Maze4D(int mazeSize) : Maze(mazeSize) {
    this->cellSize /= 2;
    this->wallSize = this->cellSize * 0.02 + 1;

    this->mazeDimension = 4;
    playerPosition.assign(this->mazeDimension, 0);
    
    /*playerPosition[0] = this->mazeSize - 1;
    playerPosition[1] = this->mazeSize - 1;
    playerPosition[2] = this->mazeSize - 1;
    playerPosition[3] = this->mazeSize - 1;*/

    // create 4d vector maze
    std::vector<std::vector<std::vector<std::vector<Cell>>>> maze(this->mazeSize, 
        std::vector<std::vector<std::vector<Cell>>>(this->mazeSize, 
            std::vector<std::vector<Cell>>(this->mazeSize, 
                std::vector<Cell>(this->mazeSize))));

    this->maze = maze;
}

void Maze4D::run() {
    int windowWidth = 2 * (this->mazeSize * (this->cellSize + this->wallSize) + this->wallSize) + this->rightMargin;
    int windowHeight = 2 * (this->mazeSize * (this->cellSize + this->wallSize) + this->wallSize) + this->bottomMargin;


    this->window.create(sf::VideoMode(windowWidth, windowHeight, sf::VideoMode::getDesktopMode().bitsPerPixel * 10), "2D Maze", sf::Style::Close);
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
                if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]][playerPosition[3]].n && (char)e.text.unicode == 'w') {
                    this->playerPosition[1]--;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]][playerPosition[3]].s && (char)e.text.unicode == 's') {
                    this->playerPosition[1]++;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]][playerPosition[3]].w && (char)e.text.unicode == 'a') {
                    this->playerPosition[0]--;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]][playerPosition[3]].e && (char)e.text.unicode == 'd') {
                    this->playerPosition[0]++;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]][playerPosition[3]].u && (char)e.text.unicode == 'k') {
                    this->playerPosition[2]++;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]][playerPosition[3]].d && (char)e.text.unicode == 'i') {
                    this->playerPosition[2]--;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]][playerPosition[3]].a && (char)e.text.unicode == 'l') {
                    this->playerPosition[3]++;
                }
                else if (maze[playerPosition[0]][playerPosition[1]][playerPosition[2]][playerPosition[3]].k && (char)e.text.unicode == 'j') {
                    this->playerPosition[3]--;
                }
                break;
            }
            //std::cout << "[" << playerPosition[0] << ", " << playerPosition[1] << ", " << playerPosition[2] << "]" << std::endl;
        }

        this->window.clear(sf::Color::White);
        drawMaze();
        drawStartFinish();
        drawPlayer();
        drawStats();
        this->window.display();
    }
}

void Maze4D::init() {

    // generate maze randomly

    srand(time(NULL));

    std::stack<std::vector<int>> stack;
    std::vector<int> pomV(this->mazeDimension, 0);

    stack.push(pomV);
    this->maze[0][0][0][0].visited = true;

    int visited = 1;
    

    while (visited != this->mazeSize * this->mazeSize * this->mazeSize * this->mazeSize) {
        std::string s;
        int cellCordX = stack.top()[0];
        int cellCordY = stack.top()[1];
        int cellCordZ = stack.top()[2];
        int cellCordW = stack.top()[3];

        // looking for legable directions
        
        if (!(cellCordX == 0)) {
            if (!this->maze[cellCordX - 1][cellCordY][cellCordZ][cellCordW].visited) {
                s += 'w';
            }
        }
        if (!(cellCordX == this->mazeSize - 1)) {
            if (!this->maze[cellCordX + 1][cellCordY][cellCordZ][cellCordW].visited) {
                s += 'e';
            }
        }

        if (!(cellCordY == 0)) {
            if (!this->maze[cellCordX][cellCordY - 1][cellCordZ][cellCordW].visited) {
                s += 'n';
            }
        }
        if (!(cellCordY == this->mazeSize - 1)) {
            if (!this->maze[cellCordX][cellCordY + 1][cellCordZ][cellCordW].visited) {
                s += 's';
            }
        }

        if (!(cellCordZ == 0)) {
            if (!this->maze[cellCordX][cellCordY][cellCordZ - 1][cellCordW].visited) {
                s += 'd';
            }
        }
        if (!(cellCordZ == this->mazeSize - 1)) {
            if (!this->maze[cellCordX][cellCordY][cellCordZ + 1][cellCordW].visited) {
                s += 'u';
            }
        }

        if (!(cellCordW == 0)) {
            if (!this->maze[cellCordX][cellCordY][cellCordZ][cellCordW - 1].visited) {
                s += 'k';
            }
        }
        if (!(cellCordW == this->mazeSize - 1)) {
            if (!this->maze[cellCordX][cellCordY][cellCordZ][cellCordW + 1].visited) {
                s += 'a';
            }
        }

        std::cout << "[" << stack.top()[0] << ", " << stack.top()[1] << ", " << stack.top()[2] << ", " << stack.top()[3] << "]" << s << std::endl;

        
        // setting cells up
        if (s.size() < 1) {
            stack.pop();
        }
        else {
            int r = rand() % s.size();
            //std::cout << s[r];

            switch (s[r]) {
            case 'n':
                pomV[0] = cellCordX;
                pomV[1] = cellCordY - 1;
                pomV[2] = cellCordZ;
                pomV[3] = cellCordW;

                stack.push(pomV);
                this->maze[cellCordX][cellCordY - 1][cellCordZ][cellCordW].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW].n = true;
                this->maze[cellCordX][cellCordY - 1][cellCordZ][cellCordW].s = true;
                break;

            case 's':
                pomV[0] = cellCordX;
                pomV[1] = cellCordY + 1;
                pomV[2] = cellCordZ;
                pomV[3] = cellCordW;

                stack.push(pomV);
                this->maze[cellCordX][cellCordY + 1][cellCordZ][cellCordW].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW].s = true;
                this->maze[cellCordX][cellCordY + 1][cellCordZ][cellCordW].n = true;
                break;

            case 'w':
                pomV[0] = cellCordX - 1;
                pomV[1] = cellCordY;
                pomV[2] = cellCordZ;
                pomV[3] = cellCordW;

                stack.push(pomV);
                this->maze[cellCordX - 1][cellCordY][cellCordZ][cellCordW].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW].w = true;
                this->maze[cellCordX - 1][cellCordY][cellCordZ][cellCordW].e = true;
                break;

            case 'e':
                pomV[0] = cellCordX + 1;
                pomV[1] = cellCordY;
                pomV[2] = cellCordZ;
                pomV[3] = cellCordW;

                stack.push(pomV);
                this->maze[cellCordX + 1][cellCordY][cellCordZ][cellCordW].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW].e = true;
                this->maze[cellCordX + 1][cellCordY][cellCordZ][cellCordW].w = true;
                break;

            case 'u':
                pomV[0] = cellCordX;
                pomV[1] = cellCordY;
                pomV[2] = cellCordZ + 1;
                pomV[3] = cellCordW;

                stack.push(pomV);
                this->maze[cellCordX][cellCordY][cellCordZ + 1][cellCordW].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW].u = true;
                this->maze[cellCordX][cellCordY][cellCordZ + 1][cellCordW].d = true;
                break;

            case 'd':
                pomV[0] = cellCordX;
                pomV[1] = cellCordY;
                pomV[2] = cellCordZ - 1;
                pomV[3] = cellCordW;

                stack.push(pomV);
                this->maze[cellCordX][cellCordY][cellCordZ - 1][cellCordW].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW].d = true;
                this->maze[cellCordX][cellCordY][cellCordZ - 1][cellCordW].u = true;
                break;

            case 'a':
                pomV[0] = cellCordX;
                pomV[1] = cellCordY;
                pomV[2] = cellCordZ;
                pomV[3] = cellCordW + 1;

                stack.push(pomV);
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW + 1].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW].a = true;
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW + 1].k = true;
                break;

            case 'k':
                pomV[0] = cellCordX;
                pomV[1] = cellCordY;
                pomV[2] = cellCordZ;
                pomV[3] = cellCordW - 1;

                stack.push(pomV);
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW - 1].visited = true;
                visited++;
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW].k = true;
                this->maze[cellCordX][cellCordY][cellCordZ][cellCordW - 1].a = true;
                break;
            }
        }
    }
}

void Maze4D::drawMaze() {
    // draw maze

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
    wall.setPosition(0, mazeWidth);
    this->window.draw(wall);

    // north outer walls
    wall.setSize(sf::Vector2f(mazeWidth, this->wallSize));
    wall.setFillColor(sf::Color::Black);
    wall.setPosition(0, mazeWidth);
    this->window.draw(wall);


    //fourth board
    // west outer walls
    wall.setSize(sf::Vector2f(this->wallSize, mazeWidth));
    wall.setFillColor(sf::Color::Black);
    wall.setPosition(mazeWidth, mazeWidth);
    this->window.draw(wall);

    // north outer walls
    wall.setSize(sf::Vector2f(mazeWidth, this->wallSize));
    wall.setFillColor(sf::Color::Black);
    wall.setPosition(mazeWidth, mazeWidth);
    this->window.draw(wall);

    
    // draw maze
    sf::RectangleShape cell(sf::Vector2f(this->cellSize, this->cellSize));
    cell.setFillColor(sf::Color::White);

    // first maze, XY (standard cord system)
    for (int y = 0; y < this->mazeSize; y++) {
        for (int x = 0; x < this->mazeSize; x++) {
            cell.setPosition(x * this->cellSize + x * this->wallSize + this->wallSize, y * this->cellSize + y * this->wallSize + this->wallSize);
            this->window.draw(cell);

            // southern wall for each cell
            if (!this->maze[x][y][this->playerPosition[2]][this->playerPosition[3]].s) {
                sf::RectangleShape wall(sf::Vector2f(this->cellSize + 2 * this->wallSize, this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(cell.getPosition().x - this->wallSize, cell.getPosition().y + this->cellSize);
                this->window.draw(wall);
            }

            // eastern wall for each cell
            if (!this->maze[x][y][this->playerPosition[2]][this->playerPosition[3]].e) {
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
            if (!this->maze[x][this->playerPosition[1]][z][this->playerPosition[3]].u) {
                sf::RectangleShape wall(sf::Vector2f(this->cellSize + 2 * this->wallSize, this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(mazeWidth + cell.getPosition().x - this->wallSize, cell.getPosition().y + this->cellSize);
                this->window.draw(wall);
            }

            // northern wall for each cell
            if (!this->maze[x][this->playerPosition[1]][z][this->playerPosition[3]].e) {
                sf::RectangleShape wall(sf::Vector2f(this->wallSize, this->cellSize + this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(mazeWidth + cell.getPosition().x + this->cellSize, cell.getPosition().y);
                this->window.draw(wall);
            }
        }
    }

    
    // third maze, WY
    for (int y = 0; y < this->mazeSize; y++) {
        for (int w = 0; w < this->mazeSize; w++) {
            cell.setPosition(w * this->cellSize + w * this->wallSize + this->wallSize, y * this->cellSize + y * this->wallSize + this->wallSize);
            this->window.draw(cell);

            // southern wall for each cell
            if (!this->maze[this->playerPosition[0]][y][this->playerPosition[2]][w].s) {
                sf::RectangleShape wall(sf::Vector2f(this->cellSize + 2 * this->wallSize, this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(cell.getPosition().x - this->wallSize, mazeWidth + cell.getPosition().y + this->cellSize);
                this->window.draw(wall);
            }

            // northern wall for each cell
            if (!this->maze[this->playerPosition[0]][y][this->playerPosition[2]][w].a) {
                sf::RectangleShape wall(sf::Vector2f(this->wallSize, this->cellSize + this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(cell.getPosition().x + this->cellSize, mazeWidth + cell.getPosition().y);
                this->window.draw(wall);
            }
        }
    }
    
    // fourth maze, WZ
    for (int z = 0; z < this->mazeSize; z++) {
        for (int w = 0; w < this->mazeSize; w++) {
            cell.setPosition(w * this->cellSize + w * this->wallSize + this->wallSize, z * this->cellSize + z * this->wallSize + this->wallSize);
            this->window.draw(cell);

            // southern wall for each cell
            if (!this->maze[this->playerPosition[0]][this->playerPosition[1]][z][w].u) {
                sf::RectangleShape wall(sf::Vector2f(this->cellSize + 2 * this->wallSize, this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(mazeWidth + cell.getPosition().x - this->wallSize, mazeWidth + cell.getPosition().y + this->cellSize);
                this->window.draw(wall);
            }

            // northern wall for each cell
            if (!this->maze[this->playerPosition[0]][this->playerPosition[1]][z][w].a) {
                sf::RectangleShape wall(sf::Vector2f(this->wallSize, this->cellSize + this->wallSize));
                wall.setFillColor(sf::Color::Black);
                wall.setPosition(mazeWidth + cell.getPosition().x + this->cellSize, mazeWidth + cell.getPosition().y);
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
    wall.setPosition(cellSize / 8, mazeWidth - cellSize / 8 - this->cellSize / 2);
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


    // WY
    // W
    wall.setSize(sf::Vector2f(this->cellSize / 2, 2 * this->wallSize));
    wall.setFillColor(sf::Color::Cyan);
    wall.setPosition(cellSize / 8, mazeWidth + mazeWidth - cellSize / 8);
    this->window.draw(wall);

    // Y
    wall.setSize(sf::Vector2f(2 * this->wallSize, this->cellSize / 2));
    wall.setFillColor(sf::Color::Green);
    wall.setPosition(cellSize / 8, mazeWidth + mazeWidth - cellSize / 8 - this->cellSize / 2);
    this->window.draw(wall);


    // WY
    // W
    wall.setSize(sf::Vector2f(this->cellSize / 2, 2 * this->wallSize));
    wall.setFillColor(sf::Color::Cyan);
    wall.setPosition(mazeWidth + cellSize / 8, mazeWidth + mazeWidth - cellSize / 8);
    this->window.draw(wall);

    // Z
    wall.setSize(sf::Vector2f(2 * this->wallSize, this->cellSize / 2));
    wall.setFillColor(sf::Color::Blue);
    wall.setPosition(mazeWidth + cellSize / 8, mazeWidth + mazeWidth - cellSize / 8 - this->cellSize / 2);
    this->window.draw(wall);
    
}


void Maze4D::drawStartFinish() {
    // draw start finish circles

    int mazeWidth = this->cellSize * this->mazeSize + this->wallSize * this->mazeSize + this->wallSize;
    sf::CircleShape circle(this->cellSize / 3);

    // ***************** start *************************
    circle.setFillColor(sf::Color::Red);

    // start in XY plane
    if (this->playerPosition[2] == 0 && this->playerPosition[3] == 0) {
        circle.setPosition(sf::Vector2f(this->wallSize + this->cellSize / 6, this->wallSize + this->cellSize / 6));
       
        window.draw(circle);
    }

    // start in XZ plane
    if (this->playerPosition[1] == 0 && this->playerPosition[3] == 0) {
        circle.setPosition(sf::Vector2f(mazeWidth + this->wallSize + this->cellSize / 6, this->wallSize + this->cellSize / 6));
        window.draw(circle);
    }

    // start in WY plane
    if (this->playerPosition[0] == 0 && this->playerPosition[2] == 0) {
        circle.setPosition(sf::Vector2f(this->wallSize + this->cellSize / 6, mazeWidth + this->wallSize + this->cellSize / 6));
        window.draw(circle);
    }

    // start in WZ plane
    if (this->playerPosition[0] == 0 && this->playerPosition[1] == 0) {
        circle.setPosition(sf::Vector2f(mazeWidth + this->wallSize + this->cellSize / 6, mazeWidth + this->wallSize + this->cellSize / 6));
        window.draw(circle);
    }

    // **************** finish *********************
    circle.setFillColor(sf::Color::Green);

    // finish in XY plane
    if (this->playerPosition[2] == this->mazeSize - 1 && this->playerPosition[3] == this->mazeSize - 1) {
        circle.setPosition(sf::Vector2f(this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6,
            this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6));
        
        window.draw(circle);
    }

    // finish in XZ plane
    if (this->playerPosition[1] == this->mazeSize - 1 && this->playerPosition[3] == this->mazeSize - 1) {
        circle.setPosition(sf::Vector2f(mazeWidth + this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6,
            this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6));
        window.draw(circle);
    }

    // finish in WY plane
    if (this->playerPosition[0] == this->mazeSize - 1 && this->playerPosition[2] == this->mazeSize - 1) {
        circle.setPosition(sf::Vector2f(this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6,
            mazeWidth + this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6));
        window.draw(circle);
    }

    // finish in WZ plane
    if (this->playerPosition[0] == this->mazeSize - 1 && this->playerPosition[1] == this->mazeSize - 1) {
        circle.setPosition(sf::Vector2f(mazeWidth + this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6,
            mazeWidth + this->cellSize * (this->mazeSize - 1) + this->wallSize * (this->mazeSize) + this->cellSize / 6));
        window.draw(circle);
    }
}

void Maze4D::drawPlayer() {
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

    // ZY plane -> WY
    player.setPosition(sf::Vector2f(this->cellSize * (this->playerPosition[3]) + this->wallSize * (this->playerPosition[3] + 1) + this->cellSize / 6,
        mazeWidth + this->cellSize * (this->playerPosition[1]) + this->wallSize * (this->playerPosition[1] + 1) + this->cellSize / 6));
    window.draw(player);

    // WZ
    player.setPosition(sf::Vector2f(mazeWidth + this->cellSize * (this->playerPosition[3]) + this->wallSize * (this->playerPosition[3] + 1) + this->cellSize / 6,
        mazeWidth + this->cellSize * (this->playerPosition[2]) + this->wallSize * (this->playerPosition[2] + 1) + this->cellSize / 6));
    window.draw(player);
}

void Maze4D::drawStats() {

    // draw stats and text to console

    int mazeWidth = this->cellSize * this->mazeSize + this->wallSize * this->mazeSize + this->wallSize;

    sf::Font font;
    font.loadFromFile("ARIAL.TTF");
    if (!font.loadFromFile("ARIAL.TTF"))
    {
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
    text.setPosition(2 * mazeWidth + 10 * this->rightMargin / 20, 0);
    window.draw(text);

    text.setString(std::to_string(playerPosition[2]));
    if (playerPosition[2] < 10) {
        text.setPosition(2 * mazeWidth + 10 * this->rightMargin / 20, text.getCharacterSize());
    }
    else {
        text.setPosition(2 * mazeWidth + 10 * this->rightMargin / 20 - text.getCharacterSize() / 4, text.getCharacterSize());
    }
    window.draw(text);

    
    // w cord
    text.setString("W");
    text.setFillColor(sf::Color::Cyan);
    text.setPosition(2 * mazeWidth + 14*this->rightMargin / 20, 0);
    window.draw(text);

    text.setString(std::to_string(playerPosition[3]));
    if (playerPosition[3] < 10) {
        text.setPosition(2 * mazeWidth + 14*this->rightMargin / 20, text.getCharacterSize());
    }
    else {
        text.setPosition(2 * mazeWidth + 14*this->rightMargin / 20 - text.getCharacterSize() / 5, text.getCharacterSize());
    }
    window.draw(text);


    // win screen
    if (playerPosition[0] == mazeSize - 1 && playerPosition[1] == mazeSize - 1 && playerPosition[2] == mazeSize - 1 && playerPosition[3] == mazeSize - 1) {
        text.setFillColor(sf::Color::Red);
        text.setCharacterSize(this->windowWidth / 5);
        text.setPosition(this->mazeSize * this->cellSize / 10, this->mazeSize * this->cellSize / 4);
        text.setString("You Win!");
        window.draw(text);
    }
}
