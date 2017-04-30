#include "stdafx.h"
#include "Tree.h"

void Tree::add_segment(const TSegment& s) {
    segments_.push_back(s);
}

const std::vector<Tree::TSegment>& Tree::segments() const {
    return segments_;
}

// возвращает точки возможного роста
std::vector<Vec2> Tree::grow_points() const {
    // пока возвращаем тупо концы все веток
    std::vector<Vec2> grow_points;
    grow_points.reserve(segments_.size());
    for (auto& s : segments_)
        grow_points.push_back(s.second);
    return grow_points;
}
