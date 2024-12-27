/**
 * @file PictureObserver.h
 * @author John Korreck
 *
 *
 */
 
#ifndef PICTUREOBSERVER_H
#define PICTUREOBSERVER_H

#include <memory>
class Picture;

/**
 * Observer base class for a picture.
 *
 * This class implements the base class functionality for
 * an observer in the observer pattern.
 */
class PictureObserver {
private:
 /// Picture we are observing
 std::shared_ptr<Picture> mPicture;
public:
 /// Copy Constructor (Disabled)
 PictureObserver(const PictureObserver &) = delete;
 /// Assignment Operator (Disabled)
 void operator=(const PictureObserver &) = delete;
 ~PictureObserver();
 void SetPicture(std::shared_ptr<Picture> picture);
 /// This function is called to update any observers
 virtual void UpdateObserver() = 0;
 /*!
  * Returns the observed picture
  * @return pointer to picture that is being observed
  */
 std::shared_ptr<Picture> GetPicture() { return mPicture; }

protected:
 /// Constructor
 PictureObserver() {}
};



#endif //PICTUREOBSERVER_H
