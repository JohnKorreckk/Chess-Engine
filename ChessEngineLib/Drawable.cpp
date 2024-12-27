/**
* @file Drawable.cpp
 * @author John Korreck
 */

#include "pch.h"
#include "Drawable.h"
#include "Item.h"
#include <cmath>

/**
 * Constructor
 * @param name The drawable name
 */
Drawable::Drawable(const std::wstring &name) : mName(name) {}

/**
 * Set the actor using this drawable
 */
void Drawable::SetActor(Item *actor) {
 mActor = actor;
}

/**
 * Place this drawable relative to its parent
 */
void Drawable::Place(wxPoint offset, double rotate) {
 mPosition = offset + mPosition;
 mPlacedR = mRotation + rotate;

 for (auto drawable : mChildren) {
  drawable->Place(mPosition, mPlacedR);
 }
}

/**
 * Add a child drawable
 */
void Drawable::AddChild(std::shared_ptr<Drawable> child) {
 mChildren.push_back(child);
 child->SetParent(this);
}

/**
 * Move this drawable
 */
void Drawable::Move(wxPoint delta) {

 if (mParent != nullptr) {
  mPosition = mPosition + RotatePoint(delta, -mParent->mPlacedR);
 } else {
  mPosition = mPosition + -delta;
 }
}

/**
 * Rotate a point by an angle
 * @return Rotated point
 */
wxPoint Drawable::RotatePoint(wxPoint point, double angle) {
 double cosA = cos(angle);
 double sinA = sin(angle);
 return wxPoint(int(cosA * point.x + sinA * point.y), int(-sinA * point.x + cosA * point.y));
}