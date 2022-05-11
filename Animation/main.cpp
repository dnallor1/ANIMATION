#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class AnimatedSprite : public sf::Sprite {
public:
    AnimatedSprite(const int fps, const std::string& path) : fps_(fps)//make an initializer list for fps_
    {
        sf::Texture texture;
        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
        setTextureRect(sf::IntRect(50, 0, 50, 37));
    }
    void setSpeed(const int& x_speed, const int& y_speed, const int & ro_speed) {
        x_speed_ = x_speed;
        y_speed_ = y_speed;
        ro_speed_ = ro_speed;
    }
    void move_down(const sf::Time &elapsed) {
        bouncce();
        float dt = elapsed.asSeconds();
        int g = 10;
        t_ += dt;
        move(0,0.5 * g * pow(t_,2));
    }
    void move_up(const sf::Time &elapsed) {
        bouncce();
        float dt = elapsed.asSeconds();
        int g = -10;
        t_ += dt;
        move(0,0.5 * g * pow(t_,2));
    }
    void jump(const sf::Time &elapsed) {
        bouncce();
        float dt = elapsed.asSeconds();
        int g = -20;
        move(0,0.5 * g * pow(dt,2));
    }
    void move_right(const sf::Time &elapsed){
        bouncce();
        float dt = elapsed.asSeconds();
        t_ += dt;
        if(t_ > 1.0/fps_){
            fragments_index++;
            t_ = 0.0;
        }
        if(fragments_index == running_frames.size()){
            fragments_index = 0;
        }
        setTextureRect(running_frames[fragments_index]);
        move(x_speed_*dt, 0);
    }
    void move_leftt(const sf::Time &elapsed){
        bouncce();
        float dt = elapsed.asSeconds();
        t_ += dt;
        if(t_ > 1.0/fps_){
            fragments_index++;
            t_ = 0.0;
        }
        if(fragments_index == running_frames.size()){
            fragments_index = 0;
        }
        setTextureRect(running_frames[fragments_index]);
        move(-x_speed_*dt, 0);
    }
    void setBounds(const float& l_bound, const float& r_bound,const float& u_bound,const float& d_bound){
        l_bound_  = l_bound  ;
        r_bound_  = r_bound  ;
        u_bound_  = u_bound  ;
        d_bound_  = d_bound  ;
    }
    void add_animation_frame(const sf::IntRect& frame){
        running_frames.emplace_back(frame);
    }
private:
    sf::Texture texture_;
    int fps_;
    int x_speed_ = 0 ;
    int y_speed_ = 0 ;
    int ro_speed_ = 0 ;
    float l_bound_ = 0;
    float r_bound_ = 0;
    float u_bound_ = 0;
    float d_bound_ = 0;
    float t_ = 0.0;
    unsigned int fragments_index = 0;
    std::vector<sf::IntRect> running_frames;

    void bouncce(){
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        if(rectangle_bounds.top <= u_bound_){
            y_speed_ = abs(y_speed_);
        }
        if(rectangle_bounds.top + rectangle_bounds.height >= d_bound_){
            y_speed_ = abs(y_speed_) * -1;
        }
        if(rectangle_bounds.left <= l_bound_ ){
            x_speed_ = abs(x_speed_);
        }
        if(rectangle_bounds.left + rectangle_bounds.width >= r_bound_){
            x_speed_ = abs(x_speed_) * -1;
        }
    }
};
int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    const float gridSize = 50.f;
    std::vector<sf::Sprite> walls;
    sf::Vector2f vel_;
    sf::Vector2i mos_Pos_;
    AnimatedSprite hero(10, "Character\\character.png");
    hero.setBounds(0, window.getSize().x, 0, window.getSize().y);
    hero.setSpeed(70,70,10);

    hero.add_animation_frame(sf::IntRect(150, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(200, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(250, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(300, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(350, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(400, 0, 50, 37)); // hero running frame 1

    sf::Clock clock;
    // run the program as long as the window is open
    while (window.isOpen()) {
        hero.setTextureRect(sf::IntRect(50, 0, 50, 37));
        hero.setScale(1.5,1.5);

        sf::Time elapsed = clock.restart();
        mos_Pos_.x = sf::Mouse::getPosition(window).x / (int)gridSize;
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))  {
            hero.move_up(elapsed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  {
            hero.move_down(elapsed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  {
            hero.move_leftt(elapsed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  {
            hero.move_right(elapsed);
        }
        //        collision
        for(auto &wall : walls) {
            sf::FloatRect heroBounds = hero.getGlobalBounds();
            sf::FloatRect wallBounds = wall.getGlobalBounds();
            heroBounds.left += vel_.x;
            heroBounds.top += vel_.y;

            if(wallBounds.intersects(heroBounds)){
                vel_.y = 0.f;
                vel_.x = 0.f;
                //top Collision
                if(heroBounds.top < wallBounds.top
                        && heroBounds.top  + heroBounds.height < wallBounds.top  + wallBounds.height
                        && heroBounds.left < wallBounds.left + wallBounds.width
                        && heroBounds.left + heroBounds.width > wallBounds.left){
                    hero.jump(elapsed);
                }
                //bottom Collision
                else if(heroBounds.top > wallBounds.top
                        && heroBounds.top  + heroBounds.height > wallBounds.top  + wallBounds.height
                        && heroBounds.left < wallBounds.left + wallBounds.width
                        && heroBounds.left + heroBounds.width > wallBounds.left){
                    hero.setPosition(heroBounds.left, wallBounds.top + wallBounds.height);
                }
                //Right Collision
                if(heroBounds.left < wallBounds.left
                        && heroBounds.left + heroBounds.width < wallBounds.left + wallBounds.width
                        && heroBounds.top < wallBounds.top + wallBounds.height
                        && heroBounds.top + heroBounds.height > wallBounds.top){
                    hero.setPosition(wallBounds.left - heroBounds.width, heroBounds.top);
                }
                //Left Collision
                else if(heroBounds.left > wallBounds.left
                        && heroBounds.left + heroBounds.width > wallBounds.left + wallBounds.width
                        && heroBounds.top < wallBounds.top + wallBounds.height
                        && heroBounds.top + heroBounds.height > wallBounds.top){
                    hero.setPosition(wallBounds.left + wallBounds.width, heroBounds.top);
                }
            }
        }
        window.clear(sf::Color::Black);

        sf::Texture texture;
        if (!texture.loadFromFile("grass.png")) {
            std::cerr << "Could not load texture" << std::endl;
            return 1;
        }
        texture.setRepeated(true);
        sf::Sprite grass;
        grass.setTexture(texture);
        grass.setTextureRect(sf::IntRect(0, 0, 800, 600));

        sf::Texture texture_wall;
        if(!texture_wall.loadFromFile("wall.png")) {
            return 1; }
        sf::Sprite wall;
        wall.setTexture(texture_wall);
        wall.setTextureRect(sf::IntRect(0, 0, 40, 300));
        texture_wall.setRepeated(true);

        //wall_1
        wall.setPosition(sf::Vector2f(50, 80));
        walls.push_back(wall);
        //wall_2;
        wall.setPosition(sf::Vector2f(250, 70));
        walls.push_back(wall);
        //wall_3;
        wall.setPosition(sf::Vector2f(450, 300));
        walls.push_back(wall);
        //wall_4;
        wall.setPosition(sf::Vector2f(700, 100));
        walls.push_back(wall);
        //wall_5;
        wall.setPosition(sf::Vector2f(250, 235));
        wall.setRotation(-90);
        walls.push_back(wall);
        //wall_6;
        wall.setPosition(sf::Vector2f(70, 520));
        wall.setRotation(-90);
        walls.push_back(wall);
        //wall_7;
        wall.setPosition(sf::Vector2f(450, 520));
        wall.setRotation(-90);
        walls.push_back(wall);

        // draw everything here...
        window.draw(grass);
        window.draw(hero);
        for(auto &wall_ : walls) {
            window.draw(wall_);
        }
        window.display();
    }
    return 0;
}
