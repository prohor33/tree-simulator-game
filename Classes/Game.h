#pragma once


class Game {
public:
    static Game* instance();
    
    void Start();
    void Exit();
    
private:
    Game();
    
    static std::shared_ptr<Game> instance_;
};
