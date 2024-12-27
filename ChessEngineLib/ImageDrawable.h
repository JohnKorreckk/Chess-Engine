/**
 * @file ImageDrawable.h
 * @author John Korreck
 * @brief Header file for the ImageDrawable class, which represents a drawable image.
 */

#ifndef IMAGEDRAWABLE_H
#define IMAGEDRAWABLE_H

#include "Drawable.h"

/**
 * @class ImageDrawable
 * @brief A class for drawing and manipulating images.
 */
class ImageDrawable : public Drawable {
private:
    /** The center point of the image */
    wxPoint mCenter = wxPoint(0, 0);
    /** The graphics bitmap used for the image */
    wxGraphicsBitmap mBitmap;
protected:
    /** The wxImage object that holds the image data */
    std::unique_ptr<wxImage> mImage;

public:
    /**
     * @brief Constructor that initializes the ImageDrawable with a name.
     * @param name the name of the image
     */
    ImageDrawable(const std::wstring& name);

    /**
     * @brief Constructor that initializes the ImageDrawable with a name and a filename.
     */
    ImageDrawable(const std::wstring& name, const std::wstring& filename);

    /**
     * @brief Draws the image on the provided graphics context.
     */
    void Draw(std::shared_ptr<wxGraphicsContext> graphics);

    /**
     * @brief Performs a hit test to check if a point is inside the image.
     * @return true if the point is inside the image, false otherwise.
     */
    bool HitTest(wxPoint pos);

    /** Destructor */
    virtual ~ImageDrawable() {}

    /** Default constructor (disabled) */
    ImageDrawable() = delete;

    /** Copy constructor (disabled) */
    ImageDrawable(const Drawable &) = delete;

    /** Assignment operator */
    void operator=(const ImageDrawable &) = delete;

    /**
     * @brief Gets the center point of the image.
     * @return The center point of the image.
     */
    wxPoint GetCenter() { return mCenter; }

    /**
     * @brief Sets the center point of the image.
     * @param center The new center point.
     */
    void SetCenter(wxPoint center) { mCenter = center; }
};

#endif //IMAGEDRAWABLE_H

