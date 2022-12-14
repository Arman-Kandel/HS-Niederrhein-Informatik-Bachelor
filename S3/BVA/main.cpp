/****************************************************************************\
* Vorlage fuer das Praktikum "Bildverarbeitung" WS 2021/22
* FB 03 der Hochschule Niederrhein
* Christian Neumann, Regina Pohle-Froehlich
*
* Der Code basiert auf den C++-Beispielen der Bibliothek royale
\****************************************************************************/

// #define USE_LIBROYALE // Comment out if not needed

#include "camera.h"
#include "histogram.h"
#include <exception>
#include <future>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>
#include <shared_mutex>
#include <stdlib.h>
#include <string>
#include <thread>
#include <utility>

using namespace std;
using namespace cv;

map<string, VideoWriter> mapOpenWriters = {};
bool hasDisplay = std::getenv("DISPLAY") != nullptr;

void imshow(string windowName, Mat img, double fps = 60.0) {
  if (hasDisplay) {
    cv::imshow(windowName, img);
  } else {
    // Write to file
    if (img.channels() == 1) {
      // Convert the grayscale image to a colored image with 3 channels
      // Otherwise the media players cannot play the video or show the image
      cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
    }

    // Convert the image to the correct depth (8-bit unsigned)
    img.convertTo(img, CV_8UC3);

    VideoWriter writer;
    try {
      writer = mapOpenWriters.at(windowName);
    } catch (out_of_range e) { // When key not exists yet
      writer = VideoWriter(windowName + (".avi"), 0, fps,
                           Size(img.cols, img.rows), true);
      if (!writer.isOpened()) {
        cerr << "Could not open the VideoWriter for: " << windowName << endl;
        return;
      }
      mapOpenWriters[windowName] = writer;
      cout << "New VideoWriter for " << windowName << "\n";
    }
    writer << img;
  }
}

Mat modifyImageP1(Mat img) {
  // Check the number of channels in the input image
  // Since we need it to only have one, aka be grayscaled
  int channels = img.channels();

  // Convert the input image to grayscale if needed
  cv::Mat grayImage;
  if (channels == 3) {
    cv::cvtColor(img, grayImage, cv::COLOR_BGR2GRAY);
  } else if (channels == 4) {
    cv::cvtColor(img, grayImage, cv::COLOR_BGRA2GRAY);
  }

  // Create a mask that lets us ignore 0 values
  // since in this context 0 == no data
  Mat mask;
  compare(grayImage, 0, mask, CMP_GT);

  // Find min and max values
  double minZ, maxZ;
  cv::Point minLoc, maxLoc;
  minMaxLoc(grayImage, &minZ, &maxZ, &minLoc, &maxLoc, mask);

  // Lineare Skalierung
  // Scales, calculates absolute values, and converts the result to 8-bit.
  Mat imgScaled;
  double scale = 255.0 / (maxZ - minZ);
  double shift = -minZ * scale;
  convertScaleAbs(
      grayImage, // Input array
      imgScaled, // Result array
      scale,     // Multiplicative scale factor(optional) if 1 no change
      shift      // Additive offset // moves the histogram by this amount to the
                 // right, aka increases brightness if black-white picture
  );

  return imgScaled;
  // Color the image
  // Mat imgColored;
  // applyColorMap(imgScaled, imgColored, COLORMAP_RAINBOW);
  // return imgColored;
}

// Also fills small holes
Mat getForegroundBinary(Mat img) {
  // Check the number of channels in the input image
  // Since we need it to only have one, aka be grayscaled
  int channels = img.channels();

  // Convert the input image to grayscale if needed
  if (channels == 3) {
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
  } else if (channels == 4) {
    cv::cvtColor(img, img, cv::COLOR_BGRA2GRAY);
  }

  // Pixel gray value closer to 0 == closer to camera
  // Problem is that 0 == no data, and the whole background is 0/black
  // which should be furthest away, thus actually have 255/white as value.
  // This is fixed below:
  int count = 0, countChanged = 0;
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      // Get the pixel at the current row and column
      uchar pixel = img.at<uchar>(i, j);
      if (pixel <= 50) { // Should be 0, but in test video there are only a few
                         // 0 values...
        img.at<uchar>(i, j) = 255;
        countChanged++;
      }
      count++;
    }
  }
  // cout << "px:"<< count << "changed px:" << countChanged << "\n";
  // imshow("zImage-augh", img, fps);

  // Find threshold:
  vector<int> hist = getHistogram(img);
  hist = smoothHistogram(hist, 4);
  hist = smoothHistogram(hist, 4);
  hist = smoothHistogram(hist, 4);
  Mat histImg = getHistogramMat(hist, img.cols, img.rows);
  // Schwellwert:
  int minGray = drawMinMaxLines(hist, histImg, 50);
  Mat imgBinary;
  // With THRESH_BINARY all values below the threshold are
  // set to 0, and all above are set to max (255).
  // We need it the other way around:
  threshold(img, imgBinary, minGray, 255, THRESH_BINARY_INV);

  // Fill small holes
  // Create a kernel for the morphological operations
  Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
  // Dilate the image to expand the white pixels
  dilate(imgBinary, imgBinary, kernel);
  // Erode the image to shrink the white pixels
  erode(imgBinary, imgBinary, kernel);
  return imgBinary;
}

Mat modifyImageP2(Mat img, double fps) {
  Mat finalImg;
  img.clone().copyTo(finalImg);
  // Histogram:
  vector<int> hist = getHistogram(finalImg);
  hist = smoothHistogram(hist, 4);
  hist = smoothHistogram(hist, 4);
  hist = smoothHistogram(hist, 4);
  Mat histImg = getHistogramMat(hist, finalImg.cols, finalImg.rows);
  drawMinMaxLines(hist, histImg, 50);
  imshow("zImageHist-P2", histImg, fps);
  // Get binary image:
  Mat imgBinary = getForegroundBinary(finalImg);
  imshow("zImageBin-P2", imgBinary, fps);
  // Contours:
  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;
  // RETR_EXTERNAL -> retrieves only the extreme outer contour
  // RETR_TREE -> retrieves all of the contours and reconstructs a full
  // hierarchy of nested contours.
  findContours(imgBinary, contours, hierarchy, RETR_EXTERNAL,
               CHAIN_APPROX_NONE);
  int biggestContour = 0;
  int biggestContourI = 0;
  for (int i = 0; i < contours.size(); i++) {
    vector<Point> contour = contours[i];
    if (contour.size() > biggestContour) {
      biggestContour = contour.size();
      biggestContourI = i;
    }
  }
  drawContours(finalImg, contours, biggestContourI, Scalar(0, 0, 0), 2);
  return finalImg;
}

bool hasWhitePixelIn3x3(Mat img, Point startPoint) {
  Point p = startPoint;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      uchar val = img.at<uchar>(p);
      if (val == 255) {
        return true;
      }
      p.y = p.y + 1;
    }
    p.x = p.x + 1;
  }
  return false;
}

// Returns a list containing contour points
// that are the farthest away from the given convex hull.
// Together with its distance (pixel count) to the convex hull.
vector<pair<int, Point>> mydefects(Mat imgBinary, vector<Point> convexHull,
                                   Point centerPoint) {
  vector<pair<int, Point>> list;

  // Loop thorugh convex hulls edges/corners.
  Point lastConvexHullCornerPoint;
  bool first = true;
  for (Point convexHullCornerPoint : convexHull) {
    if (first) {
      first = false;
      lastConvexHullCornerPoint = convexHullCornerPoint;
      continue;
    }

    // Loop through all pixels of the current convex hull line.
    int maxDistance = -1;
    Point maxDistancePoint;
    LineIterator it(imgBinary, lastConvexHullCornerPoint, convexHullCornerPoint,
                    8);
    for (int i = 0; i < it.count; i++, ++it) {

      // Loop through all pixels from current point to center point.
      // Stop when pixel with 255 was found, and add it to the list.
      Point convexHullPoint = it.pos();
      LineIterator itj(imgBinary, convexHullPoint, centerPoint, 8);
      for (int j = 0; j < itj.count; j++, ++itj) {
        Point pt = itj.pos();
        uchar val = imgBinary.at<uchar>(pt);
        if (hasWhitePixelIn3x3(imgBinary, pt)) {
          if (j > maxDistance) {
            maxDistance = j;
            maxDistancePoint = pt;
          }
          break;
        }
      }
    }
    if (maxDistance != -1) {
      list.push_back(pair(maxDistance, maxDistancePoint));
    }

    lastConvexHullCornerPoint = convexHullCornerPoint;
  }
  return list;
}

Mat modifyImageP3(Mat img) {
  // Create copy of image with color:
  Mat finalImg;
  img.clone().copyTo(finalImg);
  Mat imgBinary = getForegroundBinary(finalImg);
  // Contours:
  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;
  findContours(imgBinary, contours, hierarchy, RETR_EXTERNAL,
               CHAIN_APPROX_NONE);
  // Find biggest contour (our hand):
  vector<Point> biggestContour;
  double biggestContourArea = 0;
  int biggestContourI = 0;
  for (int i = 0; i < contours.size(); i++) {
    vector<Point> contour = contours[i];
    double area = contourArea(contour, false);
    if (area > biggestContourArea) {
      biggestContour = contour;
      biggestContourArea = area;
      biggestContourI = i;
    }
  }
  // Draw the convex hull on the image
  vector<Point> approxCurve;
  approxPolyDP(biggestContour, approxCurve, 2, true); // close curve
  vector<Point> hull;
  vector<int> hullIndices;
  convexHull(approxCurve, hull, true);
  convexHull(approxCurve, hullIndices, false);
  // Convexity defects
  vector<Vec4i> defects;
  convexityDefects(biggestContour, hullIndices, defects);

  // we find the center of the bounding rectangle, this should approximately
  // also be the center of the hand
  Rect hullRect = boundingRect(Mat(hull));
  Point hullCenterPoint((hullRect.tl().x + hullRect.br().x) / 2,
                        (hullRect.tl().y + hullRect.br().y) / 2);

  // Draw everything:
  cv::cvtColor(finalImg, finalImg, cv::COLOR_GRAY2BGR);
  rectangle(finalImg, hullRect, Scalar(50, 50, 50), 1);
  drawContours(finalImg, std::vector<std::vector<Point>>{biggestContour}, -1,
               Scalar(0, 0, 255), 1);
  drawContours(finalImg, std::vector<std::vector<Point>>{approxCurve}, -1,
               Scalar(255, 0, 0), 1);
  drawContours(finalImg, std::vector<std::vector<Point>>{hull}, -1,
               Scalar(0, 255, 0), 1);
  vector<pair<double, Point>> sortedDefects;
  for (int i = 0; i < defects.size(); i++) {
    Vec4i defect = defects[i];
    // int startidx = defect[0];
    // Point ptStart(biggestContour[startidx]);
    // int endidx = defect[1];
    // Point ptEnd(biggestContour[endidx]);
    int faridx = defect[2];
    Point ptFar(biggestContour[faridx]);
    // int distance = defect[3]; // distance along the contour
    // int euclideanDistance = pointPolygonTest(biggestContour, ptFar, true);
    //  Euclidian Distance from hand center to point
    double distance = norm(ptFar - hullCenterPoint);
    sortedDefects.push_back(pair(distance, ptFar));
    // All are less bright at first
    circle(finalImg, ptFar, 4, Scalar(50, 50, 0), 2);
  }
  // Sort the pairs in ascending order according to the integer value.
  sort(sortedDefects.begin(), sortedDefects.end(),
       [](const auto &a, const auto &b) { return a.first < b.first; });
  // max 4 hand spaces
  int countDefects = 0;
  for (int i = 0; i < 4; i++) {
    try {
      circle(finalImg, sortedDefects[i].second, 4, Scalar(150, 150, 0), 2);
      countDefects++;
    } catch (exception ignored) {
    }
  }

  vector<pair<int, Point>> myDefects =
      mydefects(imgBinary, hull, hullCenterPoint);
  sort(myDefects.begin(), myDefects.end(), [](const auto &a, const auto &b) {
    return a.first > b.first;
  }); // biggest value at i == 0

  for (int i = 0; i < myDefects.size(); i++) {
    circle(finalImg, myDefects[i].second, 4, Scalar(0, 0, 150), 2);
  }

  string s = "Gesture: ";
  if (countDefects == 2) { // Geste 1
    s.append("1");
  } else if (countDefects == 3) { // Geste 2
    s.append("2");
  } else if (countDefects == 4) { // Geste 3
    s.append("3");
  } else {
    s.append("-");
  }
  s.append(" Defects: ").append(to_string(defects.size()));
  cv::putText(finalImg, s, Point(10, 10), cv::FONT_HERSHEY_DUPLEX, 0.4,
              Scalar(0, 155, 0), 1, false);

  // My detection
  string s2 = "(my) Gesture: ";
  if (myDefects.size() <= 5) { // Geste 1
    s2.append("1");
  } else if (myDefects.size() <= 7) { // Geste 2
    s2.append("2");
  } else if (myDefects.size() <= 9) { // Geste 3
    s2.append("3");
  } else {
    s2.append("-");
  }
  s2.append(" Defects: ").append(to_string(myDefects.size()));
  cv::putText(finalImg, s2, Point(10, 20), cv::FONT_HERSHEY_DUPLEX, 0.4,
              Scalar(0, 155, 0), 1, false);
  return finalImg;
}

int main(int argc, char *argv[]) {
  cout << "Arguments(" << argc << "): ";
  for (int i = 0; i < argc; i++) {
    cout << " " << argv[i];
  }
  cout << endl;

  camera camera = {0};

  //////////////////////////////////////////////////////////////////////////////////////

  if (argc < 2) {
    cerr << "Missing argument! Arguments are live, record and show-recording."
         << endl;
    return 1;
  }

  char *command = argv[1];
  if (strcmp(command, "live") == 0) { //////////////////////////////////// 1
    // Capture camera, show it in a window
    // and show analysed frames in another window
    cout << "Showing live camera analysis." << endl;
    cout << "Press any key to stop." << endl;
    camera.addFrameListener([](Mat frame) {
      imshow("Live-zImage", frame);
      imshow("Live-zImage-Modified", modifyImageP1(frame));
    });
    camera.startCapture();
    waitKey(0);
    camera.stopCapture();
  } else if (strcmp(command, "record") ==
             0) { //////////////////////////////////// 2
    // Capture camera and show in window
    // and save it to file (via VideoWriter)
    if (argc != 5) {
      cerr << "Wrong usage! Expected ... record <video-name> <max-size-px> "
              "<max-fps>"
           << endl;
      return 1;
    }
    cout << "Recording camera..." << endl;
    cout << "Press any key to stop." << endl;
    char *videoName = argv[2];
    auto finalName = string(videoName).append(".avi");
    int maxSize = atoi(argv[3]);
    double maxFps = atoi(argv[4]) + 0.0;
    VideoWriter videoWriter =
        VideoWriter(finalName, VideoWriter::fourcc('P', 'I', 'M', '1'), maxFps,
                    Size(maxSize, maxSize), true);
    if (!videoWriter.isOpened()) {
      cerr << "Could not open the VideoWriter for: " << finalName << endl;
      return 1;
    }
    camera.addFrameListener(
        [&videoWriter](Mat frame) { videoWriter << frame; });
    camera.startCapture();
    waitKey(0);
    camera.stopCapture();
  } else if (strcmp(command, "show-recording") ==
             0) { //////////////////////////////////// 3
    // Read video file with (VideoCapture)
    // and show modified and normal frames
    if (argc != 3) {
      cerr << "Wrong usage! Expected ... show-recording <video-name>" << endl;
      return 1;
    }

    char *videoName = argv[2];
    auto finalName = string(videoName).append(".avi");
    cout << "Showing video " << finalName << "..." << endl;
    auto capture = VideoCapture(finalName);
    if (!capture.isOpened()) {
      cerr << "Could not open the VideoCapture for: " << finalName << endl;
      return 1;
    }

    double fps = capture.get(cv::CAP_PROP_FPS);
    long readFramesCount = 0;
    Mat zImage;
    while (capture.read(zImage)) {
      if (zImage.empty())
        break;
      imshow("zImage", zImage, fps);
      Mat p1 = modifyImageP1(zImage);
      imshow("zImage-P1", p1, fps);
      Mat p2 = modifyImageP2(p1, fps);
      imshow("zImage-P2", p2, fps);
      Mat p3 = modifyImageP3(p1);
      imshow("zImage-P3", p3, fps);
      readFramesCount++;
    }
    cout << "Read " << readFramesCount << " frames.\n";

  } else { ////////////////////////////////////
    cerr << "Invalid argument! Allowed arguments are live, record and "
            "show-recording."
         << endl;
    return 1;
  }

  return 0;
}
