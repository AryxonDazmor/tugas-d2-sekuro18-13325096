#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/string.hpp>

class DestroyerCheck : public rclcpp::Node
{
  //Duce, what is this file for?
  //This is the tank Kommandant! They listen to the sensors and shout commands!
  //Duce, I don't think they asked for a tank..
  //Well a tank is a destroyer!
  //Duce we are an Italian themed highschoolers, why are we creating tanks for some magical people?
  //Because we need money okay? And Anzio is not weak, no we are strong! 
  //Duce I am concerned
  //Uh...
  public:
    DestroyerCheck() : Node("check_move_destroyer")
    {
      auto_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
        "/autonomous_destroyer", 10, 
        std::bind(&DestroyerCheck::auto_callback, this, std::placeholders::_1));
      manual_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
        "/drive_destroyer", 10, 
        std::bind(&DestroyerCheck::manual_callback, this, std::placeholders::_1));
      final_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/power_destroyer", 10);
      type_pub_ = this->create_publisher<std_msgs::msg::String>("/move_type", 10);
      last_manual_time_ = this->now();
    }
  private:
    void auto_callback(const geometry_msgs::msg::Twist::SharedPtr msg){
      //This is when Amarreto is not driving
      //Duce, isn't Amarreto is the L3 tank driver?
      //Shut up Carpaccio the writers didn't give the P40 tank driver name, we are not Oarai dammit. Anyways this is for autopilot!
      //Duce are we sending Amaretto to those magicians?
      //Wait yeah.. I hope that Konosuba guy knows how to drive this thing
      //But we will still command Amaretto to test this!
      //Duce, didn't you say this is for the autopilot?
      auto now = this->now();
      auto diff = now - last_manual_time_;
      if (diff.seconds() > 2.0) {
        RCLCPP_INFO(this->get_logger(), "Received from Autonomous: linear.x=%.2f, linear.y=%.2f, yaw=%.2f", 
                    msg->linear.x, msg->linear.y, msg->angular.z);
        final_pub_->publish(*msg);
        auto string_msg = std_msgs::msg::String();
        string_msg.data = "Autonomous";
        type_pub_->publish(string_msg);
      }
    }

    void manual_callback(const geometry_msgs::msg::Twist::SharedPtr msg){
      //Duce, where is Pepperoni?
      //I told Pepperoni to talk with those magicians.
      //Anyways if Amarretto decided to drive, this will override it
      //Duce why are we talking here
      //uhh
      
      last_manual_time_ = this->now();  
      RCLCPP_INFO(this->get_logger(), "Received from Driver: linear.x=%.2f, linear.y=%.2f, yaw=%.2f", 
                  msg->linear.x, msg->linear.y, msg->angular.z);
                  
      final_pub_->publish(*msg);
      auto string_msg = std_msgs::msg::String();
      string_msg.data = "Driver";
      type_pub_->publish(string_msg);
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr auto_sub_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr manual_sub_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr final_pub_;
    rclcpp::Time last_manual_time_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr type_pub_;

};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DestroyerCheck>());
    rclcpp::shutdown();
    return 0;
}