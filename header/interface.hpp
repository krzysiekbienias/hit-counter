#pragma once
#include <deque>
#include <utility>

class HitCounter {
public:
    HitCounter() {};

    // API
    void hit(int timestamp);
    int getHits(int timestamp);

    // helpers method for unit testes they are public but should not be used
    int getTotalHits() const;
    int getBucketCount() const;
    std::pair<int, int> getLastBucket() const;

private:
    std::deque<std::pair<int, int>> m_playlist;
    int m_totalHits{0};
};