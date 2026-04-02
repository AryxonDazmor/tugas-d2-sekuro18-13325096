#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <random>
#include <chrono>

using namespace std::chrono_literals;

class DestroyerAuto : public rclcpp::Node
{
  public:
    DestroyerAuto() : Node("auto_mode_destroyer")
    {
      publisher_ = this-> create_publisher<geometry_msgs::msg::Twist>("/autonomous_destroyer", 10);
      timer_ = this-> create_wall_timer(500ms, std::bind(&DestroyerAuto::timer_callback, this));
      RCLCPP_INFO(this->get_logger(), "Anzio Autonomous Destroyer: ON");
    }
  private: 
    void timer_callback(){
      //Duce, I don't understand, what type of code is this?
      //This is a randomised speed generator!
      std::random_device rd;
      // This is to pick random starting value
      std::mt19937 gen(rd());
      // This an algorithm to spit random numbers
      std::uniform_real_distribution<> dist(-5.0, 5.0);
      //And we set the boundaries!
      //Thank you duce!

      auto msg = geometry_msgs::msg::Twist();
      msg.linear.x = dist(gen); //This is to move forward and backward
      msg.linear.y = dist(gen); //This is to move left and right
      msg.angular.z = dist(gen); //This is to rotate the ship
      publisher_->publish(msg);

      RCLCPP_INFO(this->get_logger(), "Auto Published: linear.x=%.2f, linear.y=%.2f, yaw=%.2f", 
                  msg.linear.x, msg.linear.y, msg.angular.z);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv); //Duce, what is argc argv? 
  //argc -> How many arguments?
  //argv -> What are they?
  //Thank you Duce! As expected of Duce! So Smart!
  rclcpp::spin(std::make_shared<DestroyerAuto>());
  rclcpp::shutdown();
  return 0;
}