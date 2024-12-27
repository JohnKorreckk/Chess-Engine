/**
 * @file PictureObserver.cpp
 * @author John Korreck
 */

#include "pch.h"
#include "PictureObserver.h"
#include "Picture.h"

/**
 * Destructor
 */
PictureObserver::~PictureObserver()
{
 if (mPicture != nullptr)
 {
  mPicture->RemoveObserver(this);
 }
}

/**
 * Set the picture for this observer
 * @param picture The picture to set
 */
void PictureObserver::SetPicture(std::shared_ptr<Picture> picture)
{
 mPicture = picture;
 mPicture->AddObserver(this);
}