/**
 * @file BoardFactory.cpp
 * @author John Korreck
 */
 
#include "pch.h"
#include "BoardFactory.h"

#include "Board.h"
#include "Item.h"
#include "Square.h"
#include "PolyDrawable.h"
#include "ImageDrawable.h"
#include "Piece.h"

/// Size of each square
const int squareSize = 75;

/// Directory within resources that contains the images.
const std::wstring ImagesDirectory = L"/images";

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

std::shared_ptr<Board> BoardFactory::Create(std::wstring resourcesDir)
{
    auto imagesDir = resourcesDir + ImagesDirectory;
    std::wstring name = L"Board";
    // Create the board item
    auto board = std::make_shared<Board>(name, imagesDir);
    board->SetRelativePosition(wxPoint(250, 100));

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++)
        {
            // Determine if the square is light or dark
            bool isLightSquare = (file + rank) % 2 != 0;
            std::wstring name = L"Square" + std::to_wstring(file) + L"_" + std::to_wstring(rank);
            // Create a drawable polygon for the square
            auto square = std::make_shared<Square>(name);
            // square->SetClickable(false);

            // Set color based on the type of square
            if (!isLightSquare) {
                square->SetColor(wxColour(240, 217, 181)); // Light square color
            }
            else {
                square->SetColor(wxColour(181, 136, 99));  // Dark square color
            }

            // Position the square on the board
            int x = board->GetRelativePosition().x + file * squareSize;
            int y = board->GetRelativePosition().y + rank * squareSize;
            square->SetPosition(wxPoint(x + squareSize/2, y + squareSize/2));
            square->SetActor(&*board);

            // Define the square's four corners
            square->AddPoint(wxPoint(x, y));
            square->AddPoint(wxPoint(x, y + squareSize));
            square->AddPoint(wxPoint(x + squareSize, y + squareSize));
            square->AddPoint(wxPoint(x + squareSize, y));
            square->SetCenter(wxPoint(x + squareSize/2, y + squareSize/2));

            // Add the square and piece to the board item
            board->AddDrawable(square);
            board->AddSquare(square);
        }
  }

        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {
                std::wstring pieceName;

                // Position the square on the board
                int x = board->GetRelativePosition().x + file * squareSize;
                int y = board->GetRelativePosition().y + rank * squareSize;


                switch (board->GetBoard()[rank][file]) {
                case WHITE_PAWN: pieceName = L"/Chess_plt60.png"; break;
                case WHITE_KNIGHT: pieceName = L"/Chess_nlt60.png"; break;
                case WHITE_BISHOP: pieceName = L"/Chess_blt60.png"; break;
                case WHITE_ROOK: pieceName = L"/Chess_rlt60.png"; break;
                case WHITE_QUEEN: pieceName = L"/Chess_qlt60.png"; break;
                case WHITE_KING: pieceName = L"/Chess_klt60.png"; break;
                case BLACK_PAWN: pieceName = L"/Chess_pdt60.png"; break;
                case BLACK_KNIGHT: pieceName = L"/Chess_ndt60.png"; break;
                case BLACK_BISHOP: pieceName = L"/Chess_bdt60.png"; break;
                case BLACK_ROOK: pieceName = L"/Chess_rdt60.png"; break;
                case BLACK_QUEEN: pieceName = L"/Chess_qdt60.png"; break;
                case BLACK_KING: pieceName = L"/Chess_kdt60.png"; break;
                default: pieceName = L"EMPTY"; break;
                }

                if (pieceName != "EMPTY")
                {
                    auto piece = std::make_shared<Piece>(std::to_wstring(board->GetBoard()[rank][file]), imagesDir + pieceName);
                    piece->SetPosition(wxPoint(x, y));
                    piece->SetActor(&*board);
                    board->AddDrawable(piece);
                    board->AddPiece(piece);
                    board->GetSquares()[rank*8 + file]->SetPiece(&*piece);
                }
            }
 }
    return board;
}
