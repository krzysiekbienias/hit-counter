#include <gtest/gtest.h>
#include "interface.hpp"

TEST(HitCounterGetHitsTest, EmptyCounterReturnsZero) {
    HitCounter counter;

    EXPECT_EQ(counter.getHits(1), 0);
}

TEST(HitCounterGetHitsTest, SingleHitIsReturnedAtSameTimestamp) {
    HitCounter counter;
    counter.hit(1);

    EXPECT_EQ(counter.getHits(1), 1);
}

TEST(HitCounterGetHitsTest, MultipleHitsAtSameTimestampAreCounted) {
    HitCounter counter;
    counter.hit(10);
    counter.hit(10);
    counter.hit(10);

    EXPECT_EQ(counter.getHits(10), 3);
}

TEST(HitCounterGetHitsTest, HitsAcrossDifferentTimestampsAreSummedWithinWindow) {
    HitCounter counter;
    counter.hit(1);
    counter.hit(2);
    counter.hit(3);

    EXPECT_EQ(counter.getHits(3), 3);
}

TEST(HitCounterGetHitsTest, HitAtTimestampOneIsStillIncludedAtThreeHundred) {
    HitCounter counter;
    counter.hit(1);

    EXPECT_EQ(counter.getHits(300), 1);
}

TEST(HitCounterGetHitsTest, HitAtTimestampOneExpiresAtThreeHundredOne) {
    HitCounter counter;
    counter.hit(1);

    EXPECT_EQ(counter.getHits(301), 0);
}

TEST(HitCounterGetHitsTest, KeepsHitsInsideClosedWindowFromTimestampMinus299ToTimestamp) {
    HitCounter counter;
    counter.hit(1);
    counter.hit(2);
    counter.hit(300);

    EXPECT_EQ(counter.getHits(300), 3);
}

TEST(HitCounterGetHitsTest, RemovesExpiredHitsWhenWindowMovesForward) {
    HitCounter counter;
    counter.hit(1);
    counter.hit(2);
    counter.hit(300);

    EXPECT_EQ(counter.getHits(301), 2); // timestamp 1 expired
}

TEST(HitCounterGetHitsTest, RemovesMultipleExpiredBucketsAtOnce) {
    HitCounter counter;
    counter.hit(1);
    counter.hit(2);
    counter.hit(3);
    counter.hit(400);

    EXPECT_EQ(counter.getHits(400), 1);
}

TEST(HitCounterGetHitsTest, ExactLeftBoundaryIsExcluded) {
    HitCounter counter;
    counter.hit(20);
    counter.hit(21);
    counter.hit(320);

    EXPECT_EQ(counter.getHits(320), 2); // valid range [21, 320]
}

TEST(HitCounterGetHitsTest, RepeatedQueriesWithoutNewExpiryReturnSameAnswer) {
    HitCounter counter;
    counter.hit(100);
    counter.hit(150);
    counter.hit(200);

    EXPECT_EQ(counter.getHits(200), 3);
    EXPECT_EQ(counter.getHits(200), 3);
    EXPECT_EQ(counter.getHits(201), 3);
}

TEST(HitCounterGetHitsTest, QueryFarInFutureReturnsZeroWhenEverythingExpired) {
    HitCounter counter;
    counter.hit(1);
    counter.hit(2);
    counter.hit(3);

    EXPECT_EQ(counter.getHits(1000), 0);
}

TEST(HitCounterGetHitsTest, LargeBurstAtSameTimestampExpiresTogether) {
    HitCounter counter;
    for (int i = 0; i < 100; ++i) {
        counter.hit(5);
    }

    EXPECT_EQ(counter.getHits(304), 100);
    EXPECT_EQ(counter.getHits(305), 0);
}

TEST(HitCounterGetHitsTest, InterleavedHitsAndQueriesWorkCorrectly) {
    HitCounter counter;

    counter.hit(1);
    counter.hit(1);
    EXPECT_EQ(counter.getHits(1), 2);

    counter.hit(2);
    EXPECT_EQ(counter.getHits(2), 3);

    counter.hit(301);
    EXPECT_EQ(counter.getHits(301), 2); // two hits at timestamp 1 expired
}

TEST(HitCounterGetHitsTest, MixedBucketsReturnCorrectCountAfterCleanup) {
    HitCounter counter;
    counter.hit(10);
    counter.hit(10);
    counter.hit(11);
    counter.hit(100);
    counter.hit(310);

    EXPECT_EQ(counter.getHits(310), 3); // only 100 and 310 remain
}

TEST(HitCounterGetHitsTest, AllHitsWithinWindowRemainCounted) {
    HitCounter counter;
    counter.hit(50);
    counter.hit(100);
    counter.hit(150);
    counter.hit(200);
    counter.hit(250);
    counter.hit(300);

    EXPECT_EQ(counter.getHits(300), 6);
}

TEST(HitCounterGetHitsTest, CleanupDoesNotRemoveValidFrontBucket) {
    HitCounter counter;
    counter.hit(21);
    counter.hit(22);
    counter.hit(320);

    EXPECT_EQ(counter.getHits(320), 3); // 21 is still valid
}