
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <time.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define SPACE 32
#define ESC 27

using namespace std;

vector<int> readdata()
{
    ifstream file("userdata.txt");
    if (file.is_open())
    {
        vector<int> data(3, 0);
        for (int i = 0; i < 3; i++)
            file >> data[i];
        file.close();
        return data;
    }
    else
    {
        ofstream file("userdata.txt", ios::out | ios::trunc);
        file << 25 << endl
             << 25 << endl
             << 0 << endl;
        file.close();
        return readdata();
    }
}

vector<int> data = readdata();
int WIDTH = (data[0] > 2) ? data[0] : 50;
int HEIGHT = (data[1] > 2) ? data[1] : 25;
vector<vector<int>> map(HEIGHT, vector<int>(WIDTH, 0));

void gotoxy(int x, int y)
{ // inline make the fuctions slightly faster its useful for small functions which are called more frequently
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void hideCursor()
{
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

class Tetris
{
public:
    struct Tetromino
    {
        char type;
        std::vector<std::vector<int>> shape;
        int color;
        Tetromino(char t, std::vector<std::vector<int>> s, int c) : type(t), shape(s), color(c) {}
    };

    int x, y;
    bool spwancollision;
    std::vector<Tetromino> tetrominoes;
    Tetromino tetromino;

    Tetris() : spwancollision(false), tetromino('I', {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 9)
    {
        tetrominoes = {
            {'I', {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 9},
            {'O', {{1, 1}, {1, 1}}, 14},
            {'T', {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}}, 5},
            {'S', {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}}, 12},
            {'Z', {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}, 10},
            {'J', {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}}, 3},
            {'L', {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}}, 15}};
    }

    void Rotate()
    {
        int n = tetromino.shape.size();
        std::vector<std::vector<int>> rotated(n, std::vector<int>(n, 0));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                rotated[j][n - 1 - i] = tetromino.shape[i][j];
        }
        tetromino.shape = rotated;
    }

    void moveLeft()
    {
        if (x >= -1 && !isCollision(-1, 0))
            x--;
    }

    void moveRight()
    {
        if (x < WIDTH - int(tetromino.shape[0].size()) && !isCollision(1, 0))
            x++;
    }

    void moveDown()
    {
        if (!isCollision(0, 1))
            y++;
        else
        {
            merge();
            createNewTetromino();
            RenderTetris();
        }
    }

    void hardDrop()
    {
        while (!isCollision(0, 1))
            y++;
        merge();
        createNewTetromino();
        RenderTetris();
    }

    bool isCollision(int dx, int dy)
    {
        for (int i = 0; i < int(tetromino.shape.size()); i++)
        {
            for (int j = 0; j < int(tetromino.shape.size()); j++)
            {
                if (tetromino.shape[i][j])
                {
                    int newX = x + j + dx;
                    int newY = y + i + dy;
                    if (newX <= 0 || newX >= WIDTH - 1 || newY >= HEIGHT - 1)
                        return true;
                    if (map[newY][newX] >= 10 || map[newY][newX] == 9)
                        return true;
                    if (map[newY][newX] == 2 || newY < 0)
                        return true;
                }
            }
        }
        return false;
    }

    void merge()
    {
        for (int i = 0; i < int(tetromino.shape.size()); i++)
        {
            for (int j = 0; j < int(tetromino.shape[0].size()); j++)
            {
                if (tetromino.shape[i][j])
                    map[y + i][x + j] = 10 + tetromino.color;
            }
        }
    }

    int createNewTetromino()
    {
        tetromino = tetrominoes[rand() % tetrominoes.size()];
        x = WIDTH / 2 - (int(tetromino.shape[0].size()) / 2);
        y = 1;
        if (isCollision(0, 0))
        {
            spwancollision = true;
            setColor(4);
            gotoxy(WIDTH * 2 + 5, HEIGHT / 2 + 3);
            cout << "Spawn collision detected! Game over.";
            Sleep(1000);
            return -1;
        }
        RenderTetris();
        return 0;
    }

    void RenderTetris()
    {
        for (int i = 0; i < int(tetromino.shape.size()); i++)
        {
            for (int j = 0; j < int(tetromino.shape[0].size()); j++)
            {
                if (tetromino.shape[i][j])
                    map[y + i][x + j] = 1;
            }
        }
    }

    bool currTetromino()
    {
        for (int i = 0; i < int(map.size()); i++)
        {
            for (int j = 0; j < int(map[0].size()); j++)
            {
                if (map[i][j] == 1)
                    return true;
            }
        }
        return false;
    }
};

class Game
{

private:
    Tetris &tetris;
    bool gameover;
    int score;
    int Difficulty = 1;
    int highscore;
    bool dev = false; // Developer mode

public:
    Game(Tetris &tetris, int highestscore) : tetris(tetris), gameover(false), score(0), highscore(highestscore)
    {
        layout();
    }

    void writedata(int width, int height, int highscore)
    {
        ofstream file("userdata.txt", std::ios::out | std::ios::trunc); // ios::trunc will always clear the file if it exists
        if (file.is_open())
        {
            file << width << endl;
            file << height << endl;
            file << highscore << endl;
            file.close();
        }
        else
        {
            cout << "Unable to open file";
        }
    }
    void layout()
    {
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1)
                    map[i][j] = 9;
                else if (map[i][j] >= 10)
                    continue;
                else
                    map[i][j] = 0;
            }
        }
    }

    void pauseMenu()
    {
        gotoxy(WIDTH * 2 + 5, HEIGHT / 2 - 3);
        setColor(1);
        cout << "Game Paused" << endl;
        gotoxy(WIDTH * 2 + 5, HEIGHT / 2 - 1);
        setColor(2);
        cout << "R - Resume" << endl;
        gotoxy(WIDTH * 2 + 5, HEIGHT / 2);
        cout << "E - Exit";
        gotoxy(WIDTH * 2 + 5, HEIGHT / 2 + 1);
        cout << "M - Main menu";
        setColor(7);

    PAUSE:
        char c = _getch();
        switch (c)
        {
        case 'r':
            system("cls");
            draw();
            break;
        case 'e':
            exit(0);
            break;
        case ESC:
            system("cls");
            draw();
            break;
        case 'm':
            Startmenu();
            break;
        default:
            goto PAUSE;
        }
    }

    void draw()
    {
        int startX = 3, startY = 4;

        gotoxy(startX, startY);

        for (int i = 0; i < HEIGHT; i++)
        {

            for (int j = 0; j < WIDTH; j++)
            {

                gotoxy(j * 2 + startX, i + startY);

                if (map[i][j] == 1)
                {
                    setColor(tetris.tetromino.color);
                    cout << char(219) << char(219);
                    setColor(7);
                }
                else if (map[i][j] >= 10)
                {
                    setColor(map[i][j] - 10);
                    cout << char(219) << char(219);
                    setColor(7);
                }
                else if (map[i][j] == 0)
                    cout << "  ";
                else if (map[i][j] == 9)
                    cout << char(178) << char(178);
            }
        }
    }

    bool checkGameover()
    {
        for (int i = 0; i < WIDTH; i++)
        {
            if (map[2][i] >= 10)
                return true;
        }
        return tetris.spwancollision;
    }

    void update()
    {
        tetris.moveDown();

        layout();
        tetris.RenderTetris();

        if (checkGameover())
        {
            gameover = true;
        }
        draw();
        clearLine();
        Sleep(250 / Difficulty);
    }

    bool isgameOver() { return gameover; }

    void Pause() { pauseMenu(); }

    void clearLine()
    {
        for (int i = HEIGHT - 2; i > 0; i--)
        {
            bool full = true;
            for (int j = 1; j < WIDTH - 1; j++)
            {
                if (map[i][j] == 0)
                {
                    full = false;
                    break;
                }
            }
            if (full)
            {
                for (int k = i; k > 1; k--)
                {
                    for (int j = 1; j < WIDTH - 1; j++)
                    {
                        map[k][j] = map[k - 1][j];
                    }
                }
                for (int j = 1; j < WIDTH - 1; j++)
                {
                    map[1][j] = 0;
                }

                score += WIDTH - 2;

                i++;
                if (score > 5 * (WIDTH - 2) && Difficulty <= 5)
                    Difficulty++;
                if (score > 15 * (WIDTH - 2) && Difficulty <= 5)
                    Difficulty++;
                if (score > 30 * (WIDTH - 2) && Difficulty <= 5)
                    Difficulty++;
                if (score > 50 * (WIDTH - 2) && Difficulty <= 5)
                    Difficulty++; // this will unlock the highest difficulty that is not available in start menu()
            }
        }
    }

    void Reset()
    {
        gameover = false;
        score = 0;
        map.assign(HEIGHT, vector<int>(WIDTH, 0));

        layout();

        tetris.spwancollision = false;
        tetris.createNewTetromino();
        tetris.RenderTetris();
    }

    int GameoverScreen()
    {
        system("cls");
        gotoxy(WIDTH, HEIGHT / 2 - 3);
        setColor(4);
        cout << "GAME OVER!!";
        gotoxy(WIDTH + 3, HEIGHT / 2 - 1);
        setColor(2);
        cout << "Score: " << score << "        Highest Score: " << score;
        gotoxy(WIDTH + 1, HEIGHT / 2 + 1);
        setColor(1);
        cout << "Press E - Exit";
        gotoxy(WIDTH + 1, HEIGHT / 2 + 2);
        cout << "Press R - Restart the game";
        gotoxy(WIDTH + 1, HEIGHT / 2 + 3);
        cout << "Press M - Main Screen";
        setColor(7);

        char in = _getch();
        switch (in)
        {
        case 'e':
            exit(0);
            break;
        case 'r':
            system("cls");
            return 1;
        case 'm':
            Startmenu();
            break;
            return 0;
        }
    }

    void Startmenu()
    {
    START:
        system("cls");
        gotoxy(10, 5);
        setColor(14);
        cout << "Welcome to Tetris by Team Tech-Squad";

        gotoxy(14, 7);
        setColor(1);
        cout << "1. Start the Game";
        gotoxy(14, 9);
        cout << "2. Options";
        gotoxy(14, 11);
        cout << "3. Exit the Game";
        setColor(7);

        char in = _getch();
        switch (in)
        {
        case '1':
            system("cls");
            break;

        case '2':
            system("cls");
            setColor(14);
            gotoxy(10, 5);
            cout << "Options";
            setColor(1);
            gotoxy(14, 7);
            cout << "1. Change Difficulty Level";
            gotoxy(14, 9);
            cout << "2. Set Gridsize";
            gotoxy(14, 11);
            cout << "3. Back to Main Menu";

            in = _getch();

            switch (in)
            {
            case '1':
            CHANGE_DIFFICULTY:

                system("cls");
                setColor(14);
                gotoxy(10, 5);
                cout << "Change Difficulty Level";
                setColor(3);
                gotoxy(14, 7);
                cout << "1. Easy";
                setColor(6);
                gotoxy(14, 9);
                cout << "2. Normal";
                setColor(4);
                gotoxy(14, 11);
                cout << "3. Hard";
                setColor(1);
                gotoxy(14, 13);
                cout << "4. Back to Options";
                setColor(7);

                in = _getch();
                switch (in)
                {
                case '1':
                    Difficulty = 1;
                    goto START;
                    break;
                case '2':
                    Difficulty = 2;
                    goto START;
                    break;
                case '3':
                    Difficulty = 4;
                    goto START;
                    break;
                case '4':
                    system("cls");
                    goto START;
                    break;
                default:
                    gotoxy(14, 15);
                    setColor(4);
                    cout << "Invalid Selection! Please try again.";
                    Sleep(1500);
                    system("cls");
                    setColor(7);
                    goto CHANGE_DIFFICULTY;
                }
                break;

            case '2':
            SETGRID:
                system("cls");
                setColor(14);
                gotoxy(10, 5);
                cout << "Enter the Grid size ( (w,h) , limit (10, 10) - (60, 30)):";
                int x, y;
                gotoxy(16, 7);
                setColor(1);
                cin >> x >> y;
                if (x >= 10 && y >= 10 && x <= 60 && y <= 30)
                {
                    WIDTH = x;
                    HEIGHT = y;

                    writedata(x, y, highscore);
                    setColor(7);

                    gotoxy(10, 10);

                    cout << "Grid Size set Restarting the game....";

                    Restart();
                }
                else
                {
                    setColor(4);
                    cout << "Invalid Grid size! Please try again.";
                    Sleep(1500);
                    goto SETGRID;
                }
                setColor(7);
                break;

            case '3':
                goto START;
                break;

            default:
                gotoxy(14, 13);
                setColor(4);
                cout << "Invalid Selection! Please try again.";
                Sleep(1500);
                system("cls");
                goto START;
            }

        case '3':
            exit(0);
            break;

        default:
            gotoxy(14, 15);
            setColor(4);
            cout << "Invalid Selection! Please try again.";
            Sleep(1500);
            system("cls");
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            goto START;
        }
    }

    void Restart()
    {
        Sleep(2000);
        // Relaunch the program
        system("start tetris.exe");

        // exit the current window
        exit(0);
    }

    int Score() { return score; }

    bool isDev_mode() { return dev; }

    void setDev_mode(bool val) { dev = val; }
};

void input(Tetris &t1, Game &game)
{
    if (_kbhit())
    {
        int ch = _getch();
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // from stackoverflow
        if (ch == SPACE)
            t1.hardDrop();
        if (ch == ESC)
            game.Pause();

        if (ch == 224)
        {
            ch = _getch();
            switch (ch)
            {
            case KEY_UP:
                t1.Rotate();
                break;
            case KEY_DOWN:
                t1.moveDown();
                break;
            case KEY_LEFT:
                t1.moveLeft();
                break;
            case KEY_RIGHT:
                t1.moveRight();
                break;
            }
        }
    }
}

int main()
{
    srand(time(0));
    Tetris t1;
    Game game(t1, data[2]);

    HWND s = GetConsoleWindow(); // from stackoverflow
    MoveWindow(s, 300, 100, 480, 620, true);
    hideCursor();
    system("cls");

    t1.createNewTetromino();
    t1.RenderTetris();
    game.Startmenu();

    while (true)
    {
        while (!game.isgameOver())
        {

            if (game.isDev_mode())
            {
                gotoxy(WIDTH * 2 + 5, 6);
                setColor(6);
                gotoxy(WIDTH * 2 + 5, 7);
                cout << "PRESS E to swtich to dev-menu";
                setColor(7);
            }

            data = readdata();
            gotoxy(WIDTH / 2 - 5, 1);
            cout << "SCORE: " << game.Score();
            gotoxy(WIDTH / 2 + 10, 1);
            cout << "HIGHEST: " << data[2];
            game.update();

            if (game.Score() > data[2])
                game.writedata(WIDTH, HEIGHT, game.Score());

            input(t1, game);
        }
        if (game.GameoverScreen() == 1)
            game.Reset();
    }
    system("cls");
    return 0;
}
