/**
 * @file Square.cpp
 * @author John Korreck
 */

#include "pch.h"

#include "Square.h"

#include "Piece.h"

/// Size of each square
const int squareSize = 75;

Square::Square(const std::wstring& name) : PolyDrawable(name)
{
}

void Square::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
 PolyDrawable::Draw(graphics);
}

void Square::SetPiece(Piece* piece)
{
 mPiece = piece;
 if (piece)
 {
  piece->SetSquare(this);
 }
}