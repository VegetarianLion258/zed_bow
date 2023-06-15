#include <iostream>
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(void) {
  // 配置图像路径
  Mat img1 = imread("/home/tanhaozhang/zed_bow/image_data/8.png", 0);
  Mat img2 = imread("/home/tanhaozhang/zed_bow/image_data/10.png", 0);
  // 判断输入图像是否读取成功
  if (img1.empty() || img2.empty() || img1.channels() != 1 ||
      img2.channels() != 1) {
    std::cout << "Input Image is nullptr or the image channels is not gray!" << endl;
    system("pause");
  }
  // ORB算法继承Feature2D基类
  Ptr<ORB> orb = ORB::create(400, 1.2, 8, 31, 0, 2, 0, 31, 20);
  // 调整精度，值越小点越少，越精准
  vector<KeyPoint> kpts1, kpts2;
  // 特征点检测算法...
  orb->detect(img1, kpts1);
  orb->detect(img2, kpts2);

  // 特征点描述算法...
  Mat desc1, desc2;


  // 默认选择BRIEF描述符
  if (1) {
    // ORB 算法中默认BRIEF描述符
    orb->compute(img1, kpts1, desc1);
    orb->compute(img2, kpts2, desc2);
  }
  // 粗精匹配数据存储结构
  vector<vector<DMatch>> matches;
  vector<DMatch> goodMatchKpts;
  // Keypoint Matching...
  DescriptorMatcher *pMatcher = new BFMatcher(NORM_HAMMING, false);
  pMatcher->knnMatch(desc1, desc2, matches, 2);
  // 欧式距离度量  阈值设置为0.8
  for (unsigned int i = 0; i < matches.size(); ++i) {
    if (matches[i][0].distance < 0.8 * matches[i][1].distance) {
      goodMatchKpts.push_back(matches[i][0]);
    }
  }
  // 显示匹配点对
  Mat show_match;
  drawMatches(img1, kpts1, img2, kpts2, goodMatchKpts, show_match);

  // 显示输出
  ostringstream s_time;
  s_time << time;
  imshow("ORB_Algorithms_" + s_time.str(), show_match);

  cout << "(kpts1: " << kpts1.size() << ") && (kpts2:" << kpts2.size()
       << ") = goodMatchesKpts: " << goodMatchKpts.size() << endl;

  waitKey(0);

  // RANSAC Geometric Verification
  if (goodMatchKpts.size() < 4) {
    cout << "The Match Kpts' Size is less than Four to estimate!" << endl;
    return 0;
  }

  vector<Point2f> obj, scene;
  for (unsigned int i = 0; i < goodMatchKpts.size(); ++i) {
    obj.push_back(kpts1[goodMatchKpts[i].queryIdx].pt);
    scene.push_back(kpts2[goodMatchKpts[i].trainIdx].pt);
  }
  // 估计Two Views变换矩阵
  Mat H = findHomography(obj, scene, CV_RANSAC);
  vector<Point2f> obj_corners(4), scene_corners(4);
  obj_corners[0] = cvPoint(0, 0);
  obj_corners[1] = cvPoint(img1.cols, 0);
  obj_corners[2] = cvPoint(img1.cols, img1.rows);
  obj_corners[3] = cvPoint(0, img1.rows);
  // 点集变换标出匹配重复区域
  perspectiveTransform(obj_corners, scene_corners, H);

  line(show_match, scene_corners[0] + Point2f(img1.cols, 0),
       scene_corners[1] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);
  line(show_match, scene_corners[1] + Point2f(img1.cols, 0),
       scene_corners[2] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);
  line(show_match, scene_corners[2] + Point2f(img1.cols, 0),
       scene_corners[3] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);
  line(show_match, scene_corners[3] + Point2f(img1.cols, 0),
       scene_corners[0] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);

  imshow("Match End", show_match);
  imwrite("img_boat15.jpg", show_match);
  waitKey(0);
  system("pause");
  return 0;
}
