/**
 * @file PictureFactory.cpp
 *
 * @author John Korreck
 */

#include "pch.h"
#include "PictureFactory.h"
#include "BoardFactory.h"
#include "Picture.h"
#include "Item.h"
#include "ImageDrawable.h"
#include "PolyDrawable.h"
#include "Board.h"


/// Directory within resources that contains the images.
const std::wstring ImagesDirectory = L"/images";


/**
 * FItemy method to create a new picture.
 * @param resourcesDir Directory that contains the resources for this application
 * @return The created picture
 */
std::shared_ptr<Picture> PictureFactory::Create(std::wstring resourcesDir)
{
    auto imagesDir = resourcesDir + ImagesDirectory;

    auto picture = std::make_shared<Picture>();

    std::wstring name = L"Board";

    BoardFactory boardFactory;
    std::shared_ptr<Board> board = boardFactory.Create(resourcesDir);

    picture->SetBoard(board);

    return picture;
}