#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <cmath> 
#include <functional>
#include <std_msgs/msg/string.hpp>

class DestroyerMove : public rclcpp::Node
{
  public:
    DestroyerMove() : Node("move_mode_destroyer"), x_(0.0), y_(0.0), theta_(0.0), total_dist_(0.0){
        power_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/power_destroyer", 10, std::bind(&DestroyerMove::power_cb, this, std::placeholders::_1));
        
        // 2. Dengarkan kabel tipe kendali dari Anchovy
        type_sub_ = this->create_subscription<std_msgs::msg::String>(
            "/move_type", 10, std::bind(&DestroyerMove::type_cb, this, std::placeholders::_1));

      RCLCPP_INFO(this->get_logger(), "Destroyer Status: ONLINE!");
    }
  private:
  void power_cb(const geometry_msgs::msg::Twist::SharedPtr msg) {
    double vx = msg->linear.x;
    double vy = msg->linear.y;
    double omega = msg->angular.z;
    double dx = vx * cos(theta_) - vy * sin(theta_);
    double dy = vx * sin(theta_) + vy * cos(theta_); 
    

    x_ += dx;
    y_ += dy;


    theta_ += omega;


    if (vx != 0.0 || vy != 0.0) {
        total_dist_ += std::sqrt(vx*vx + vy*vy);
        
        RCLCPP_INFO(this->get_logger(), 
            "Avanti! Moving to: (%.2f, %.2f) | Dist: %.2f", 
            x_, y_, total_dist_);
    }


    if (omega != 0.0) {
        RCLCPP_INFO(this->get_logger(), "Turning! New Heading: %.2f rad", theta_);
    }
    std::string move = "Diam";
        
        if (msg->linear.x > 0) move = "Maju";
        else if (msg->linear.x < 0) move = "Mundur";
        else if (msg->linear.y > 0) move = "Geser Kiri";
        else if (msg->linear.y < 0) move = "Geser Kanan";
        else if (msg->angular.z > 0) move = "Berputar Kiri";
        else if (msg->angular.z < 0) move = "Berputar Kanan";
        
        RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: %s", move.c_str());
    }


    void type_cb(const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Tipe Kendali Aktif: %s", msg->data.c_str());
    }
    double x_, y_, theta_, total_dist_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr power_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr type_sub_;
};



int main(int argc, char * argv[])
{
  rclcpp::init(argc,argv);
  auto node = std::make_shared<DestroyerMove>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}