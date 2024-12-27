/**
 * @file ImageDrawableTest.cpp
 * @author Charles Owen
 */

#include <pch.h>
#include "gtest/gtest.h"

#include <ImageDrawable.h>

TEST(ImageDrawableTest, Center)
{
    ImageDrawable imageDrawable(L"Shirt", L"images/harold_shirt.png");

    ASSERT_EQ(L"Shirt", imageDrawable.GetName());

    ASSERT_EQ(0, imageDrawable.GetCenter().x);
    ASSERT_EQ(0, imageDrawable.GetCenter().y);

    imageDrawable.SetCenter(wxPoint(234, 569));
    ASSERT_EQ(234, imageDrawable.GetCenter().x);
    ASSERT_EQ(569, imageDrawable.GetCenter().y);
}