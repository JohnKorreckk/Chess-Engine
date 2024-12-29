/**
 * @file Square.h
 * @author John Korreck
 *
 *
 */
 
#ifndef SQUARE_H
#define SQUARE_H

#include "PolyDrawable.h"
class Piece;

class Square : public PolyDrawable {
private:
 /// The piece assigned to this square
 std::shared_ptr<Drawable> mPiece = nullptr;

 /// The center of the square
 wxPoint mCenter = wxPoint(0, 0);
public:
 /**
  * @brief Constructor for the Square class with name and filename.
  * @param name The name of the Square.
  * @param filename The file path for the image.
  */
 Square(const std::wstring& name);

 Square() = delete;

 /** Copy constructor (disabled) */
 Square(const Drawable &) = delete;

 /** Assignment operator */
 void operator=(const Square &) = delete;

 wxPoint GetCenter() { return mCenter; };

 void SetCenter(wxPoint point) { mCenter = point; }

 void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

 void SetPiece(std::shared_ptr<Drawable> piece) { mPiece = piece; }

 std::shared_ptr<Drawable> GetPiece() { return mPiece; }
};



#endif //SQUARE_H
