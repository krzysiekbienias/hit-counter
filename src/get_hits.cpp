#include <interface.hpp>


int HitCounter::getHits(int timestamp) {
    while (!m_playlist.empty() && m_playlist.front().first<=timestamp-300) {
        m_totalHits-=m_playlist.front().second;
        m_playlist.pop_front();
    }
    return m_totalHits;
}
