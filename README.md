# realsense4ros

A lightweight ROS2 node for publishing images from Intel Realsense cameras with minimal dependencies and configuration.

## Why This Package?
The existing [realsense-ros](https://github.com/realsenseai/realsense-ros) package provides comprehensive functionality but can be overly complex for simple image publishing tasks.

## Dependencies
- ROS2 Jazzy 

- Intel RealSense SDK 2.0 (librealsense2, version 2.56+)

- OpenCV 4.x

ROS2 stuff:

- rclcpp

- image_transport

- cv_bridge

## Installation
1. Install librealsense2. Follow instructions from official [repo](https://github.com/IntelRealSense/librealsense).
2.  Install ROS2 deps:
```bash
sudo apt-get install ros-jazzy-image-transport ros-jazzy-cv-bridge
```

## Building
```bash
colcon build --packages-select realsense4ros
source install/setup.bash
```

## Usage

### Running the node
```bash
ros2 run realsense4ros image_publisher
```
### Parameters
- `camera_frame` (string, default: "realsense"): TF frame ID for the camera
- `input_topic` (string, default: "realsense/image"): Topic to publish images to

Example with custom parameters:
```bash
ros2 run realsense4ros image_publisher --ros-args -p camera_frame:=camera_link -p input_topic:=/camera/color/image_raw
```

## Troubleshooting
1. Camera not detected
```bash
realsense-viewer
# Verify camera works with official tools first
```
2. Build errors:
- Ensure librealsense2 is installed and discoverable by CMake
- Check ROS 2 dependencies are installed


**Note**: This package is maintained for simplicity and specific use cases. For production systems with complex requirements, the official realsense-ros package is recommended.