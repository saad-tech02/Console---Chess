#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

const int SIZE = 8;

enum Color { WHITE, BLACK };

class Board;

// abstract class 
class Piece {
protected:
    Color color;
    char symbol;
    int row, col;
    bool firstMove;

public:
    Piece(Color c, int r, int co, char s) {
        color = c;
        row = r;
        col = co;
        symbol = s;
        firstMove = true;
    }

    virtual ~Piece() {}

    Color getColor() { return color; }
    char getSymbol() { return symbol; }
    int getRow() { return row; }
    int getCol() { return col; }
    void setPos(int r, int c) { row = r; col = c; }
    bool isFirstMove() { return firstMove; }
    void moved() { firstMove = false; }

    virtual bool canMove(int toRow, int toCol, Piece* board[SIZE][SIZE]) = 0;
};

class Pawn : public Piece {
public:
    Pawn(Color c, int r, int co) : Piece(c, r, co, (c == WHITE) ? 'P' : 'p') {}

    bool canMove(int toRow, int toCol, Piece* board[SIZE][SIZE]) {
        int dir = (color == WHITE) ? -1 : 1;

        // moving next one step
        if (toCol == col && toRow == row + dir && board[toRow][toCol] == nullptr) {
            return true;
        }

        // moving two steps from start
        if (firstMove && toCol == col && toRow == row + 2 * dir) {
            int mid = row + dir;
            if (board[mid][toCol] == nullptr && board[toRow][toCol] == nullptr) {
                return true;
            }
        }

        // capturing diagonally
        if (abs(toCol - col) == 1 && toRow == row + dir) {
            if (board[toRow][toCol] != nullptr && board[toRow][toCol]->getColor() != color) {
                return true;
            }
        }

        return false;
    }
};

class Rook : public Piece {
public:
    Rook(Color c, int r, int co) : Piece(c, r, co, (c == WHITE) ? 'R' : 'r') {}

    bool canMove(int toRow, int toCol, Piece* board[SIZE][SIZE]) {
        // rook moves straight
        if (row != toRow && col != toCol) return false;

        // check path is clear
        if (row == toRow) {
            int start = (col < toCol) ? col : toCol;
            int end = (col > toCol) ? col : toCol;
            for (int c = start + 1; c < end; c++) {
                if (board[row][c] != nullptr) return false;
            }
        }
        else {
            int start = (row < toRow) ? row : toRow;
            int end = (row > toRow) ? row : toRow;
            for (int r = start + 1; r < end; r++) {
                if (board[r][col] != nullptr) return false;
            }
        }

        // cannot capture  capture own piece
        if (board[toRow][toCol] != nullptr && board[toRow][toCol]->getColor() == color) {
            return false;
        }
        return true;
    }
};

class Knight : public Piece {
public:
    Knight(Color c, int r, int co) : Piece(c, r, co, (c == WHITE) ? 'N' : 'n') {}

    bool canMove(int toRow, int toCol, Piece* board[SIZE][SIZE]) {
        int rowDiff = abs(toRow - row);
        int colDiff = abs(toCol - col);

        // L shape movement
        if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))) {
            return false;
        }

        // check next place
        if (board[toRow][toCol] != nullptr && board[toRow][toCol]->getColor() == color) {
            return false;
        }
        return true;
    }
};

class Bishop : public Piece {
public:
    Bishop(Color c, int r, int co) : Piece(c, r, co, (c == WHITE) ? 'B' : 'b') {}

    bool canMove(int toRow, int toCol, Piece* board[SIZE][SIZE]) {
        // must move diagonally
        if (abs(toRow - row) != abs(toCol - col)) return false;

        int dirR = (toRow > row) ? 1 : -1;
        int dirC = (toCol > col) ? 1 : -1;

        int r = row + dirR;
        int c = col + dirC;

        while (r != toRow) {
            if (board[r][c] != nullptr) return false;
            r += dirR;
            c += dirC;
        }

        if (board[toRow][toCol] != nullptr && board[toRow][toCol]->getColor() == color) {
            return false;
        }
        return true;
    }
};

class Queen : public Piece {
public:
    Queen(Color c, int r, int co) : Piece(c, r, co, (c == WHITE) ? 'Q' : 'q') {}

    bool canMove(int toRow, int toCol, Piece* board[SIZE][SIZE]) {
        int rowDiff = abs(toRow - row);
        int colDiff = abs(toCol - col);

        // queen combines rook and bishop movement
        if (!(row == toRow || col == toCol || rowDiff == colDiff)) return false;

        // diagonal movement
        if (row != toRow && col != toCol) {
            int dirR = (toRow > row) ? 1 : -1;
            int dirC = (toCol > col) ? 1 : -1;
            int r = row + dirR, c = col + dirC;

            while (r != toRow) {
                if (board[r][c] != nullptr) return false;
                r += dirR;
                c += dirC;
            }
        }
        // straight movement
        else {
            if (row == toRow) {
                int start = (col < toCol) ? col : toCol;
                int end = (col > toCol) ? col : toCol;
                for (int c = start + 1; c < end; c++) {
                    if (board[row][c] != nullptr) return false;
                }
            }
            else {
                int start = (row < toRow) ? row : toRow;
                int end = (row > toRow) ? row : toRow;
                for (int r = start + 1; r < end; r++) {
                    if (board[r][col] != nullptr) return false;
                }
            }
        }

        if (board[toRow][toCol] != nullptr && board[toRow][toCol]->getColor() == color) {
            return false;
        }
        return true;
    }
};

class King : public Piece {
public:
    King(Color c, int r, int co) : Piece(c, r, co, (c == WHITE) ? 'K' : 'k') {}

    bool canMove(int toRow, int toCol, Piece* board[SIZE][SIZE]) {
        int rowDiff = abs(toRow - row);
        int colDiff = abs(toCol - col);

        // king moves one step anywhere
        if (rowDiff > 1 || colDiff > 1) return false;
        if (rowDiff == 0 && colDiff == 0) return false;

        if (board[toRow][toCol] != nullptr && board[toRow][toCol]->getColor() == color) {
            return false;
        }
        return true;
    }
};

// Board class to manage the game state
class Board {
private:
    Piece* grid[SIZE][SIZE];

    void clearBoard() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                grid[i][j] = nullptr;
            }
        }
    }

public:
    Board() {
        clearBoard();
        setupPieces();
    }

    ~Board() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (grid[i][j] != nullptr) {
                    delete grid[i][j];
                }
            }
        }
    }

    void setupPieces() {
        // black pieces on top
        grid[0][0] = new Rook(BLACK, 0, 0);
        grid[0][1] = new Knight(BLACK, 0, 1);
        grid[0][2] = new Bishop(BLACK, 0, 2);
        grid[0][3] = new Queen(BLACK, 0, 3);
        grid[0][4] = new King(BLACK, 0, 4);
        grid[0][5] = new Bishop(BLACK, 0, 5);
        grid[0][6] = new Knight(BLACK, 0, 6);
        grid[0][7] = new Rook(BLACK, 0, 7);

        for (int i = 0; i < SIZE; i++) {
            grid[1][i] = new Pawn(BLACK, 1, i);
        }

        // white pieces on bottom
        for (int i = 0; i < SIZE; i++) {
            grid[6][i] = new Pawn(WHITE, 6, i);
        }

        grid[7][0] = new Rook(WHITE, 7, 0);
        grid[7][1] = new Knight(WHITE, 7, 1);
        grid[7][2] = new Bishop(WHITE, 7, 2);
        grid[7][3] = new Queen(WHITE, 7, 3);
        grid[7][4] = new King(WHITE, 7, 4);
        grid[7][5] = new Bishop(WHITE, 7, 5);
        grid[7][6] = new Knight(WHITE, 7, 6);
        grid[7][7] = new Rook(WHITE, 7, 7);
    }

    Piece* getPiece(int r, int c) {
        if (r >= 0 && r < SIZE && c >= 0 && c < SIZE) {
            return grid[r][c];
        }
        return nullptr;
    }

    void showBoard() {
        system("cls");

        cout << "\n    a   b   c   d   e   f   g   h"<<endl;
        cout << "  +---+---+---+---+---+---+---+---+"<<endl;

        for (int i = 0; i < SIZE; i++) {
            cout << 8 - i << " |";
            for (int j = 0; j < SIZE; j++) {
                if (grid[i][j] != nullptr) {
                    cout << " " << grid[i][j]->getSymbol() << " |";
                }
                else {
                    cout << "   |";
                }
            }
            cout << " " << 8 - i << "\n";
            cout << "  +---+---+---+---+---+---+---+---+\n";
        }

        cout << "    a   b   c   d   e   f   g   h"<<endl;
        cout << "\nWhite: K Q R B N P    Black: k q r b n p"<<endl;
        cout << "Enter moves like: e2 e4"<<endl<<endl;
    }

    Piece* findKing(Color clr) {
        char ks = (clr == WHITE) ? 'K' : 'k';
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (grid[i][j] != nullptr && grid[i][j]->getSymbol() == ks) {
                    return grid[i][j];
                }
            }
        }
        return nullptr;
    }

    bool isAttacked(int r, int c, Color byWho) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (grid[i][j] != nullptr && grid[i][j]->getColor() == byWho) {
                    if (grid[i][j]->canMove(r, c, grid)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool isInCheck(Color clr) {
        Piece* k = findKing(clr);
        if (k == nullptr) return false;

        Color enemy = (clr == WHITE) ? BLACK : WHITE;
        return isAttacked(k->getRow(), k->getCol(), enemy);
    }

    bool kingCanEscape(Color clr) {
        Piece* k = findKing(clr);
        if (k == nullptr) return false;

        int kr = k->getRow();
        int kc = k->getCol();
        Color enemy = (clr == WHITE) ? BLACK : WHITE;

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;

                int nr = kr + i;
                int nc = kc + j;

                if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE) {
                    if (grid[nr][nc] == nullptr || grid[nr][nc]->getColor() != clr) {
                        if (k->canMove(nr, nc, grid) && !isAttacked(nr, nc, enemy)) {
                            // try the move
                            Piece* tmp = grid[nr][nc];
                            grid[nr][nc] = k;
                            grid[kr][kc] = nullptr;
                            k->setPos(nr, nc);

                            bool still = isAttacked(nr, nc, enemy);

                            // undo
                            grid[kr][kc] = k;
                            grid[nr][nc] = tmp;
                            k->setPos(kr, kc);

                            if (!still) return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool otherPiecesCanHelp(Color clr) {
        Piece* k = findKing(clr);
        if (k == nullptr) return false;

        Color enemy = (clr == WHITE) ? BLACK : WHITE;

        for (int fr = 0; fr < SIZE; fr++) {
            for (int fc = 0; fc < SIZE; fc++) {
                if (grid[fr][fc] != nullptr && grid[fr][fc]->getColor() == clr) {
                    if (fr == k->getRow() && fc == k->getCol()) continue;

                    for (int tr = 0; tr < SIZE; tr++) {
                        for (int tc = 0; tc < SIZE; tc++) {
                            if (fr == tr && fc == tc) continue;

                            Piece* p = grid[fr][fc];
                            if (p->canMove(tr, tc, grid)) {
                                Piece* cap = grid[tr][tc];
                                grid[tr][tc] = p;
                                grid[fr][fc] = nullptr;
                                int oldR = p->getRow();
                                int oldC = p->getCol();
                                p->setPos(tr, tc);

                                bool still = isAttacked(k->getRow(), k->getCol(), enemy);

                                grid[fr][fc] = p;
                                grid[tr][tc] = cap;
                                p->setPos(oldR, oldC);

                                if (!still) return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    bool hasAnyMove(Color clr) {
        if (kingCanEscape(clr)) return true;
        if (otherPiecesCanHelp(clr)) return true;
        return false;
    }

    bool makeMove(int fr, int fc, int tr, int tc) {
        Piece* p = grid[fr][fc];
        if (p == nullptr) return false;

        if (!p->canMove(tr, tc, grid)) return false;

        // simulate to see if move put  king in check
        Piece* cap = grid[tr][tc];
        grid[tr][tc] = p;
        grid[fr][fc] = nullptr;
        int oldR = p->getRow();
        int oldC = p->getCol();
        p->setPos(tr, tc);

        bool inCheck = isInCheck(p->getColor());

        // undo simulation
        grid[fr][fc] = p;
        grid[tr][tc] = cap;
        p->setPos(oldR, oldC);

        if (inCheck) return false;

        // make actual move
        if (grid[tr][tc] != nullptr) {
            delete grid[tr][tc];
        }
        grid[tr][tc] = p;
        grid[fr][fc] = nullptr;
        p->setPos(tr, tc);
        p->moved();

        return true;
    }

    bool isCheckmate(Color clr) {
        if (!isInCheck(clr)) return false;
        if (hasAnyMove(clr)) return false;
        return true;
    }

    bool isStalemate(Color clr) {
        if (isInCheck(clr)) return false;
        if (hasAnyMove(clr)) return false;
        return true;
    }
};

// Game class for handling the gameplay
class Game {
private:
    Board board;
    Color turn;
    bool over;

    bool parseInput(const char* in, int& fc, int& fr, int& tc, int& tr) {
        if (strlen(in) != 5 || in[2] != ' ') return false;

        fc = in[0] - 'a';
        fr = '8' - in[1];
        tc = in[3] - 'a';
        tr = '8' - in[4];

        return (fc >= 0 && fc < 8 && fr >= 0 && fr < 8 &&
            tc >= 0 && tc < 8 && tr >= 0 && tr < 8);
    }

public:
    Game() {
        turn = WHITE;
        over = false;
    }

    void showMenu() {
        system("cls");
        cout << "\n\n";
        cout << "  =============================\n";
        cout << "       CHESS GAME\n";
        cout << "  =============================\n\n";
        cout << "    1. Start New Game\n";
        cout << "    2. Exit\n\n";
        cout << "  =============================\n";
        cout << "  Enter your choice: ";
    }

    void showCheckmate() {
        system("cls");
        cout << "\n\n\n";
        cout << "  ============================="<<endl;
        cout << "       C H E C K M A T E !"<<endl;
        if (turn == WHITE) {
            cout << "       BLACK Wins: "<<endl;
        }
        else {
            cout << "       WHITE Wins!"<<endl;
        }
        cout << "\n  ============================="<<endl;
        cout << "\n  Press Enter for menu...";
        cin.get();
    }

    void showStalemate() {
        system("cls");
        cout << "";
        cout << "  ============================="<<endl;
        cout << "     S T A L E M A T E !"<<endl;
        cout << "     It's a Draw!"<<endl;
        cout << "\n  ============================="<<endl;
        cout << "\n  Press Enter for Menu..."<<endl;
        cin.get();
    }

    void start() {
        char input[100];

        board.showBoard();
        cout << "Game begins! White plays first."<<endl;

        while (!over) {
            // check game ending conditions
            if (board.isCheckmate(turn)) {
                board.showBoard();
                showCheckmate();
                break;
            }

            if (board.isStalemate(turn)) {
                board.showBoard();
                showStalemate();
                break;
            }

            // warn about check
            if (board.isInCheck(turn)) {
                cout << "CHECK! Your king is under attack!\n\n";
            }

            // get player move
            if (turn == WHITE) {
                cout << "White ki move: ";
            }
            else {
                cout << "Black ki move: ";
            }
            cin.getline(input, 100);

            if (strcmp(input, "quit") == 0) {
                cout << "Game ended.\nPress Enter for menu.";
                cin.get();
                break;
            }

            int fc, fr, tc, tr;
            if (!parseInput(input, fc, fr, tc, tr)) {
                cout << "Format is Wrong! Use like: a2  a3\n";
                cout << "Press Enter key..";
                cin.get();
                board.showBoard();
                continue;
            }

            Piece* piece = board.getPiece(fr, fc);
            if (piece == nullptr || piece->getColor() != turn) {
                cout << "either no piece or wrong color!\n";
                cout << "Press Enter...";
                cin.get();
                board.showBoard();
                continue;
            }

            if (!board.makeMove(fr, fc, tr, tc)) {
                cout << "Invalid move! Try again.\n";
                cout << "Press Enter...";
                cin.get();
                board.showBoard();
                continue;
            }

            // change player
            if (turn == WHITE) {
                turn = BLACK;
            }
            else {
                turn = WHITE;
            }
            board.showBoard();
            
        }
    }
};

// main function
int main() {
    char choice[10];
    bool run = true;

    while (run) {
        Game g;
        g.showMenu();
        cin.getline(choice, 10);

        if (choice[0] == '1') {
            g.start();
        }
        else if (choice[0] == '2') {
            system("cls");
            cout << "Thank you for playing!"<<endl;
            run = false;
        }
        else {
            cout << "Option is wrong. press enter: "<<endl;
            cin.get();
        }
    }

    return 0;
}
