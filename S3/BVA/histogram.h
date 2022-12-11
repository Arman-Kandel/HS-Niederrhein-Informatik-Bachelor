#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Returns smoothed histogram via an average filter
vector<int> smoothHistogram(vector<int> hist, int relevantNeighborsCount = 2) {
  std::vector<int> finalHist(256);

  for (int i = 0; i < 256; ++i) {
    int total = 0;
    int ii = i - relevantNeighborsCount;
    int iEnd = i + relevantNeighborsCount;
    for (; ii <= iEnd; ii++) {
      try {
        total += hist[ii];
      } catch (exception ignored) {
      }
    }
    finalHist[i] = total / ((relevantNeighborsCount * 2) +
                            1); // +1 since only neighbor count given
  }
  return finalHist;
}

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
  Scalar cyan = {232, 217, 56}; // BGR cyan
  // Scalar white = {255, 255, 255};
  Mat mat = {pxHeight, pxWidth, CV_8UC3};
  Point lastPoint = {0, 0};
  for (int i = 0; i < pxWidth; i++) {
    Point point;
    int posY = histogram[i]; // throws exception if key doesn't exist
    posY = pxHeight -
           posY; // Because opencv starts draw at top left, not bottom left
    point = {i, posY}; // x, y
    line(mat, lastPoint, point, cyan, 1);
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
  if (colsToSkip >= histogramMat.cols) {
    cerr << "Columns to skip (" << colsToSkip
         << ") is equal or bigger than histogram width (" << histogramMat.cols
         << ").\n";
    return 0;
  }
  int _colsToSkip = colsToSkip;
  for (int i = 0; i < histogramMat.cols; i++) {
    if (_colsToSkip > 1) {
      _colsToSkip--;
      continue;
    }
    lineHeight = histogram[i];
    if (searchForFirstMax) {
      // lineHeight 10
      // lastLineHeight 0
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
  Scalar green = {0, 255, 0}; // BGR green
  Scalar red = {0, 0, 255};   // BGR red
  Point maxStart = {maxLineCol, 0};
  Point minStart = {minLineCol, 0};
  Point maxEnd = {maxLineCol, histogramMat.rows};
  Point minEnd = {minLineCol, histogramMat.rows};
  // Line that marks the ignored values:
  line(histogramMat, Point(colsToSkip, 0), Point(colsToSkip, histogramMat.rows),
       red, 1);
  // Local high:
  line(histogramMat, maxStart, maxEnd, green, 1);
  // Local low:
  line(histogramMat, minStart, minEnd, green, 1);
  return minLineCol;
}