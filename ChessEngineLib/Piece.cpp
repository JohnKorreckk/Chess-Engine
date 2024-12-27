/**
 * @file Piece.cpp
 * @author John Korreck
 */

#include "pch.h"

#include "Piece.h"

Piece::Piece(const std::wstring& name, const std::wstring& filename) :
ImageDrawable(name, filename)
{
}
