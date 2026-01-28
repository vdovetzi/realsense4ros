#include <cv_bridge/cv_bridge.hpp>
#include <image_transport/image_transport.hpp>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <rclcpp/rclcpp.hpp>

using ImageTransport = image_transport::ImageTransport;
using TransportHints = image_transport::TransportHints;
using Publisher = image_transport::Publisher;
using CvImage = cv_bridge::CvImage;
using Header = std_msgs::msg::Header;
using namespace sensor_msgs::image_encodings;

int main(int argc, const char **argv) {
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("image_publisher");

  node->declare_parameter<std::string>("camera_frame", "realsense");
  node->declare_parameter<std::string>("output_topic", "realsense/image");

  auto camera_frame = node->get_parameter("camera_frame").as_string();
  auto output_topic = node->get_parameter("output_topic").as_string();

  auto it = std::make_unique<ImageTransport>(node);

  auto pub = std::make_unique<Publisher>(it->advertise(output_topic, 10));

  rs2::pipeline p;
  p.start();

  cv::Mat cv_frame;

  while (rclcpp::ok()) {
    rs2::frameset frames = p.wait_for_frames();
    rs2::video_frame color = frames.get_color_frame();
    if (!color)
      continue;

    if (cv_frame.empty()) {
      RCLCPP_INFO(node->get_logger(), "Successfully got first frame");
      cv_frame = cv::Mat(color.get_height(), color.get_width(), CV_8UC3);
    }
    cv_frame.data =
        (uchar *)reinterpret_cast<const uint8_t *>(color.get_data());

    cv::cvtColor(cv_frame, cv_frame, CV_BGR2RGB);

    auto header = Header();

    header.frame_id = camera_frame;
    header.stamp = node->now();

    auto msg = CvImage(header, BGR8, cv_frame).toImageMsg();

    pub->publish(msg);
  }
}
