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
#include <stdlib.h>
#include <string>
#include <thread>

using namespace std;
using namespace cv;

int findeSchwellwert(Mat mat) {}

map<int, int> getHistogram(Mat image) {
  // key == pixel gray value 
  // value == total count of this gray value in this image  
  map<int, int> map = {}; 

  for (int y = 0; y < image.rows; y++) {
    for (int x = 0; x < image.cols; x++) {
      int grayValue = image.at<int>(y, x);
      try {
        int count = map.at(grayValue); // throws exception if key doesn't exist
        map[grayValue] = count + 1;
      } catch (exception e) {
        map[grayValue] = 1; // means that key wasnt added yet
      }
    }
  }
  return map;
}

void getHistogramMat(map<int, int> histogram, int pxWidth, int pxHeight) {
  Mat mat = {pxHeight, pxWidth, CV_32FC1};
  Scalar color = {255, 255, 255};
  Point lastPoint = {0, 0};
  for (int i = 0; i < pxWidth; i++) {
    Point point;
    try {
      int count = histogram.at(i); // throws exception if key doesn't exist
      point = {i, count};          // x, y
    } catch (exception e) {
      histogram[i] = 0; // means that key wasnt added yet
      // thus insert 0 to have a complete histogram
     point = {i, 0};
    }
    line(mat, lastPoint, point, color, 1);
    lastPoint = point;
  }
}

/*
Ignore 0 values in image with a mask compare(), inside minMaxLoc()
then apply a color map like COLORMAP RAINBOW
*/
Mat modifyImage(Mat zImage) {

  // Create a mask that lets us ignore 0 values
  // since in this context 0 == no data
  Mat mask;
  compare(zImage, 0, mask, CMP_GT);

  // Find min and max values
  double minZ, maxZ;
  minMaxLoc(zImage, &minZ, &maxZ, 0, 0, mask);

  // Lineare Skalierung
  // Scales, calculates absolute values, and converts the result to 8-bit.
  Mat imgScaled;
  double scale = 255.0 / (maxZ - minZ);
  double shift = -minZ * scale;
  convertScaleAbs(
      zImage,    // Input array
      imgScaled, // Result array
      scale,     // Multiplicative scale factor(optional) if 1 no change
      shift      // Additive offset // moves the histogram by this amount to the
                 // right, aka increases brightness if black-white picture
  );

  // Color the image
  Mat imgColored;
  applyColorMap(imgScaled, imgColored, COLORMAP_RAINBOW);

  return imgColored;
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
  if (strcmp(command, "live") == 0) { ////////////////////////////////////
    // Capture camera, show it in a window
    // and show analysed frames in another window
    cout << "Showing live camera analysis." << endl;
    cout << "Press any key to stop." << endl;
    camera.addFrameListener([](Mat frame) {
      imshow("Live-zImage", frame);
      imshow("Live-zImage-Modified", modifyImage(frame));
    });
    camera.startCapture();
    waitKey(0);
    camera.stopCapture();
  } else if (strcmp(command, "record") ==
             0) { ////////////////////////////////////
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
             0) { ////////////////////////////////////
    // Read video file with (VideoCapture)
    // and show modified and normal frames
    if (argc != 3) {
      cerr << "Wrong usage! Expected ... 3 <video-name>" << endl;
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

    Mat zImage;
    while (capture.read(zImage)) {
      imshow("zImage", zImage);
      imshow("zImage-Modified", modifyImage(zImage));
      waitKey(10);
    }
  } else { ////////////////////////////////////
    cerr << "Invalid argument! Allowed arguments are live, record and "
            "show-recording."
         << endl;
    return 1;
  }

  return 0;
}
