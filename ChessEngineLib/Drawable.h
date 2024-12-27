/**
* @file Drawable.h
 * @author John Korreck
 */

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <vector>
#include <memory>
#include <string>
#include <wx/graphics.h>
#include <wx/gdicmn.h>

class Item;

/**
 * Abstract base class for drawable elements of a picture.
 */
class Drawable {
private:
    /// Drawable position
    wxPoint mPosition = wxPoint(0, 0);

    /// Rotation angle in radians
    double mRotation = 0;

    /// The actor using this drawable
    Item *mActor = nullptr;

    /// Children drawables
    std::vector<std::shared_ptr<Drawable>> mChildren;

    /// Parent drawable
    Drawable* mParent = nullptr;

protected:
    /// Drawable name
    std::wstring mName;

    /// Placed position
    wxPoint mPlacedPosition = wxPoint(0, 0);

    /// Placed rotation
    double mPlacedR = 0;

   /// Is this drawable clickable?
   // bool mClickable = true;

    /**
     * Constructor
     * @param name The drawable name
     */
    Drawable(const std::wstring &name);

    /**
     * Rotate a point by a given angle
     * @param point The point to rotate
     * @param angle An angle in radians
     * @return Rotated point
     */
    wxPoint RotatePoint(wxPoint point, double angle);

public:
    /// Destructor
    virtual ~Drawable() {}

    /// Disabled default constructor
    Drawable() = delete;

    /// Disabled copy constructor
    Drawable(const Drawable &) = delete;

    /// Disabled assignment operator
    void operator=(const Drawable &) = delete;

    /**
     * Set the actor using this drawable
     * @param actor The actor to set
     */
    void SetActor(Item* actor);

    /**
     * Draw the drawable
     * @param graphics The graphics context
     */
    virtual void Draw(std::shared_ptr<wxGraphicsContext> graphics) = 0;

    /**
     * Place this drawable relative to its parent
     * @param offset Parent offset
     * @param rotate Parent rotation
     */
    void Place(wxPoint offset, double rotate);

    /**
     * Add a child drawable to this drawable
     * @param child The child to add
     */
    void AddChild(std::shared_ptr<Drawable> child);

    /**
     * Move this drawable by a specified amount
     * @param delta The amount to move in pixels
     */
    void Move(wxPoint delta);

    /**
     * Test to see if we have been clicked on by the mouse
     * @param pos Position to test
     * @return true if clicked on, false otherwise
     */
    virtual bool HitTest(wxPoint pos) = 0;

    /**
     * Determine if this drawable is movable
     * @return true if movable, false otherwise
     */
    virtual bool IsMovable() { return true; }

    /**
     * Set the drawable position
     * @param pos The new drawable position
     */
    void SetPosition(wxPoint pos) { mPosition = pos; }

    /**
     * Get the drawable position
     * @return The drawable position
     */
    wxPoint GetPosition() const { return mPosition; }

    /**
     * Set the rotation angle in radians
     * @param r The new rotation angle in radians
     */
    void SetRotation(double r) { mRotation = r; }

    /**
     * Get the rotation angle in radians
     * @return The rotation angle in radians
     */
    double GetRotation() const { return mRotation; }

    /**
     * Get the drawable name
     * @return The drawable name
     */
    std::wstring GetName() const { return mName; }

    /**
     * Get the parent drawable
     * @return Pointer to the parent drawable
     */
    Drawable* GetParent() { return mParent; }

    /**
     * Set the parent drawable
     * @param parent Pointer to the parent drawable
     */
    void SetParent(Drawable* parent) { mParent = parent; }
    /**
     *
     *
     */
    // void SetClickable(bool clickable) { mClickable = clickable; }
};

#endif //DRAWABLE_H
