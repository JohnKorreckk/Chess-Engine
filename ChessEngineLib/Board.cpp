/**
 * @file Board.cpp
 * @author John Korreck
 */

#include "pch.h"

#include "Board.h"
#include "ImageDrawable.h"
#include "Piece.h"
#include "Square.h"
#include "Item.h"
#include "PolyDrawable.h"

/// Size of each square
const int squareSize = 75;

/// Directory within resources that contains the images.
const std::wstring ImagesDirectory = L"/images";

// Binary representations for pieces

// Uncolored pieces
const int EMPTY = 0;
const int KING = 1;
const int PAWN = 2;
const int KNIGHT = 3;
const int BISHOP = 4;
const int ROOK = 5;
const int QUEEN = 6;

// Color values
const int WHITE = 8;
const int BLACK = 16;

// White pieces (uppercase)
const int WHITE_KING = WHITE + KING;
const int WHITE_PAWN = WHITE + PAWN;
const int WHITE_KNIGHT = WHITE + KNIGHT;
const int WHITE_BISHOP = WHITE + BISHOP;
const int WHITE_ROOK = WHITE + ROOK;
const int WHITE_QUEEN = WHITE + QUEEN;

// Black pieces (lowercase)
const int BLACK_KING = BLACK + KING;
const int BLACK_PAWN = BLACK + PAWN;
const int BLACK_KNIGHT = BLACK + KNIGHT;
const int BLACK_BISHOP = BLACK + BISHOP;
const int BLACK_ROOK = BLACK + ROOK;
const int BLACK_QUEEN = BLACK + QUEEN;

Board::Board(std::wstring& name, std::wstring resourcesDir) : Item(name)
{
    mBoard = FenParser(mChessPosition);
    for (auto file : mBoard)
    {
        for (auto item : file)
        {
            std::cout << item << " | ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> Board::FenParser(std::wstring fenString)
{
    // Initialize the board with empty squares (represented as 0)
    std::vector<std::vector<int>> board(8, std::vector<int>(8, 0));

    int file = 0;  // File (column) - 0 to 7
    int rank = 0;  // Rank (row) - 7 to 0 (because FEN starts from rank 8 down to rank 1)

    // Iterate through the FEN string
    for (auto letter : fenString)
    {
        if (letter == ' ') {
            break;  // Skip the rest of the FEN string after the piece placement part
        }

        if (letter == '/') {
            // Move to the next rank (row)
            rank++;
            file = 0;
        }
        else if (isdigit(letter)) {
            // Empty squares are represented by numbers (1-8)
            int emptySquares = letter - '0';
            while (emptySquares > 0) {
                board[rank][file] = 0;  // Set to empty square (binary 00000000)
                file++;
                emptySquares--;
            }
        }
        else {
            // Piece representation
            int piece = 0;

            switch (letter) {
                case 'P': piece = WHITE_PAWN; break;
                case 'N': piece = WHITE_KNIGHT; break;
                case 'B': piece = WHITE_BISHOP; break;
                case 'R': piece = WHITE_ROOK; break;
                case 'Q': piece = WHITE_QUEEN; break;
                case 'K': piece = WHITE_KING; break;
                case 'p': piece = BLACK_PAWN; break;
                case 'n': piece = BLACK_KNIGHT; break;
                case 'b': piece = BLACK_BISHOP; break;
                case 'r': piece = BLACK_ROOK; break;
                case 'q': piece = BLACK_QUEEN; break;
                case 'k': piece = BLACK_KING; break;
            default: ;
            }

            // Set the piece at the current position in the board (file, rank)
            board[rank][file] = piece;
            file++;
        }
    }

    return board;
}

/**
* Returns the closest square to the position.
* @param pos Mouse position on drawing
* @return The closest square to the position
*/
std::shared_ptr<Square> Board::GetClosestSquare(wxPoint pos)
{
    std::shared_ptr<Square> closestSquare;
    double minDistance = INT_MAX;
    for (auto const &square : mSquares)
    {
        double newDistance = sqrt(pow((pos.x - square->GetCenter().x),2) + pow(pos.y - square->GetCenter().y,2));
        if (newDistance < minDistance)
        {
            minDistance = newDistance;
            closestSquare = square;
        }
    }
    return closestSquare;
}

void Board::GeneratePossibleMoves()
{
    mPossibleMoves.clear();
    for (int file = 0; file < 8; file++) {
        for (int rank = 0; rank < 8; rank++) {

            int pieceNum = mBoard[file][rank];
            std::wstring currentFile = std::to_wstring(std::abs(file - 8));
            std::wstring currentRank = std::wstring(1, 'a' + rank);
            std::wstring currentSquare = currentRank + currentFile;

            if (mWhiteTurn && (pieceNum & WHITE) != 0)
            {
                if (pieceNum == WHITE_PAWN)
                {
                    GeneratePawnMoves(pieceNum, file, rank, currentSquare);
                }
                if (pieceNum == WHITE_KING)
                {
                    GenerateKingMoves(pieceNum, file, rank, currentSquare);
                }
                if (pieceNum == WHITE_KNIGHT)
                {
                    GenerateKnightMoves(pieceNum, file, rank, currentSquare);
                }
                if (pieceNum == WHITE_BISHOP)
                {
                    GenerateDiagonalMoves(pieceNum, file, rank, currentSquare);
                }
                if (pieceNum == WHITE_ROOK)
                {
                    GenerateSlidingMoves(pieceNum, file, rank, currentSquare);
                }
                if (pieceNum == WHITE_QUEEN)
                {
                    GenerateSlidingMoves(pieceNum, file, rank, currentSquare);
                }

            }
            else if (!mWhiteTurn && (pieceNum & BLACK) != 0)
            {
                if (pieceNum == BLACK_PAWN)
                {
                    GeneratePawnMoves(pieceNum, file, rank, currentSquare);
                }
                if (pieceNum == BLACK_KING)
                {
                    GenerateKingMoves(pieceNum, file, rank, currentSquare);
                }
                if (pieceNum == BLACK_KNIGHT)
                {
                    GenerateKnightMoves(pieceNum, file, rank, currentSquare);
                }
                if (pieceNum == BLACK_BISHOP)
                {
                    GenerateDiagonalMoves(pieceNum, file, rank, currentSquare);
                }
                if (pieceNum == BLACK_ROOK)
                {
                    GenerateSlidingMoves(pieceNum, file, rank, currentSquare);
                }
                if (pieceNum == BLACK_QUEEN)
                {
                    GenerateSlidingMoves(pieceNum, file, rank, currentSquare);
                }
            }
        }
    }
}

std::shared_ptr<Piece> Board::HitTest(wxPoint pos)
{
    for (auto piece : mPieces)
    {
        if (piece->HitTest(pos))
        {
            return piece;
        }
    }
    return nullptr;
}

void Board::UpdateBoard(std::wstring const &move)
{
    int oldRank = move[0] - 'a';
    int oldFile = (move[1] - '0') - 8;
    int newRank = move[2] - 'a';
    int newFile = (move[3] - '0') - 8;

    oldFile = std::abs(oldFile);
    newFile = std::abs(newFile);

    int piece = mBoard[oldFile][oldRank];

    mBoard[oldFile][oldRank] = 0;
    mBoard[newFile][newRank] = piece;

    for (auto rank : mBoard)
    {
        for (auto file : rank)
        {
            std::cout << file << " | ";
        }
        std::cout << std::endl;
    }
}

bool Board::CheckBounds(int file, int rank)
{
    if (file < 0 || rank < 0 || file >= 8 || rank >= 8)
    {
        return false;
    }
    return true;
}

void Board::GenerateKingMoves(int const &pieceNum, int const &file, int const &rank, std::wstring const &currentSquare)
{
    if (pieceNum == WHITE_KING)
    {
         // Up-Right diagonal
         if (CheckBounds(file + 1, rank + 1) && (mBoard[file + 1][rank + 1] & BLACK || mBoard[file + 1][rank + 1] == 0))
         {
             std::wstring upRight = currentSquare;
             upRight[1] = currentSquare[1] - 1;
             upRight[0] = currentSquare[0] + 1;
             mPossibleMoves.push_back(currentSquare + upRight);
         }

         // Up-Left diagonal
         if (CheckBounds(file + 1, rank - 1) && (mBoard[file + 1][rank - 1] & BLACK || mBoard[file + 1][rank - 1] == 0))
         {
             std::wstring upLeft = currentSquare;
             upLeft[1] = currentSquare[1] - 1;
             upLeft[0] = currentSquare[0] - 1;
             mPossibleMoves.push_back(currentSquare + upLeft);
         }

         // Down-Right diagonal
         if (CheckBounds(file - 1, rank + 1) && (mBoard[file - 1][rank + 1] & BLACK || mBoard[file - 1][rank + 1] == 0))
         {
             std::wstring downRight = currentSquare;
             downRight[1] = currentSquare[1] + 1;
             downRight[0] = currentSquare[0] + 1;
             mPossibleMoves.push_back(currentSquare + downRight);
         }

         // Down-Left diagonal
         if (CheckBounds(file - 1, rank - 1) && (mBoard[file - 1][rank - 1] & BLACK || mBoard[file - 1][rank - 1] == 0))
         {
             std::wstring downLeft = currentSquare;
             downLeft[1] = currentSquare[1] + 1;
             downLeft[0] = currentSquare[0] - 1;
             mPossibleMoves.push_back(currentSquare + downLeft);
         }

        // Up
        if (CheckBounds(file + 1, rank) && (mBoard[file + 1][rank] & BLACK || mBoard[file + 1][rank] == 0))
        {
            std::wstring up = currentSquare;
            up[1] = currentSquare[1] - 1;
            mPossibleMoves.push_back(currentSquare + up);
        }

        // Down
        if (CheckBounds(file - 1, rank) && (mBoard[file - 1][rank] & BLACK || mBoard[file - 1][rank] == 0))
        {
            std::wstring down = currentSquare;
            down[1] = currentSquare[1] + 1;
            mPossibleMoves.push_back(currentSquare + down);
        }

        // Right
        if (CheckBounds(file, rank + 1) && (mBoard[file][rank + 1] & BLACK || mBoard[file][rank + 1] == 0))
        {
            std::wstring right = currentSquare;
            right[0] = currentSquare[0] + 1;
            mPossibleMoves.push_back(currentSquare + right);
        }

        // Left
        if (CheckBounds(file, rank - 1) && (mBoard[file][rank - 1] & BLACK || mBoard[file][rank - 1] == 0))
        {
            std::wstring left = currentSquare;
            left[0] = currentSquare[0] - 1;
            mPossibleMoves.push_back(currentSquare + left);
        }
    }
    else if (pieceNum == BLACK_KING)
    {
         // Up-Right diagonal
         if (CheckBounds(file + 1, rank + 1) && (mBoard[file + 1][rank + 1] & WHITE || mBoard[file + 1][rank + 1] == 0))
         {
             std::wstring upRight = currentSquare;
             upRight[1] = currentSquare[1] - 1;
             upRight[0] = currentSquare[0] + 1;
             mPossibleMoves.push_back(currentSquare + upRight);
         }

         // Up-Left diagonal
         if (CheckBounds(file + 1, rank - 1) && (mBoard[file + 1][rank - 1] & WHITE || mBoard[file + 1][rank - 1] == 0))
         {
             std::wstring upLeft = currentSquare;
             upLeft[1] = currentSquare[1] - 1;
             upLeft[0] = currentSquare[0] - 1;
             mPossibleMoves.push_back(currentSquare + upLeft);
         }

         // Down-Right diagonal
         if (CheckBounds(file - 1, rank + 1) && (mBoard[file - 1][rank + 1] & WHITE || mBoard[file - 1][rank + 1] == 0))
         {
             std::wstring downRight = currentSquare;
             downRight[1] = currentSquare[1] + 1;
             downRight[0] = currentSquare[0] + 1;
             mPossibleMoves.push_back(currentSquare + downRight);
         }

         // Down-Left diagonal
         if (CheckBounds(file - 1, rank - 1) && (mBoard[file - 1][rank - 1] & WHITE || mBoard[file - 1][rank - 1] == 0))
         {
             std::wstring downLeft = currentSquare;
             downLeft[1] = currentSquare[1] + 1;
             downLeft[0] = currentSquare[0] - 1;
             mPossibleMoves.push_back(currentSquare + downLeft);
         }

        // Up
        if (CheckBounds(file + 1, rank) && (mBoard[file + 1][rank] & WHITE || mBoard[file + 1][rank] == 0))
        {
            std::wstring up = currentSquare;
            up[1] = currentSquare[1] - 1;
            mPossibleMoves.push_back(currentSquare + up);
        }

        // Down
        if (CheckBounds(file - 1, rank) && (mBoard[file - 1][rank] & WHITE || mBoard[file - 1][rank] == 0))
        {
            std::wstring down = currentSquare;
            down[1] = currentSquare[1] + 1;
            mPossibleMoves.push_back(currentSquare + down);
        }

        // Right
        if (CheckBounds(file, rank + 1) && (mBoard[file][rank + 1] & WHITE || mBoard[file][rank + 1] == 0))
        {
            std::wstring right = currentSquare;
            right[0] = currentSquare[0] + 1;
            mPossibleMoves.push_back(currentSquare + right);
        }

        // Left
        if (CheckBounds(file, rank - 1) && (mBoard[file][rank - 1] & WHITE || mBoard[file][rank - 1] == 0))
        {
            std::wstring left = currentSquare;
            left[0] = currentSquare[0] - 1;
            mPossibleMoves.push_back(currentSquare + left);
        }
    }
}

void Board::GeneratePawnMoves(int const & pieceNum, int const &file, int const &rank, std::wstring const &currentSquare)
{
    if (pieceNum == WHITE_PAWN)
    {
        if (CheckBounds(file - 1, rank) && mBoard[file - 1][rank] == 0)
        {
            // White pawn advances one
            std::wstring oneSquareUp = currentSquare;
            oneSquareUp[1] = currentSquare[1] + 1;  // Move up one rank
            mPossibleMoves.push_back(currentSquare + oneSquareUp);
        }
        // White pawn advances two
        if (CheckBounds(file - 2, rank) && currentSquare[1] == '2' && mBoard[file - 2][rank] == 0)
        {
            std::wstring twoSquaresUp = currentSquare;
            twoSquaresUp[1] = currentSquare[1] + 2;  // Move up two ranks
            mPossibleMoves.push_back(currentSquare + twoSquaresUp);
        }
        // White pawn captures piece left diagonal
        if (CheckBounds(file - 1, rank - 1) && mBoard[file - 1][rank - 1] != 0 && (mBoard[file - 1][rank - 1] & WHITE) == 0)
        {
            std::wstring upLeft = currentSquare;
            upLeft[1] = currentSquare[1] + 1;
            upLeft[0] = currentSquare[0] - 1;
            mPossibleMoves.push_back(currentSquare + upLeft);
        }

        // White pawn captures piece right diagonal
        if (CheckBounds(file - 1, rank + 1) && mBoard[file - 1][rank + 1] != 0 && (mBoard[file - 1][rank + 1] & WHITE) == 0)
        {
            std::wstring upRight = currentSquare;
            upRight[1] = currentSquare[1] + 1;
            upRight[0] = currentSquare[0] + 1;
            mPossibleMoves.push_back(currentSquare + upRight);
        }
    }

    else if (pieceNum == BLACK_PAWN)
    {
        // Black pawn advances one
        if (CheckBounds(file + 1, rank) && mBoard[file + 1][rank] == 0)
        {
            std::wstring oneSquareDown = currentSquare;
            oneSquareDown[1] = currentSquare[1] - 1;  // Move down one rank
            mPossibleMoves.push_back(currentSquare + oneSquareDown);
        }

        // Black pawn advances two
        if (CheckBounds(file + 2, rank) && currentSquare[1] == '7' && mBoard[file + 2][rank] == 0)
        {
            std::wstring twoSquaresDown = currentSquare;
            twoSquaresDown[1] = currentSquare[1] - 2;  // Move down two ranks
            mPossibleMoves.push_back(currentSquare + twoSquaresDown);
        }

        // Black pawn captures piece left diagonal
        if (CheckBounds(file + 1, rank - 1) && mBoard[file + 1][rank - 1] != 0 && (mBoard[file + 1][rank - 1] & BLACK) == 0)
        {
            std::wstring upLeft = currentSquare;
            upLeft[1] = currentSquare[1] - 1;  // Move down two ranks
            upLeft[0] = currentSquare[0] - 1;
            mPossibleMoves.push_back(currentSquare + upLeft);
        }

        // Black pawn captures piece right diagonal
        if (CheckBounds(file + 1, rank + 1) && mBoard[file + 1][rank + 1] != 0 && (mBoard[file + 1][rank + 1] & BLACK) == 0)
        {
            std::wstring upRight = currentSquare;
            upRight[1] = currentSquare[1] - 1;  // Move down two ranks
            upRight[0] = currentSquare[0] + 1;
            mPossibleMoves.push_back(currentSquare + upRight);
        }
    }
}

void Board::GenerateKnightMoves(int const &pieceNum, int const &file, int const &rank, std::wstring const &currentSquare)
{
    if (pieceNum == WHITE_KNIGHT)
    {
        // Up 2 Right 1
        if (CheckBounds(file + 2, rank + 1) && (mBoard[file + 2][rank + 1] & BLACK || mBoard[file + 2][rank + 1] == 0))
        {
            std::wstring upRight = currentSquare;
            upRight[1] = currentSquare[1] - 2;
            upRight[0] = currentSquare[0] + 1;
            mPossibleMoves.push_back(currentSquare + upRight);
        }
        // Up 2 Left 1
        if (CheckBounds(file + 2, rank - 1) && (mBoard[file + 2][rank - 1] & BLACK || mBoard[file + 2][rank - 1] == 0))
        {
            std::wstring upLeft = currentSquare;
            upLeft[1] = currentSquare[1] - 2;
            upLeft[0] = currentSquare[0] - 1;
            mPossibleMoves.push_back(currentSquare + upLeft);
        }
        // Down 2 Right 1
        if (CheckBounds(file - 2, rank + 1) && (mBoard[file - 2][rank + 1] & BLACK || mBoard[file - 2][rank + 1] == 0))
        {
            std::wstring downRight = currentSquare;
            downRight[1] = currentSquare[1] + 2;
            downRight[0] = currentSquare[0] + 1;
            mPossibleMoves.push_back(currentSquare + downRight);
        }
        // Down 2 Left 1
        if (CheckBounds(file - 2, rank - 1) && (mBoard[file - 2][rank - 1] & BLACK || mBoard[file - 2][rank - 1] == 0))
        {
            std::wstring downLeft = currentSquare;
            downLeft[1] = currentSquare[1] + 2;
            downLeft[0] = currentSquare[0] - 1;
            mPossibleMoves.push_back(currentSquare + downLeft);
        }

        // Up 1 Right 2
        if (CheckBounds(file + 1, rank + 2) && (mBoard[file + 1][rank + 2] & BLACK || mBoard[file + 1][rank + 2] == 0))
        {
            std::wstring upRight = currentSquare;
            upRight[1] = currentSquare[1] - 1;
            upRight[0] = currentSquare[0] + 2;
            mPossibleMoves.push_back(currentSquare + upRight);
        }
        // Up 1 Left 2
        if (CheckBounds(file + 1, rank - 2) && (mBoard[file + 1][rank - 2] & BLACK || mBoard[file + 1][rank - 2] == 0))
        {
            std::wstring upLeft = currentSquare;
            upLeft[1] = currentSquare[1] - 1;
            upLeft[0] = currentSquare[0] - 2;
            mPossibleMoves.push_back(currentSquare + upLeft);
        }
        // Down 1 Right 2
        if (CheckBounds(file - 1, rank + 2) && (mBoard[file - 1][rank + 2] & BLACK || mBoard[file - 1][rank + 2] == 0))
        {
            std::wstring downRight = currentSquare;
            downRight[1] = currentSquare[1] + 1;
            downRight[0] = currentSquare[0] + 2;
            mPossibleMoves.push_back(currentSquare + downRight);
        }
        // Down 1 Left 2
        if (CheckBounds(file - 1, rank - 2) && (mBoard[file - 1][rank - 2] & BLACK || mBoard[file - 1][rank - 2] == 0))
        {
            std::wstring downLeft = currentSquare;
            downLeft[1] = currentSquare[1] + 1;
            downLeft[0] = currentSquare[0] - 2;
            mPossibleMoves.push_back(currentSquare + downLeft);
        }
    }
    else if (pieceNum == BLACK_KNIGHT)
    {
        // Up 2 Right 1
        if (CheckBounds(file + 2, rank + 1) && (mBoard[file + 2][rank + 1] & WHITE || mBoard[file + 2][rank + 1] == 0))
        {
            std::wstring upRight = currentSquare;
            upRight[1] = currentSquare[1] - 2;
            upRight[0] = currentSquare[0] + 1;
            mPossibleMoves.push_back(currentSquare + upRight);
        }
        // Up 2 Left 1
        if (CheckBounds(file + 2, rank - 1) && (mBoard[file + 2][rank - 1] & WHITE || mBoard[file + 2][rank - 1] == 0))
        {
            std::wstring upLeft = currentSquare;
            upLeft[1] = currentSquare[1] - 2;
            upLeft[0] = currentSquare[0] - 1;
            mPossibleMoves.push_back(currentSquare + upLeft);
        }
        // Down 2 Right 1
        if (CheckBounds(file - 2, rank + 1) && (mBoard[file - 2][rank + 1] & WHITE || mBoard[file - 2][rank + 1] == 0))
        {
            std::wstring downRight = currentSquare;
            downRight[1] = currentSquare[1] + 2;
            downRight[0] = currentSquare[0] + 1;
            mPossibleMoves.push_back(currentSquare + downRight);
        }
        // Down 2 Left 1
        if (CheckBounds(file - 2, rank - 1) && (mBoard[file - 2][rank - 1] & WHITE || mBoard[file - 2][rank - 1] == 0))
        {
            std::wstring downLeft = currentSquare;
            downLeft[1] = currentSquare[1] + 2;
            downLeft[0] = currentSquare[0] - 1;
            mPossibleMoves.push_back(currentSquare + downLeft);
        }

        // Up 1 Right 2
        if (CheckBounds(file + 1, rank + 2) && (mBoard[file + 1][rank + 2] & WHITE || mBoard[file + 1][rank + 2] == 0))
        {
            std::wstring upRight = currentSquare;
            upRight[1] = currentSquare[1] - 1;
            upRight[0] = currentSquare[0] + 2;
            mPossibleMoves.push_back(currentSquare + upRight);
        }
        // Up 1 Left 2
        if (CheckBounds(file + 1, rank - 2) && (mBoard[file + 1][rank - 2] & WHITE || mBoard[file + 1][rank - 2] == 0))
        {
            std::wstring upLeft = currentSquare;
            upLeft[1] = currentSquare[1] - 1;
            upLeft[0] = currentSquare[0] - 2;
            mPossibleMoves.push_back(currentSquare + upLeft);
        }
        // Down 1 Right 2
        if (CheckBounds(file - 1, rank + 2) && (mBoard[file - 1][rank + 2] & WHITE || mBoard[file - 1][rank + 2] == 0))
        {
            std::wstring downRight = currentSquare;
            downRight[1] = currentSquare[1] + 1;
            downRight[0] = currentSquare[0] + 2;
            mPossibleMoves.push_back(currentSquare + downRight);
        }
        // Down 1 Left 2
        if (CheckBounds(file - 1, rank - 2) && (mBoard[file - 1][rank - 2] & WHITE || mBoard[file - 1][rank - 2] == 0))
        {
            std::wstring downLeft = currentSquare;
            downLeft[1] = currentSquare[1] + 1;
            downLeft[0] = currentSquare[0] - 2;
            mPossibleMoves.push_back(currentSquare + downLeft);
        }
    }
}

void Board::GenerateSlidingMoves(int const &pieceNum, int const &file, int const &rank, std::wstring const &currentSquare)
{
    if (pieceNum & WHITE)
    {
        int currentFile = file;
        int currentRank = rank;
        if (pieceNum == WHITE_ROOK || pieceNum == WHITE_QUEEN)
        {
            std::wstring slidingMove = currentSquare;
            currentFile--;
            while (currentFile >= 0)
            {
                slidingMove[1] = slidingMove[1] + 1;

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                currentFile--;
            }

            currentFile = file;
            slidingMove = currentSquare;
            currentFile++;
            while (currentFile <= 7)
            {
                slidingMove[1] = slidingMove[1] - 1;

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                currentFile++;
            }
            currentFile = file;
            currentRank--;
            slidingMove = currentSquare;

            while (currentRank >= 0)
            {
                slidingMove[0] = slidingMove[0] - 1;

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                currentRank--;
            }
            currentRank = rank;
            slidingMove = currentSquare;
            currentRank++;

            while (currentRank <= 7)
            {
                slidingMove[0] = slidingMove[0] + 1;

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                currentRank++;
            }
        }
    }
    else if (pieceNum & BLACK)
    {
        int currentFile = file;
        int currentRank = rank;
        if (pieceNum == BLACK_ROOK || pieceNum == BLACK_QUEEN)
        {
            std::wstring slidingMove = currentSquare;
            currentFile--;
            while (currentFile >= 0)
            {
                slidingMove[1] = slidingMove[1] + 1;

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                currentFile--;
            }

            currentFile = file;
            slidingMove = currentSquare;
            currentFile++;
            while (currentFile <= 7)
            {
                slidingMove[1] = slidingMove[1] - 1;

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                currentFile++;
            }
            currentFile = file;
            currentRank--;
            slidingMove = currentSquare;

            while (currentRank >= 0)
            {
                slidingMove[0] = slidingMove[0] - 1;

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                currentRank--;
            }
            currentRank = rank;
            slidingMove = currentSquare;
            currentRank++;

            while (currentRank <= 7)
            {
                slidingMove[0] = slidingMove[0] + 1;

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                currentRank++;
            }
        }
    }
}

void Board::GenerateDiagonalMoves(int const &pieceNum, int const &file, int const &rank, std::wstring const &currentSquare)
{
    if (pieceNum & WHITE)
    {
        int currentFile = file;
        int currentRank = rank;
        if (pieceNum == WHITE_BISHOP)
        {
            std::wstring slidingMove = currentSquare;
            currentRank--;
            currentFile--;
            while (currentFile >= 0 && currentRank >= 0)
            {
                slidingMove[0] = slidingMove[0] - 1;
                slidingMove[1] = slidingMove[1] + 1;
                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                currentFile--;
                currentRank--;
            }
            currentFile = file;
            currentRank = rank;

            slidingMove = currentSquare;

            currentFile++;
            currentRank--;
            while (currentFile <= 7 && currentRank >= 0)
            {
                slidingMove[0] = slidingMove[0] - 1;
                slidingMove[1] = slidingMove[1] - 1;
                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                currentFile++;
                currentRank--;
            }
            currentFile = file;
            currentRank = rank;

            slidingMove = currentSquare;

            currentRank++;
            currentFile--;
            while (currentFile >= 0 && currentRank <= 7)
            {
                slidingMove[0] = slidingMove[0] + 1;
                slidingMove[1] = slidingMove[1] + 1;
                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                currentFile--;
                currentRank++;
            }
            currentFile = file;
            currentRank = rank;

            slidingMove = currentSquare;

            currentRank++;
            currentFile++;
            while (currentFile <= 7 && currentRank <= 7)
            {
                slidingMove[0] = slidingMove[0] + 1;
                slidingMove[1] = slidingMove[1] - 1;
                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                currentFile++;
                currentRank++;
            }
        }
    }

    if (pieceNum & BLACK)
    {
        int currentFile = file;
        int currentRank = rank;
        if (pieceNum == BLACK_BISHOP)
        {
            std::wstring slidingMove = currentSquare;
            currentRank--;
            currentFile--;
            while (currentFile >= 0 && currentRank >= 0)
            {
                slidingMove[0] = slidingMove[0] - 1;
                slidingMove[1] = slidingMove[1] + 1;
                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                currentFile--;
                currentRank--;
            }
            currentFile = file;
            currentRank = rank;

            slidingMove = currentSquare;

            currentFile++;
            currentRank--;
            while (currentFile <= 7 && currentRank >= 0)
            {
                slidingMove[0] = slidingMove[0] - 1;
                slidingMove[1] = slidingMove[1] - 1;
                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                currentFile++;
                currentRank--;
            }
            currentFile = file;
            currentRank = rank;

            slidingMove = currentSquare;

            currentRank++;
            currentFile--;
            while (currentFile >= 0 && currentRank <= 7)
            {
                slidingMove[0] = slidingMove[0] + 1;
                slidingMove[1] = slidingMove[1] + 1;
                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                currentFile--;
                currentRank++;
            }
            currentFile = file;
            currentRank = rank;

            slidingMove = currentSquare;

            currentRank++;
            currentFile++;
            while (currentFile <= 7 && currentRank <= 7)
            {
                slidingMove[0] = slidingMove[0] + 1;
                slidingMove[1] = slidingMove[1] - 1;
                if (mBoard[currentFile][currentRank] & BLACK)
                {
                    break;
                }
                mPossibleMoves.push_back(currentSquare + slidingMove);

                if (mBoard[currentFile][currentRank] & WHITE)
                {
                    break;
                }
                currentFile++;
                currentRank++;
            }
        }
    }
}