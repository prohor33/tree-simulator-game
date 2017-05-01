#pragma once

USING_NS_CC;

class Tree {
public:
    
    // ветка
    using TSegment = std::pair<Vec2, Vec2>;
    
    const std::vector<TSegment>& segments() const;
    void add_segment(const TSegment& s);
    
    // возвращает точки возможного роста
    std::vector<Vec2> grow_points() const;
    
    // листья
    const std::vector<Vec2>& leafs() const;
    void add_leaf(const Vec2& leaf);
    
private:
    // просто для примера
    // ветки
    std::vector<TSegment> segments_;
    // листья
    std::vector<Vec2> leafs_;
};
