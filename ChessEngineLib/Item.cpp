/**
 * @file Item.cpp
 * @author John Korreck
 */

#include "pch.h"
#include "Item.h"
#include "Drawable.h"
#include "Square.h"

/**
 * Constructor
 * @param name The actor name
 */
Item::Item(const std::wstring& name) : mName(name)
{

}



/**
 * Set the root drawable for the actor
 * @param root Pointer to root drawable
 */
void Item::SetRoot(std::shared_ptr<Drawable> root)
{
 mRoot = root;
}

/**
 * Draw this actor
 * @param graphics The Graphics object we are drawing on
 */
void Item::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
 // Don't draw if not enabled
 if (!mEnabled)
  return;

 // This takes care of determining the absolute placement
 // of all of the child drawables. We have to determine this
 // in tree order, which may not be the order we draw.
 if (mRoot != nullptr)
  mRoot->Place(mPosition, 0);

 for (auto drawable : mDrawablesInOrder)
 {
  drawable->Draw(graphics);
 }
}


/**
* Test to see if a mouse click is on this actor.
* @param pos Mouse position on drawing
* @return A drawable object we clicked on or nullptr if we missed.
*/
std::shared_ptr<Drawable> Item::HitTest(wxPoint pos)
{
 // If not enabled or not clickable, we indicate no hit.
 if (!mClickable || !mEnabled)
  return nullptr;

 // Since this list is in drawing order, we really want to know the last thing drawn
 // under the mouse, since it will be on top. So, we reverse iterate over the list.
 for (auto d = mDrawablesInOrder.rbegin(); d != mDrawablesInOrder.rend(); d++)
 {
  auto drawable = *d;
  if (drawable->HitTest(pos))
   // I want to move this drawable to the end of the "mDrawablesInOrder" vector
   return drawable;
 }

 return nullptr;
}


/**
* Add a drawable to this actor
* @param drawable The drawable to add
*/
void Item::AddDrawable(std::shared_ptr<Drawable> drawable)
{
 mDrawablesInOrder.push_back(drawable);
 drawable->SetActor(this);
}

void Item::SetPicture(Picture* picture)
{
 mPicture = picture;
}

void MoveToBack(std::shared_ptr<Item> item);

void Item::MoveToBack(std::shared_ptr<Drawable> drawable)
{
 auto loc = std::find(mDrawablesInOrder.begin(), mDrawablesInOrder.end(), drawable);
 if (loc != mDrawablesInOrder.end())
 {
  mDrawablesInOrder.erase(loc);
 }
 mDrawablesInOrder.push_back(drawable);
}