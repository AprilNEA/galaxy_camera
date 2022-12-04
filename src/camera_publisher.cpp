#include <sstream>

#include "cv_bridge/cv_bridge.h"
#include "image_transport/image_transport.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/header.hpp"
#include "galaxy_camera.h"

int main(int argc, char **argv) {
    // Check if video source has been passed as a parameter
//  if (argv[1] == NULL) {
//    return 1;
//  }

    rclcpp::init(argc, argv);
    rclcpp::NodeOptions options;
    rclcpp::Node::SharedPtr node = rclcpp::Node::make_shared("image_publisher", options);
    image_transport::ImageTransport it(node);
    image_transport::Publisher pub = it.advertise("galaxy_camera/image", 1);

    // Convert the command line parameter index for the video device to an integer
//  std::istringstream video_sourceCmd(argv[1]);
//  int video_source;
//
//  // Check if it is indeed a number
//  if (!(video_sourceCmd >> video_source)) {
//    return 1;
//  }

//  cv::VideoCapture cap(video_source);
    // Check if video device can be opened with the given index
//  if (!cap.isOpened()) {
//    return 1;
//  }


    std_msgs::msg::Header hdr;
    sensor_msgs::msg::Image::SharedPtr msg;

    cv::Mat frame;
    std::shared_ptr<Camera> camera_left = nullptr;
    camera_left = std::make_shared<DHCamera>("KN0210060029");
    camera_left->init(0, 0, 640, 384, 10000, 10, false);

    rclcpp::WallRate loop_rate(60);
    while (rclcpp::ok()) {
        if (!camera_left->start()) {
            std::cout << "NO Camera or Just one,please check" << std::endl;
            return 1;
        }
        camera_left->read(frame);

        if (!frame.empty()) {
            msg = cv_bridge::CvImage(hdr, "bgr8", frame).toImageMsg();
            pub.publish(msg);
            cv::waitKey(1);
        } else {
            std::cout << "IMG IS EMPTY" << std::endl;
            return 1;
        }
        // Check if grabbed frame is actually full with some content
        if (!frame.empty()) {
            msg = cv_bridge::CvImage(hdr, "bgr8", frame).toImageMsg();
            pub.publish(msg);
            cv::waitKey(1);
        }

        rclcpp::spin_some(node);
        loop_rate.sleep();
    }

    return 0;
}
