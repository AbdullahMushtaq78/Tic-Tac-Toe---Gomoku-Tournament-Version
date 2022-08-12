#include <iostream>
#include<fstream>
#include <iomanip>
using namespace std;
#define Grid 19
#define MaxPlayers 10
int WinnerIndex = 0;
struct Board {
    char G[Grid][Grid];
    int Dimension;
    char PlayerSymbol[MaxPlayers];
    char PlayerName[MaxPlayers][30];
    int turn, ri = Grid, ci = Grid, ConsecWinCounter, NOP, WinOrder[MaxPlayers]{};
};
struct Position {
    int ri, ci;
};
void Init(Board& B)
{
    fstream Rdr("Board.txt");
    Rdr >> B.Dimension;
    for (int ri = 0; ri < B.Dimension; ri++)
    {
        for (int ci = 0; ci < B.Dimension; ci++)
        {
            Rdr >> B.G[ri][ci];
        }
    }
    cout << "Enter the number of players: ";
    cin >> B.NOP;
    int shownumber = 1;
    for (int t = 0; t < B.NOP; t++)
    {
        cout << endl << "Enter Player " << shownumber << " Name: ";
        cin >> B.PlayerName[t];
        cout << "Enter the Symbol of Player " << shownumber << " Symbol: ";
        cin >> B.PlayerSymbol[t];
        shownumber++;
    }
    cout << "Enter the Win counter: ";
    cin >> B.ConsecWinCounter;
    B.turn = rand() % B.NOP;
}
void DisplayBoard(char b[][Grid], int dim)
{


    int riDisplay = 1;
    int ciDisplay = 1;
    cout << "      ";
    for (int i = 0; i < dim; i++)
    {
        cout << setw(2) << ciDisplay << "  ";
        ciDisplay++;
    }
    cout << endl << endl;
    for (int ri = 0; ri < dim; ri++)
    {
        cout << left << setw(4) << riDisplay << "|";
        for (int ci = 0; ci < dim; ci++)
        {
            cout << right << setw(2) << b[ri][ci] << " |";
        }
        cout << endl << "    ";
        for (int i = 0; i < dim; i++)
        {
            cout << "----";
        }
        cout << endl;
        riDisplay++;
    }
}
void DisplayTurnMsg(char aikName[], char aikSym)
{
    cout << aikName << "'s Turn " << endl << "Symbol is: " << aikSym << endl << "Sample Input--->[row's index, coloum's index]" << endl;

}
void SelectPosition(Position& p)
{
    int repeaterRef = 0;
    while (true) {
        if (repeaterRef > 0)
            cout << "Enter the Values Again, Values must be lesser than Grid or Size of Board!\nSample Input--->[row's index, coloum's index]: " << endl;
        cin >> p.ri >> p.ci;
        p.ri--, p.ci--;
        if (p.ri > Grid || p.ci > Grid)
        {
            repeaterRef++;
        }
        else
            break;
    }
}
bool isValidPosition(Position p, int d, char b[][Grid])
{
    return((p.ri >= 0 && p.ci >= 0) && (p.ri < d&& p.ci < d)/* && (b[p.ri][p.ci] == '-')*/);
}
bool OverwriteError(Position p, char G[][Grid], Board B) {
    for (int i = 0; i < B.NOP; i++)
    {
        if (G[p.ri][p.ci] == B.PlayerSymbol[i])
        {
            return true;
        }
    }
    return false;
}
void UpdateBoard(char b[][Grid], Position p, char One_Sym)
{
    b[p.ri][p.ci] = One_Sym;

}
void TurnChange(int& turn, int NOP, Board B)
{
    bool flag = true;
    turn = (turn + 1) % NOP;
    for (int i = 0; i < WinnerIndex&&flag; i++) {
        flag = true;
        if (turn == B.WinOrder[i])
        {
            flag = false;
            turn = (turn + 1) % NOP;
        }
    }
}
bool H_Win_Checker(Board b, int ri, int ci)
{
    if ((ci + b.ConsecWinCounter) > b.Dimension)
    {
        return false;
    }
    for (int i = 0; i < b.ConsecWinCounter; i++)
    {
        if (b.G[ri][ci + i] != b.PlayerSymbol[b.turn])
        {
            return false;
        }
    }
    return true;
}
bool V_Win_Checker(Board b, int ri, int ci)
{
    if ((ri + b.ConsecWinCounter) > b.Dimension)
    {
        return false;
    }
    for (int i = 0; i < b.ConsecWinCounter; i++)
    {
        if (b.G[ri + i][ci] != b.PlayerSymbol[b.turn])
        {
            return false;
        }
    }
    return true;
}
bool LowerD_Win_Checker(Board b, int ri, int ci)
{
    if ((ri + b.ConsecWinCounter) > b.Dimension && (ci + b.ConsecWinCounter) > b.Dimension)
        return false;
    for (int i = 0; i < b.ConsecWinCounter; i++)
    {
        if (b.G[ri + i][ci + i] != b.PlayerSymbol[b.turn])
            return false;
    }
    return true;
}
bool UpperD_Win_Checker(Board b, int ri, int ci)
{
    if ((ri + b.ConsecWinCounter) > b.Dimension && (ci - b.ConsecWinCounter - 1) < 0)
        return false;
    for (int i = 0; i < b.ConsecWinCounter; i++)
    {
        if (b.G[ri + i][ci - i] != b.PlayerSymbol[b.turn])
            return false;
    }
    return true;
}
bool IsWin(Board b)
{
    for (int i = 0; i < b.Dimension; i++)
    {

        for (int j = 0; j < b.Dimension; j++)
        {
            if (H_Win_Checker(b, i, j) || V_Win_Checker(b, i, j) || LowerD_Win_Checker(b, i, j) || UpperD_Win_Checker(b, i, j))
                return true;
        }
    }
    return false;
}
bool IsDraw(Board b)
{
    for (int i = 0; i < b.Dimension; i++)
    {
        for (int j = 0; j < b.Dimension; j++)
        {
            if (b.G[i][j] == '-')
                return false;
        }
    }
    return true;
}
void SkipperAWinner(Board& B) {
    B.WinOrder[WinnerIndex] = B.turn;
    TurnChange(B.turn, B.NOP, B);
    WinnerIndex++;
}
void Main() {

    Board B;
    Position pos;
    int ref = 1;
    char color[9] = "Color E0";
    int colorChanger = 0;
    int draw = -1;
    Init(B);
    do {
        system(color);
        DisplayBoard(B.G, B.Dimension);
        if (colorChanger < 9)
            colorChanger++;
        if (colorChanger == 7 || colorChanger == 6)
            colorChanger++;
        if (colorChanger == 7)
            colorChanger++;
        if (colorChanger >= 9)
            colorChanger = 0;
        color[7] = (char)(48 + colorChanger);
        DisplayTurnMsg(B.PlayerName[B.turn], B.PlayerSymbol[B.turn]);
        do {

            SelectPosition(pos);
            system("cls");
            if (!isValidPosition(pos, B.Dimension, B.G)) {
                cout << "Enter the coordinates Again: \n";
                continue;
            }
            else 
                break;
        } while (true);
        if (!OverwriteError(pos, B.G, B))
        {
            UpdateBoard(B.G, pos, B.PlayerSymbol[B.turn]);
            if (IsWin(B))
            {
                SkipperAWinner(B);
            }
            else
            {
                TurnChange(B.turn, B.NOP, B);
            }
            if (IsDraw(B)) {
                draw = 1;
                break;
            }
        }
        else {
            cout << "You are trying to overwrite the other player's character!\n" << endl;
            system("pause");
            system("cls");
            cout << "Enter the Coloum and Row Again!" << endl;
        }
    } while (WinnerIndex != B.NOP - 1);
    if (draw == 1)
    {
        cout << "\t\t\tOops! The Game is Draw now...  :( \n";
    }
    else
    {
        cout << "\n\t\t---------------------------------------------------------" << endl;
        cout << "\t\t|                     --------------                     |\n";
        cout << "\t\t|                     | Score Card |                     |\n";
        cout << "\t\t|                     --------------                     |\n";
        cout << "\t\t---------------------------------------------------------" << endl;
        cout << "\t\tFollowing are the winners of this Tournament: " << endl;
        for (int i = 0; i < B.NOP - 1; i++)
        {
            int index = B.WinOrder[i];
            cout << "\t\t----->" << i + 1 << ".\t" << B.PlayerName[index]<<" (Congratulations)" << endl;
        }
        cout << "\n\t\t---------------------------------------------------------" << endl;
        cout << "\n\t\tAnd the Loser is: " << endl;
        cout << "\t\t-----> " << "\t " << B.PlayerName[B.turn] << " (Best of Luck next time)." << endl;
        cout << "\n\t\t---------------------------------------------------------" << endl;
        cout << endl << "\t\t                         Game Over." << endl;
    }
}
int main()
{
    system("Color 04");
    cout << "\t\t\t--------------------------------\n";
    cout << "\t\t\t||    HELLO                   ||\n";
    cout << "\t\t\t|| WELCOME TO THE GOMOKU GAME ||\n";
    cout << "\t\t\t||               TOURNAMENT   ||\n";
    cout << "\t\t\t--------------------------------\n\n";
     Main();
}