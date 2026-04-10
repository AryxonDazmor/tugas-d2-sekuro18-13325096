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

    // Duce, Pepperoni-san told me that we might should use Omni wheels instead..
    // Eh? Why is that
    // Kazuma-san said something about better manuverability
    // Maybe we should just give them our CV.33..
    // Duce taht's our tank
    // Grr fine

    //Can you explain the math Carpaccio-san?
    //Omni wheels are basically a wheel with rollers around it so that it can move in two directions without turning the wheels
    //Ahh so that's why Pepperoni wants that..
    //Anyways, can you explain more? We need to code it y'know?
    //So, we need to look at each of the wheels movement
    //If we want to move left, the FR and BL needs to move forward, and FL and BR needs to move backward to balance it!
    //Hence:
    //FL_ = vx - vy;
    //FR_ = vx + vy;
    //BL_ = vx + vy;
    //BR_ = vx - vy;
    //But we need to also account for it turning!
    //Ahh but Kazuma-san doesn't give the dimension. 
    //Let's just ask kazuma-san to input it himself no?
    //Okay Carpaccio-san.
    //For now, we'll assume, from the centre, the distance is 1.0.
    //Won't the dimension matter, it will be diffrent if it's a rectangle right?
    //Ehh I mean Kazuma-san can use the pythagorean theorem and half them to get the distance. We can just tell Kazuma-san that!
    double length_ = 1.0;
    double width_ = 0.0; //Uwahh that makes no sense! It's an invisible line! Kazuma-san, you should change this! You should've given us the dimension!!
    double r = std::sqrt(std::pow(length_/2, 2) + std::pow(width_/2, 2));
    //Okay, now we get the distance from the centre to the wheel
    //We know that linear speed is equal to the angular speed multiplied by the radii
    //And to turn left the left wheels needs to move backwards, and the right wheels needs to move forward. And vice versa
    //Hence:
    double FL_ = vx - vy - omega * r;
    double FR_ = vx + vy + omega * r;
    double BL_ = vx + vy - omega * r;
    double BR_ = vx - vy + omega * r;

    RCLCPP_INFO(this->get_logger(), 
        "Omni Wheels -> FL: %.2f | FR: %.2f | BL: %.2f | BR: %.2f", 
        FL_, FR_, BL_, BR_);

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