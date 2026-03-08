#include <interface.hpp>

int HitCounter::getTotalHits() const {
    return m_totalHits;
}

std::pair<int, int> HitCounter::getLastBucket() const {
    return m_playlist.back();
}

int HitCounter::getBucketCount() const {
    return m_playlist.size();
}
