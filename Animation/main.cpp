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
        //load the texture here
        sf::Texture texture;
        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        //set Texture
        setTexture(texture_);
        setTextureRect(sf::IntRect(50, 0, 50, 37));
    }
    void setSpeed(const int& x_speed, const int& y_speed, const int & ro_speed) {
        x_speed_ = x_speed;
        y_speed_ = y_speed;
        ro_speed_ = ro_speed;
    }
    void fall(const sf::Time &elapsed) {
        float dt = elapsed.asSeconds();
        int g = 20;
        falling_t_ = falling_t_ + dt;
        //        float d = 0.5 * g * pow(fallin_t_,2); //1/2gt^2
        move(0,0.5 * g * pow(falling_t_,2));
    }
    void animate(const sf::Time &elapsed){
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

        // based on dt and fps_ fins fragments_index here
        setTextureRect(running_frames[fragments_index]);
        move(x_speed_*dt,y_speed_*dt);
    }
    void setBounds(const float& l_bound, const float& r_bound,const float& u_bound,const float& d_bound){
        l_bound_  = l_bound  ;
        r_bound_  = r_bound  ;
        u_bound_  = u_bound  ;
        d_bound_  = d_bound  ;
    }
    void add_animation_frame(const sf::IntRect& frame){
        //add animation frames to vector
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
    float falling_t_ = 0.0;
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
    hero.setSpeed(30,30,10);
    //hero.add_animation_frame(sf::IntRect(0, 0, 50, 37)); // hero standing frame 1
    //hero.add_animation_frame(sf::IntRect(50, 0, 50, 37)); // hero standing frame 2
    //hero.add_animation_frame(sf::IntRect(100, 0, 50, 37)); // hero standing frame 3
    hero.add_animation_frame(sf::IntRect(150, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(200, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(250, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(300, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(350, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(400, 0, 50, 37)); // hero running frame 1

    sf::Clock clock;
//    sf::FloatRect new_Pos_;
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
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  {
            hero.animate(elapsed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  {
            hero.fall(elapsed);
        }
        //        collision
        for(auto &wall : walls) {
            sf::FloatRect heroBounds = hero.getGlobalBounds();
            sf::FloatRect wallBounds = wall.getGlobalBounds();
//            new_Pos_ = heroBounds;
            heroBounds.left += vel_.x;
            heroBounds.top += vel_.y;

            if(wallBounds.intersects(heroBounds)){
                vel_.y = 0.f;
                vel_.x = 0.f;
                //Bottom Collision
                if(heroBounds.top < wallBounds.top
                        && heroBounds.top  + heroBounds.height < wallBounds.top  + wallBounds.height
                        && heroBounds.left < wallBounds.left + wallBounds.width
                        && heroBounds.left + heroBounds.width > wallBounds.left){
                    hero.setPosition(heroBounds.left, wallBounds.top - heroBounds.height);
                }
                //Top Collision
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
            hero.animate(elapsed);
        }
        window.clear(sf::Color::Black);

        sf::Texture texture;
        if (!texture.loadFromFile("grass.png")) {
            std::cerr << "Could not load texture" << std::endl;
            return 1;
        }
        texture.setRepeated(true);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, 800, 600));

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
        //wall_4;
        wall.setPosition(sf::Vector2f(450, 300));
        walls.push_back(wall);
        //wall_5;
        wall.setPosition(sf::Vector2f(700, 100));
        walls.push_back(wall);
        //wall_6;
        wall.setPosition(sf::Vector2f(250, 235));
        wall.setRotation(-90);
        walls.push_back(wall);
        //wall_7;
        wall.setPosition(sf::Vector2f(70, 520));
        wall.setRotation(-90);
        walls.push_back(wall);
        //wall_8;
        wall.setPosition(sf::Vector2f(450, 520));
        wall.setRotation(-90);
        walls.push_back(wall);

        // draw everything here...
        window.draw(sprite);
        window.draw(hero);
        for(auto &wall_ : walls) {
            window.draw(wall_);
        }
        // end the current frame
        window.display();
    }
    return 0;
}