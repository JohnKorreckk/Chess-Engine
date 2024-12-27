/**
 * @file TimelineTest.cpp
 * @author Charles Owen
 */

#include <pch.h>
#include "gtest/gtest.h"

#include <Timeline.h>
#include <AnimChannelAngle.h>


TEST(TimelineTest, NumFrames)
{
    Timeline timeline;
    
    // Default number of frames
    ASSERT_EQ(300, timeline.GetNumFrames());
    
    // Set and get
    timeline.SetNumFrames(123);
    ASSERT_EQ(123, timeline.GetNumFrames());
}

TEST(TimelineTest, FrameRate)
{
    Timeline timeline;

    // Default number of frames
    ASSERT_EQ(30, timeline.GetFrameRate());

    // Set and get
    timeline.SetFrameRate(22);
    ASSERT_EQ(22, timeline.GetFrameRate());
}


TEST(TimelineTest, CurrentTime)
{
    Timeline timeline;

    // Default number of frames
    ASSERT_NEAR(0.0, timeline.GetCurrentTime(), 0.00001);

    // Set and get
    timeline.SetCurrentTime(1.23);
    ASSERT_NEAR(1.23, timeline.GetCurrentTime(), 0.00001);
}



TEST(TimelineTest, GetDuration)
{
    Timeline timeline;

    // Default value
    ASSERT_NEAR(10, timeline.GetDuration(), 0.0001);

    // Changed duration
    timeline.SetFrameRate(375);
    ASSERT_NEAR(300.0 / 375.0, timeline.GetDuration(), 0.0001);

    timeline.SetNumFrames(789);
    ASSERT_NEAR(789.0 / 375.0, timeline.GetDuration(), 0.0001);
}

TEST(TimelineTest, GetCurrentFrame)
{
    Timeline timeline;

    // Default value
    ASSERT_EQ(0, timeline.GetCurrentFrame());

    // Changed time
    timeline.SetCurrentTime(9.27);
    ASSERT_EQ(278, timeline.GetCurrentFrame());
}

TEST(TimelineTest, Add)
{
    Timeline timeline;
    AnimChannelAngle channel;

    timeline.AddChannel(&channel);
    ASSERT_EQ(&timeline, channel.GetTimeline());
}