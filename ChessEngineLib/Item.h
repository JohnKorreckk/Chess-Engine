/**
 * @file Item.h
 * @author John Korreck
 *
 *
 */
 
#ifndef ACTOR_H
#define ACTOR_H
#include "Picture.h"


/**
 * Class for actors in our drawings.
 *
 * An actor is some graphical object that consists of
 * one or more parts. Actors can be animated.
 */
class Drawable;
class Square;
/**
 * Class for actors in our drawings.
 *
 * An actor is some graphical object that consists of
 * one or more parts. Actors can be animated.
 */
class Item {
private:
 /// The actor name
 std::wstring mName;

 /// Is this actor enabled (drawable)?
 bool mEnabled = true;

 /// The actor position
 wxPoint mPosition;

 /// Is this actor mouse clickable?
 bool mClickable = true;

 /// The root drawable
 std::shared_ptr<Drawable> mRoot;

 /// The drawables in drawing order
 std::vector<std::shared_ptr<Drawable>> mDrawablesInOrder;

 /// The picture using this actor
 Picture *mPicture = nullptr;

public:
 /// Destructor
 virtual ~Item() {}

 Item(const std::wstring &name);


 /// Default constructor (disabled)
 Item() = delete;

 /// Copy constructor (disabled)
 Item(const Item &) = delete;

 /// Assignment operator
 void operator=(const Item &) = delete;

 void SetRoot(std::shared_ptr<Drawable> root);
 void Draw(std::shared_ptr<wxGraphicsContext> graphics);
 std::shared_ptr<Drawable> HitTest(wxPoint pos);
 void AddDrawable(std::shared_ptr<Drawable> drawable);

 /**
  * Get the actor name
  * @return Actor name
  */
 std::wstring GetName() const { return mName; }

 /**
  * The actor position
  * @return The actor position as a point
  */
 wxPoint GetItemPosition() const { return mPosition; }

 /**
  * The actor position
  * @param pos The new actor position
  */
 void SetItemPosition(wxPoint pos) { /** std::cout << mPosition.x << ", " << mPosition.y << std::endl **/; mPosition = pos; }


 /**
  * Item is enabled
  * @return enabled status
  */
 bool IsEnabled() const { return mEnabled; }

 /**
  * Set Item Enabled
  * @param enabled New enabled status
  */
 void SetEnabled(bool enabled) { mEnabled = enabled; }

 /**
  * Item is clickable
  * @return true if actor is clickable
  */
 bool IsClickable() const { return mClickable; }

 /**
  * Item clickable
  * @param clickable New clickable status
  */
 void SetClickable(bool clickable) { mClickable = clickable; }
 /**
  * Item clickable
  * @param picture to set to
  */
 void SetPicture(Picture* picture);
 void MoveToBack(std::shared_ptr<Drawable> drawable);

 /**
  * Item clickable
  * @return the picture held
  */
 Picture* GetPicture() { return mPicture; }

 /**
  * returns drawable list
  * @return Drawable list
  */
 std::vector<std::shared_ptr<Drawable>> GetDrawablesInOrder() { return mDrawablesInOrder; }

 virtual std::shared_ptr<Square> GetClosestSquare(wxPoint point) { return nullptr; }
};



#endif //ACTOR_H
