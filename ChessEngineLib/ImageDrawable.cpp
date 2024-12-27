/**
 * @file ImageDrawable.cpp
 * @author John Korreck
 */

#include "pch.h"
#include "ImageDrawable.h"

/**
 * Constructor
 * @param name The drawable name
 * @param filename The filename for the image
 */
ImageDrawable::ImageDrawable(const std::wstring &name, const std::wstring &filename) :
        Drawable(name)
{
 mImage = std::make_unique<wxImage>(filename, wxBITMAP_TYPE_ANY);
}

/**
 * Draw the image drawable
 * @param graphics Graphics context to draw on
 */
void ImageDrawable::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
 if(mBitmap.IsNull())
 {
  mBitmap = graphics->CreateBitmapFromImage(*mImage);
 }
 graphics->PushState();

 graphics->DrawBitmap(mBitmap, GetPosition().x, GetPosition().y,
         mImage->GetWidth(), mImage->GetHeight());

 graphics->PopState();
}


/**
 * Test to see if we clicked on the image.
 * @param pos Position to test
 * @return True if clicked on
 */
bool ImageDrawable::HitTest(wxPoint pos)
{
 double x = pos.x;
 double y = pos.y;

 // Translate(-mPlacedPosition)
 x -= GetPosition().x;
 y -= GetPosition().y;


 // double sn = sin(mPlacedR);
 // double cs = cos(mPlacedR);
 //
 // // Rotate(mPlacedR)
 // double x1 = cs * x - sn * y;
 // double y1 = sn * x + cs * y;
 //
 // // Translate(mCenter)
 // x = x1 + mCenter.x;
 // y = y1 + mCenter.y;

 double wid = mImage->GetWidth();
 double hit = mImage->GetHeight();

 // Test to see if x, y are in the image
 if (x < 0 || y < 0 || x >= wid || y >= hit)
 {
  // We are outside the image
  return false;
 }

 // Test to see if x, y are in the drawn part of the image
 // If the location is transparent, we are not in the drawn
 // part of the image
 return !mImage->IsTransparent((int)x, (int)y);
}


