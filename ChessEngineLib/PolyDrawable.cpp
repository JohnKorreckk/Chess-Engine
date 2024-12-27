/**
 * @file PolyDrawable.cpp
 * @author John Korreck
 */

#include "pch.h"
#include "PolyDrawable.h"

/**
 * Constructor
 * @param name The drawable name
 */
PolyDrawable::PolyDrawable(const std::wstring &name) : Drawable(name)
{
}

/**
 * Draw our polygon.
 * @param  graphics The graphics context to draw on
 */
void PolyDrawable::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
 if(!mPoints.empty()) {
  mPath = graphics->CreatePath();
  mPath.MoveToPoint(RotatePoint(mPoints[0], mPlacedR));
  for (auto i = 1; i<mPoints.size(); i++)
  {
   mPath.AddLineToPoint(RotatePoint(mPoints[i], mPlacedR) + mPlacedPosition);
  }
  mPath.CloseSubpath();

  wxBrush brush(mColor);
  graphics->SetBrush(brush);
  graphics->FillPath(mPath);
 }
}

void PolyDrawable::AddPoint(const wxPoint& point)
{
 mPoints.push_back(point);
}

/**
 * Test to see if we hit this object with a mouse click
 * @param pos Click position
 * @return true it hit
 */
bool PolyDrawable::HitTest(wxPoint pos)
{
 return false;
  // return mPath.Contains(pos.x, pos.y);
}
