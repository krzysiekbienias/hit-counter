#include <gtest/gtest.h>
#include <utility>
#include "interface.hpp"


TEST(HitCounterHitTest, SingleHitCreatesOneBucket) {
    HitCounter counter;

    counter.hit(10);

    EXPECT_EQ(counter.getTotalHits(), 1);
    EXPECT_EQ(counter.getBucketCount(), 1u);
    EXPECT_EQ(counter.getLastBucket().first, 10);
    EXPECT_EQ(counter.getLastBucket().second, 1);
}

TEST(HitCounterHitTest, TwoHitsAtSameTimestampMergeIntoOneBucket) {
    HitCounter counter;

    counter.hit(10);
    counter.hit(10);

    EXPECT_EQ(counter.getTotalHits(), 2);
    EXPECT_EQ(counter.getBucketCount(), 1u);
    EXPECT_EQ(counter.getLastBucket().first, 10);
    EXPECT_EQ(counter.getLastBucket().second, 2);
}

TEST(HitCounterHitTest, ThreeHitsAtSameTimestampStillUseOneBucket) {
    HitCounter counter;

    counter.hit(7);
    counter.hit(7);
    counter.hit(7);

    EXPECT_EQ(counter.getTotalHits(), 3);
    EXPECT_EQ(counter.getBucketCount(), 1u);
    EXPECT_EQ(counter.getLastBucket().first, 7);
    EXPECT_EQ(counter.getLastBucket().second, 3);
}

TEST(HitCounterHitTest, NewTimestampCreatesNewBucket) {
    HitCounter counter;

    counter.hit(10);
    counter.hit(11);

    EXPECT_EQ(counter.getTotalHits(), 2);
    EXPECT_EQ(counter.getBucketCount(), 2u);
    EXPECT_EQ(counter.getLastBucket().first, 11);
    EXPECT_EQ(counter.getLastBucket().second, 1);
}

TEST(HitCounterHitTest, SameTimestampThenNewTimestampUpdatesStateCorrectly) {
    HitCounter counter;

    counter.hit(10);
    counter.hit(10);
    counter.hit(11);

    EXPECT_EQ(counter.getTotalHits(), 3);
    EXPECT_EQ(counter.getBucketCount(), 2u);
    EXPECT_EQ(counter.getLastBucket().first, 11);
    EXPECT_EQ(counter.getLastBucket().second, 1);
}

TEST(HitCounterHitTest, NewTimestampThenRepeatedTimestampMergesOnlyAtBack) {
    HitCounter counter;

    counter.hit(10);
    counter.hit(11);
    counter.hit(11);

    EXPECT_EQ(counter.getTotalHits(), 3);
    EXPECT_EQ(counter.getBucketCount(), 2u);
    EXPECT_EQ(counter.getLastBucket().first, 11);
    EXPECT_EQ(counter.getLastBucket().second, 2);
}

TEST(HitCounterHitTest, MultipleDistinctTimestampsCreateMultipleBuckets) {
    HitCounter counter;

    counter.hit(1);
    counter.hit(2);
    counter.hit(3);
    counter.hit(4);

    EXPECT_EQ(counter.getTotalHits(), 4);
    EXPECT_EQ(counter.getBucketCount(), 4u);
    EXPECT_EQ(counter.getLastBucket().first, 4);
    EXPECT_EQ(counter.getLastBucket().second, 1);
}

TEST(HitCounterHitTest, MixedPatternMaintainsCorrectTotalAndBackBucket) {
    HitCounter counter;

    counter.hit(5);
    counter.hit(5);
    counter.hit(6);
    counter.hit(6);
    counter.hit(6);
    counter.hit(9);

    EXPECT_EQ(counter.getTotalHits(), 6);
    EXPECT_EQ(counter.getBucketCount(), 3u);
    EXPECT_EQ(counter.getLastBucket().first, 9);
    EXPECT_EQ(counter.getLastBucket().second, 1);
}

TEST(HitCounterHitTest, LargeBurstAtSameTimestampAccumulatesInOneBucket) {
    HitCounter counter;

    for (int i = 0; i < 100; ++i) {
        counter.hit(42);
    }

    EXPECT_EQ(counter.getTotalHits(), 100);
    EXPECT_EQ(counter.getBucketCount(), 1u);
    EXPECT_EQ(counter.getLastBucket().first, 42);
    EXPECT_EQ(counter.getLastBucket().second, 100);
}

TEST(HitCounterHitTest, LargeBurstThenNewTimestampCreatesSecondBucket) {
    HitCounter counter;

    for (int i = 0; i < 50; ++i) {
        counter.hit(100);
    }
    counter.hit(101);

    EXPECT_EQ(counter.getTotalHits(), 51);
    EXPECT_EQ(counter.getBucketCount(), 2u);
    EXPECT_EQ(counter.getLastBucket().first, 101);
    EXPECT_EQ(counter.getLastBucket().second, 1);
}