/**
 * @file Piece.h
 * @author John Korreck
 *
 *
 */
 
#ifndef PIECE_H
#define PIECE_H

#include "ImageDrawable.h"

class Square;

class Piece : public ImageDrawable {
private:
 /// The piece assigned to this square
 Square* mSquare = nullptr;
public:
 /**
  * @brief Constructor for the Piece class with name and filename.
  * @param name The name of the piece.
  * @param filename The file path for the image.
  */
 Piece(const std::wstring& name, const std::wstring& filename);

 Piece() = delete;

 /** Copy constructor (disabled) */
 Piece(const Drawable &) = delete;

 /** Assignment operator */
 void operator=(const Piece &) = delete;

 bool IsMovable() override {return true;}

 void SetSquare(Square* square) { mSquare = square; }

 Square* GetSquare() { return mSquare; }
};



#endif //PIECE_H
