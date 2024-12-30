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

void Board::GeneratePossibleMoves(std::vector<std::vector<int>> board, bool whiteTurn)
{

}

std::vector<int> Board::GenerateSlidingMoves(std::vector<std::vector<int>> board)
{
    std::vector<int> slidingMoves;
    return slidingMoves;
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
