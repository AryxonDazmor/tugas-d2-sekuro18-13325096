#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <termios.h>
#include <unistd.h>
#include <iostream>
//So.. Duce, are you telling that magic guy to control the tank with a keyboard?
//Hey, make it simple okay? Also, who is that magic guy name again?
//Kazuma?
//Ah yes that. 

int getch() {
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int ch = getchar();
    //This waits input. If a keyboard input is detected -> it will return a value.
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
class DestroyerDrive : public rclcpp::Node
{
    public:
        DestroyerDrive() : Node("destroyer_drive") {
          publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/drive_destroyer", 10);
          RCLCPP_INFO(this->get_logger(), "Destroyer Drive Node Started.");
          RCLCPP_INFO(this->get_logger(), "Alright, the basics. Hit that gas pedal (shift) to accelerate. This wheel (AD) is to turn.");
        }

        void run() {
          geometry_msgs::msg::Twist twist;
          while (rclcpp::ok()) {
            int ch = getch();
            twist.linear.x = 0.0;
            twist.linear.y = 0.0;
            twist.linear.z = 0.0;
            twist.angular.x = 0.0;
            twist.angular.y = 0.0;
            twist.angular.z = 0.0;

            double speed_multiplier = 1.0;
            if (ch >= 'A' && ch <= 'Z') {
                RCLCPP_INFO(this->get_logger(), "Speed desu-wa!"); //Oi Carpaccio why is Rosehip here?!
                speed_multiplier = 2.5; 
                ch = std::tolower(ch);  
            }
            if(ch == 'w' || ch == 65){
              twist.linear.x = 5.0 * speed_multiplier;
              RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: Maju");
            }else if (ch == 's' || ch == 66){
              twist.linear.x = -5.0 * speed_multiplier;
              RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: Mundur");
            }
            else if (ch == 'a' || ch == 68){
              twist.linear.y = 5.0 * speed_multiplier;
              RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: Kiri");
            }
            else if (ch == 'd' || ch == 67){
              twist.linear.y = -5.0 * speed_multiplier;
              RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: Kanan");
            }
            else if (ch == 'l'){
              twist.angular.z = 1.0 * speed_multiplier;
              RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: Belok Kiri");
            }
            else if (ch == 'r'){
              twist.angular.z = -1.0 * speed_multiplier;
              RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: Belok Kanan");
            }
            else if (ch == 'b'){
              twist.linear.x = 0.0;
              twist.linear.y = 0.0;
              RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: Berhenti"); //Added brakes! We forgor
            }
            publisher_->publish(twist);
          }
        }
    private:
          rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};



int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<DestroyerDrive>();
    node->run();
    rclcpp::shutdown();
    return 0;
}
