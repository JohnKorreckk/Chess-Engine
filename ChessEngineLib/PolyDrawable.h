/**
* @file PolyDrawable.h
 * @author John Korreck
 * @brief Header file for the PolyDrawable class, which represents a drawable polygon.
 */

#ifndef POLYDRAWABLE_H
#define POLYDRAWABLE_H

#include "Drawable.h"

/**
 * @class PolyDrawable
 * @brief A class for drawing and manipulating polygons.
 */
class PolyDrawable : public Drawable {
private:
 /** The polygon color */
 wxColour mColor = *wxBLACK;
 /** The array of point objects */
 std::vector<wxPoint> mPoints;
 /** The transformed graphics path used to draw this polygon */
 wxGraphicsPath mPath;

public:
 /**
  * @brief Constructor that initializes the PolyDrawable with a name.
  */
 PolyDrawable(const std::wstring& name);

 /** Destructor */
 virtual ~PolyDrawable() {}

 /** Default constructor (disabled) */
 PolyDrawable() = delete;

 /** Copy constructor (disabled) */
 PolyDrawable(const Drawable &) = delete;

 /** Assignment operator */
 void operator=(const PolyDrawable &) = delete;

 /**
  * @brief Draws the polygon on the provided graphics context.
  */
 void Draw(std::shared_ptr<wxGraphicsContext> graphics) override;

 /**
  * @brief Performs a hit test to check if a point is inside the polygon.
  * @return true if the point is inside the polygon, false otherwise.
  */
 bool HitTest(wxPoint pos) override;

 /**
  * @brief Adds a point to the polygon.
  * @param point to add
  */
 void AddPoint(const wxPoint& point);

 /**
  * @brief Gets the color of the polygon.
  * @return color
  */
 wxColour GetColor() { return mColor; }

 /**
  * @brief Sets the color of the polygon.
  * @param color to change to
  */
 void SetColor(const wxColour& color) { mColor = color; }
};

#endif //POLYDRAWABLE_H
