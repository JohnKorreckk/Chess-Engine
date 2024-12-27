/**
 * @file DrawableTest.cpp
 * @author Charles Owen
 */

#include <pch.h>
#include "gtest/gtest.h"

#include <Drawable.h>

/** Drawable mock class for testing */
class DrawableMock : public Drawable
{
public:
    /** Constructor
     * @param name A name for the drawable */
    DrawableMock(const std::wstring &name) : Drawable(name) {}

    /** Draw dummy function
     * @param graphics Graphics object to draw on */
    virtual void Draw(std::shared_ptr<wxGraphicsContext> graphics) override {}

    /** HitTest dummy function
     * @param pos Position to test
     * @returns true if we hit the drawable */
    virtual bool HitTest(wxPoint pos) override { return false; }

};

TEST(DrawableTest, Name)
{
    DrawableMock drawable(L"Arm");
    ASSERT_EQ(std::wstring(L"Arm"), drawable.GetName());
}

TEST(DrawableTest, Position)
{
    DrawableMock drawable(L"Arm");
    ASSERT_EQ(0, drawable.GetPosition().x);
    ASSERT_EQ(0, drawable.GetPosition().y);

    drawable.SetPosition(wxPoint(123, 456));
    ASSERT_EQ(123, drawable.GetPosition().x);
    ASSERT_EQ(456, drawable.GetPosition().y);
}

TEST(DrawableTest, Rotation)
{
    DrawableMock drawable(L"Arm");
    ASSERT_NEAR(0, drawable.GetRotation(), 0.00001);

    drawable.SetRotation(1.23);
    ASSERT_NEAR(1.23, drawable.GetRotation(), 0.00001);

    double a = 0.333;
    double b = acos(cos(a));
    ASSERT_NEAR(a, b, 0.000000001);
}

TEST(DrawableTest, Association)
{
    DrawableMock body(L"Body");
    auto arm = std::make_shared<DrawableMock>(L"Arm");
    auto leg = std::make_shared<DrawableMock>(L"Leg");

    ASSERT_EQ(nullptr, arm->GetParent());
    ASSERT_EQ(nullptr, leg->GetParent());

    body.AddChild(arm);
    body.AddChild(leg);

    ASSERT_EQ(&body, arm->GetParent());
    ASSERT_EQ(&body, leg->GetParent());
}