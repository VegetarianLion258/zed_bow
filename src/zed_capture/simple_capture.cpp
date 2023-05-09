#include "/home/tanhaozhang/zed_bow/3rd/zed_open_capture/include/zed-open-capture/videocapture.hpp"
#include <iostream>
#include <iomanip>

#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
  // ----> Silence unused warning
  (void)argc;
  (void)argv;
  // <---- Silence unused warning

  sl_oc::VERBOSITY verbose = sl_oc::VERBOSITY::INFO; 
  // ----> Set Video parameters
  sl_oc::video::VideoParams params;
  params.res = sl_oc::video::RESOLUTION::VGA;
  params.fps = sl_oc::video::FPS::FPS_30;
  params.verbose = verbose;
  // <---- Set Video parameters
  unsigned int image_id = 0;
  // ----> Create Video Capture
  sl_oc::video::VideoCapture cap(params);
  if (!cap.initializeVideo(-1))
  {
    std::cerr << "Cannot open camera video capture" << std::endl;
    std::cerr << "See verbosity level for more details." << std::endl;

    return EXIT_FAILURE;
  }
  std::cout << "Connected to camera sn: " << cap.getSerialNumber() << std::endl;
  // <---- Create Video Capture

  // ----> Create rendering window
  cv::namedWindow("left_raw");
  u_int64_t last_time;
  while (1) {
    const sl_oc::video::Frame frame = cap.getLastFrame();
    if(frame.data != nullptr && frame.timestamp != last_time) {
      last_time = frame.timestamp;
      cv::Mat frameYUV = cv::Mat(frame.height, frame.width, CV_8UC2, frame.data);
      cv::Mat frameBGR, left_raw, right_raw;
      cv::cvtColor(frameYUV, frameBGR, cv::COLOR_YUV2BGR_YUYV);
      right_raw = frameBGR(cv::Rect(0, 0, frameBGR.cols / 2, frameBGR.rows));
      left_raw = frameBGR(cv::Rect(frameBGR.cols / 2, 0, frameBGR.cols / 2, frameBGR.rows));
      cv::imshow("left_raw", left_raw);
      // cv::imshow("right_raw", right_raw);
      int key = cv::waitKey(10);
      if (key == 113 || key == 81) { //Q for quit
        break;
      }
      // else if (key == 32)
      // {
      //   cv::waitKey()
      // }
      else if (key == 115) {
        image_id ++;
        cv::Mat grey_left;
        cv::cvtColor(left_raw, grey_left, CV_BGR2GRAY);
        cv::imwrite("/home/tanhaozhang/zed_bow/image_data/" + std::to_string(image_id) + ".png", grey_left);
        std::cout << "Save image " + std::to_string(image_id) + ".png" << std::endl;
        // image_id ++;
        // cv::imwrite("/home/tanhaozhang/zed/image_data/" + std::to_string(image_id) + ".png", right_raw);
        // std::cout << "Save image " + std::to_string(image_id) + ".png" << std::endl;
      }
    }
  
  }
} 
