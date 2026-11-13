/*
 * Copyright (C) 2026 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: BSL-1.0
 * See LICENSE file for details.
 */
#include "MonsterUtils.hpp"

namespace D2ModGen {

void MonResist::readFromRow(const DataRow_monstats& row, size_t i)
{
    m_resist[Element::Phys] = row.resDm[i].value_or(0);
    m_resist[Element::Mag]  = row.resMa[i].value_or(0);
    m_resist[Element::Fire] = row.resFi[i].value_or(0);
    m_resist[Element::Ltng] = row.resLi[i].value_or(0);
    m_resist[Element::Cold] = row.resCo[i].value_or(0);
    m_resist[Element::Pois] = row.resPo[i].value_or(0);
}

void MonResist::writeToRow(DataRow_monstats& row, size_t i) const
{
    row.resDm[i] = m_resist.at(Element::Phys);
    row.resMa[i] = m_resist.at(Element::Mag);
    row.resFi[i] = m_resist.at(Element::Fire);
    row.resLi[i] = m_resist.at(Element::Ltng);
    row.resCo[i] = m_resist.at(Element::Cold);
    row.resPo[i] = m_resist.at(Element::Pois);
}

void MonResist::shuffle(RandomGenerator& rng)
{
    std::map<Element, Element> permutation;
    const std::vector<Element> commonElems{ Element::Fire, Element::Ltng, Element::Cold, Element::Pois };
    std::set<Element>          remaining{ Element::Phys, Element::Mag, Element::Fire, Element::Ltng, Element::Cold, Element::Pois };
    // first, Physical and Magic have 20% chance to move to elemental, 20% to swap or 60% to stay the same.
    if (rng(5) == 0) {
        permutation[Element::Phys] = Element::Mag;
        permutation[Element::Mag]  = Element::Phys;
        remaining.erase(Element::Phys);
        remaining.erase(Element::Mag);
    } else {
        auto remainingCopy = remaining;
        remainingCopy.erase(Element::Phys);
        remainingCopy.erase(Element::Mag);
        for (Element elem : { Element::Phys, Element::Mag }) {
            if (rng(5) == 0) {
                auto it = remainingCopy.begin();
                if (remainingCopy.size() > 1)
                    std::advance(it, rng(remainingCopy.size()));
                Element dest      = *it;
                permutation[elem] = dest;
                remainingCopy.erase(dest);
                remaining.erase(dest);
            } else {
                permutation[elem] = elem;
                remaining.erase(elem);
            }
        }
    }
    // second, common element permute with the rest
    for (Element elem : commonElems) {
        auto it = remaining.begin();
        if (remaining.size() > 1)
            std::advance(it, rng(remaining.size()));
        Element dest      = *it;
        permutation[elem] = dest;
        remaining.erase(dest);
    }
    assert(remaining.size() == 0);
    assert(permutation.size() == 6);
    assert(permutation.count(Element::Phys));
    assert(permutation.count(Element::Mag));
    assert(permutation.count(Element::Fire));
    assert(permutation.count(Element::Ltng));
    assert(permutation.count(Element::Cold));
    assert(permutation.count(Element::Pois));
    std::map<Element, int> newResist;
    for (const auto& [src, dest] : permutation)
        newResist[dest] = m_resist[src];

    m_resist = newResist;
}
}
