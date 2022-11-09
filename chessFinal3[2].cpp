//These are the main data structures used
#include <map>
#include <vector>
#include <string>
#include<bits/stdc++.h>

using namespace std;

//main structure of the chess-board
struct ChessBoard
{
    //defining class as a custom data type
    enum class Piece {king, queen, white_pawn, black_pawn, rook, bishop, knight};
    enum class Turn {white, black} turn;

    //designing the structure to help in mapping the positions of the characters
    struct Pos
    {
        int x,y;
        Pos(const Pos &p, int dx=0, int dy=0)
        { 
            *this = p; 
            x+=dx;
            y+=dy;
        }
        Pos(int a, int b)
        {
            x=a;
            y=b;  
        }
        Pos()
        {
            x=-1;
            y=-1;
        }
        //made custom operators (operator overloading)
        bool operator<(const Pos & p) const 
        { 
            return (x < p.x) || (x==p.x && y < p.y); 
        }
        bool operator==(const Pos & p) const 
        { 
            return x==p.x && y==p.y; 
        }
    };

    struct Move
    {
        Pos from,to;
    };

    map<Pos,Piece> white_pieces, black_pieces;

    //player piece, uss pos p jo player ka gitti aygi usse map karega
    map<Pos,Piece> & playerPieces()
    { 
        return turn == Turn::white ? white_pieces : black_pieces; 
    }
    map<Pos,Piece> & opponentPieces()
    { 
        return turn == Turn::white ? black_pieces : white_pieces; 
    }

    void flipTurn()
    { 
        turn = turn == Turn::white? Turn::black : Turn::white; 
    }

    //func to print the chess board
    void printBoard()
    {
        static map<Piece, char> characters =
         { 
            {Piece::white_pawn,'P'},
            {Piece::black_pawn,'p'},
            {Piece::rook,'R'},
            {Piece::knight,'N'},
            {Piece::bishop,'B'},
            {Piece::king,'K'},
            {Piece::queen,'Q'}
        };
        cout<<endl<<"        1     2     3     4     5     6     7     8   "<<endl;
        cout<<"      _____ _____ _____ _____ _____ _____ _____ _____ ";
        for(int y=1; y < 9; ++y)
        {
            cout<<endl<<"     |     |     |     |     |     |     |     |     |";
            cout<<endl<<"  "<<y<<"  ";
            for(int x=1; x < 9; ++x)
            {
                cout<<"|  ";
                if(white_pieces.count(Pos(x,y)))
                    cout<<characters[white_pieces[Pos(x,y)]];
                else if(black_pieces.count(Pos(x,y)))
                    cout<<(char)tolower(characters[black_pieces[Pos(x,y)]]);
                else 
                    cout<<" ";
                cout<<"  ";
            }
            cout<<"|  "<<y<<endl<<"     |_____|_____|_____|_____|_____|_____|_____|_____|";
        }
        cout<<endl<<endl<<"        1     2     3     4     5     6     7     8   "<<endl<<endl;
    }

    //func to reset the board to the original form
    void reset()
    {
        white_pieces.clear();
        black_pieces.clear();
        turn = Turn::white;
        for(int i=1; i < 9; ++i){
            white_pieces[Pos(i,7)]=Piece::white_pawn;
            black_pieces[Pos(i,2)]=Piece::black_pawn;
        }
        int n=1;
        for(auto piece : {Piece::rook,Piece::knight,Piece::bishop,Piece::king})
        {
            white_pieces[Pos(n,8)]=white_pieces[Pos(9-n,8)]=black_pieces[Pos(n,1)]=black_pieces[Pos(9-n,1)]=piece;
            ++n;
        }
        white_pieces[Pos(4,8)]=black_pieces[Pos(4,1)]=Piece::queen;
    }

    //func to find the all possible moves for the chosen piece
    vector<Pos> possibleMoves(const Pos & from)
    {
        vector<Pos> moves;
        //count returns 1 if element is present
        auto player1 = [&] (int dx, int dy) -> bool 
        {
            return playerPieces().count(Pos(from,dx,dy));
        };
        auto player2 = [&] (int dx, int dy) -> bool 
        { 
            return opponentPieces().count(Pos(from,dx,dy));
        };
        auto inside = [&] (int dx, int dy) -> bool 
        { 
            Pos p(from,dx,dy); return p.x < 9 && p.x > 0 && p.y < 9 && p.y > 0; 
        };
        auto empty = [&] (int dx, int dy) -> bool 
        { 
            return !player1(dx,dy) && inside(dx,dy) && !player2(dx,dy); 
        };

        auto addMove = [&] (int dx, int dy) -> bool 
        {
            if(empty(dx,dy) || player2(dx,dy))
            {
                moves.push_back(Pos(from,dx,dy));
                return true;
            }
            return false;
        };

        if(!player1(0,0))
            return moves;

        auto moving_piece = playerPieces()[from];
        switch(moving_piece)
        {
            case Piece::white_pawn:
                if(empty(0,-1)) addMove(0,-1);
                if(empty(0,-1) && empty(0,-2) && from.y==7) addMove(0,-2);
                if(player2(-1,-1)) addMove(-1,-1);
                if(player2(1,-1)) addMove(1,-1);
                break;

            case Piece::black_pawn:
                if(empty(0,1)) addMove(0,1);
                if(empty(0,1) && empty(0,2) && from.y==2) addMove(0,2);
                if(player2(-1,1)) addMove(-1,1);
                if(player2(1,1)) addMove(1,1);
                break;

            case Piece::knight:
                addMove(-2,-1); addMove(-2,1); addMove(2,-1); addMove(2,1);
                addMove(-1,-2); addMove(-1,2); addMove(1,-2); addMove(1,2);
                break;

            case Piece::king:
                for(auto dy : {-1,0,1})   
                for(auto dx : {-1,0,1})
                addMove(dy,dx);
                break;

            case Piece::queen:
            case Piece::rook:
                for(int n=1; n < 9 && addMove(0,n) && !player2(0,n); ++n);
                for(int n=1; n < 9 && addMove(0,-n) && !player2(0,-n); ++n);
                for(int n=1; n < 9 && addMove(n,0) && !player2(n,0); ++n);
                for(int n=1; n < 9 && addMove(-n,0) && !player2(-n,0); ++n);
                if(moving_piece != Piece::queen)
                break;

            case Piece::bishop:
                for(int n=1; n < 9 && addMove(n,n) && !player2(n,n); ++n);
                for(int n=1; n < 9 && addMove(n,-n) && !player2(n,-n); ++n);
                for(int n=1; n < 9 && addMove(-n,n) && !player2(-n,n); ++n);
                for(int n=1; n < 9 && addMove(-n,-n) && !player2(-n,-n); ++n);
                break;
        }

        return moves;
    }

    //func to choose the piece and make the move
    bool makeMove(Pos from, Pos to)
    {
        vector<Pos> allowed = possibleMoves(from);

        //if in find we reach the end this means its not present in the vector
        if(find(allowed.begin(), allowed.end(), to) == allowed.end())
            return false;

        opponentPieces().erase(to);
        playerPieces()[to]=playerPieces()[from];
        playerPieces().erase(from);
        
        //when pawn reaches the opp end, it becomes queen
        if((playerPieces()[to]==Piece::white_pawn || playerPieces()[to]==Piece::black_pawn) && (to.y == 1 || to.y == 8))
        playerPieces()[to]=Piece::queen;

        flipTurn();

        return true;
    }

    //fucn to print help
    void printHelp()
    {
        cout<<endl<<"* Enter h: Help or q: Quit"<<endl<<endl<<"* Here white are represented by uppercase letters and black by lowercase"<<endl<<endl<<"* Give Input like this: xyxy i.e. (from coordinate)(to coordinate)"<<endl<<endl;
    }

    //func to get the input of the coordinates to move the piece
    bool Input(){
        string move;
        invalidmove:
        if(turn==Turn::white)
            cout<<"White move: ";
        else
            cout<<"Black move: ";
        if(move == "")
            cin>>move;
        if(move == "q"){
            cout<<"Bass... Nikal gayi hawa XD"<<endl<<endl;
            return false;
        }
        if(move == "h"){
            printHelp();
            move="";
            goto invalidmove;
        }

        Pos from(-1,-1),to(-1,-1);
        if(move.length() == 4){
            from.x = move[0] - '0';
            from.y = move[1] - '0';
            to.x = move[2] - '0';
            to.y = move[3] - '0';
        }
        if(!makeMove(from,to)){
            cout<<"* Invalid move"<<endl;
            move = "";
            goto invalidmove;
        }
        printBoard();
        return true;
    }

    //func for check-Mate condition
    bool king_exist()
    {
        for(auto &p : playerPieces())

        //second will see the pieces from the map
        if(p.second == Piece::king)
        return true;
        return false;
    }
};

int main()
{
    ChessBoard cb;
    cout<<endl<<"______________||| CHESS PROJECT |||______________"<<endl;
    cb.printHelp();
    cout<<"* Enter s: start";
    bool flag=true;
    while(flag)
    {
        cb.reset();
        string choice = "";
        while(choice != "s" && choice != "q"){
            cout<<endl<<"* Enter choice: ";
            cin>>choice;
        }
        if(choice == "q"){
            cout<<"Bass... Nikal gayi hawa XD"<<endl;
            break;
        }
        cb.printBoard();

        while(flag = cb.Input())
        {
            if(!cb.king_exist())
            {
                if(cb.turn==cb.Turn::black)
                {
                    cout<<"!!! WINNER WINNER CHICKEN DINNER !!!"<<endl;
                    cout<<"!! White wins !!"<<endl;
                    cb.printHelp();
                    break;
                }
                else
                {
                    cout<<"!!! WINNER WINNER CHICKEN DINNER !!!"<<endl;
                    cout<<"!! Black wins !!"<<endl;
                    cb.printHelp();
                    break;
                }
            }
        }
    }
    return 0;   
}