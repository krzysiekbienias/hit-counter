#include "interface.hpp"

void HitCounter::hit(int timestamp) {
    if (!m_playlist.empty() && m_playlist.back().first == timestamp) {
        m_playlist.back().second++;
    } else
        m_playlist.push_back({timestamp, 1});
    m_totalHits++;
}
