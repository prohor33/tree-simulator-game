#pragma once

USING_NS_CC;

class Tree {
public:
    
    // ветка
    using TSegment = std::pair<Vec2, Vec2>;
    
    void add_segment(const TSegment& s);
    const std::vector<TSegment>& segments() const;
    
    // возвращает точки возможного роста
    std::vector<Vec2> grow_points() const;
    
private:
    // просто для примера
    std::vector<TSegment> segments_;
};
