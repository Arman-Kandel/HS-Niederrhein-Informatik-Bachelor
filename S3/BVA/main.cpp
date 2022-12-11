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

Mat modifyImageP2(Mat img, double fps) {
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
      if(pixel <= 10){ // Should be 0, but in test video there are only a few
        img.at<uchar>(i, j) = 255;
        countChanged++;
      }
      count++;
    }
  }
  //cout << "px:"<< count << "changed px:" << countChanged << "\n";
  //imshow("zImage-augh", img, fps);

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
  imshow("zImageBin-P2", imgBinary, fps);
  // Contours:
  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;
  findContours(imgBinary, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
  vector<Point> handContour;
  int biggestContour = 0;
  int biggestContourI = 0;
  for (int i = 0; i < contours.size(); i++) {
    vector<Point> contour = contours[i];
    if(contour.size() > biggestContour){
      biggestContour = contour.size();
      biggestContourI = i;
    }
  }
  drawContours(img, contours, biggestContourI, Scalar(0, 0, 0), 2);
  imshow("zImageContours-P2", img, fps);
  return histImg;
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
