/****************************************************************************\
* Vorlage fuer das Praktikum "Bildverarbeitung" WS 2021/22
* FB 03 der Hochschule Niederrhein
* Christian Neumann, Regina Pohle-Froehlich
*
* Der Code basiert auf den C++-Beispielen der Bibliothek royale
\****************************************************************************/

// #define USE_LIBROYALE // Comment out if not needed

#include "camera.h"
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

int findeSchwellwert(Mat mat) {}

// index == pixel gray value
// value == total count of this gray value in this image
vector<int> getHistogram(Mat image) {
  // Create an array of bins to store the histogram values
  const int numBins = 256;
  vector<int> bins(numBins, 0); // Fill with zeros

  // Iterate over all the pixels in the input image
  for (int y = 0; y < image.rows; y++) {
    for (int x = 0; x < image.cols; x++) {
      // Get the intensity value of the current pixel
      uchar intensity = image.at<uchar>(y, x);

      // Increment the bin corresponding to the intensity value of the current
      // pixel
      bins[intensity]++;
    }
  }
  return bins;
}

Mat getHistogramMat(vector<int> histogram, int pxWidth, int pxHeight) {
  Mat mat = {pxHeight, pxWidth, CV_8UC3};
  Scalar color = {255, 255, 255};
  Point lastPoint = {0, 0};
  for (int i = 0; i < pxWidth; i++) {
    Point point;
    int count = histogram[i]; // throws exception if key doesn't exist
    point = {i, count};       // x, y
    line(mat, lastPoint, point, color, 1);
    lastPoint = point;
  }
  return mat;
}

// Expects the histogram to be complete aka have no missing values.
// Returns the local minimum, gray value
int drawMinMaxLines(vector<int> histogram, Mat histogramMat, int colsToSkip) {
  int lastLineHeight = 0;
  int lineHeight = 0;
  int maxLineHeight = 0;
  int maxLineCol = 0; // col == column
  int minLineHeight = 0;
  int minLineCol = 0; // col == column
  bool searchForFirstMax = true;
  for (int i = 0; i < histogramMat.cols; i++) {
    if (colsToSkip > 1) {
      colsToSkip--;
      continue;
    }
    lineHeight = histogram[i];
    if (searchForFirstMax) {
      if (lineHeight < lastLineHeight) {
        maxLineCol = i - 1;
        maxLineHeight = histogram[maxLineCol];
        searchForFirstMax = false;
      }
    } else { // search next minimum
      if (lineHeight > lastLineHeight) {
        minLineCol = i - 1;
        minLineHeight = histogram[minLineCol];
        break;
      }
    }
    lastLineHeight = lineHeight;
  }
  // local max == green line
  // local min == red line
  Scalar green = {0, 255, 0}; // green
  Scalar red = {0, 0, 255};   // BGR red
  Point maxStart = {maxLineCol, 0};
  Point minStart = {minLineCol, 0};
  Point maxEnd = {maxLineCol, histogramMat.rows};
  Point minEnd = {minLineCol, histogramMat.rows};
  line(histogramMat, maxStart, maxEnd, green, 1);
  line(histogramMat, minStart, minEnd, red, 1);
  return minLineCol;
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

// Returns smoothed histogram via an average filter
vector<int> smoothHistogram(vector<int> hist, int relevantNeighborsCount = 2) {
  std::vector<int> finalHist(256);

  for (int i = 0; i < 256; ++i) {
    int total = 0;
    int ii = i - relevantNeighborsCount;
    int iEnd = i + relevantNeighborsCount;
    for (; ii < iEnd; ii++) {
      try{
        total += hist[ii];
      } catch(exception ignored){}
    }
    finalHist[i] = total / relevantNeighborsCount + 1; // +1 since only neighbor count given
  }
  return finalHist;
}

Mat modifyImageP2(Mat img, double fps) {
  vector<int> hist = getHistogram(img);
  hist = smoothHistogram(hist, 10);
  Mat histImg = getHistogramMat(hist, img.cols, img.rows);
  // Schwellwert:
  int minGray = drawMinMaxLines(hist, histImg, 10);
  Mat imgBinary;
  threshold(img, imgBinary, minGray, 255, THRESH_BINARY);
  imshow("zImageBin-P2", imgBinary, fps);
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
