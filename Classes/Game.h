#pragma once


class Game {
public:
    static Game* instance();
    
    void Start();
    void Exit();
    // скорость игры
    double game_speed() const;
    // коэффициент получения и траты ресурсов
    double resources_coef() const;
    
private:
    Game();
    
    static std::shared_ptr<Game> instance_;
    double game_speed_;
};
