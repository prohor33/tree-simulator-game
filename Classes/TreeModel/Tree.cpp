#include "stdafx.h"
#include "Tree.h"

void Tree::add_segment(const TSegment& s) {
    segments_.push_back(s);
}

const std::vector<Tree::TSegment>& Tree::segments() const {
    return segments_;
}
