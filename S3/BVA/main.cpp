/****************************************************************************\
* Vorlage fuer das Praktikum "Bildverarbeitung" WS 2021/22
* FB 03 der Hochschule Niederrhein
* Christian Neumann, Regina Pohle-Froehlich
*
* Der Code basiert auf den C++-Beispielen der Bibliothek royale
\****************************************************************************/

//#define USE_LIBROYALE // Comment out if not needed

#include <mutex>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <thread>
#include <string>
#include <future>
#include "camera.h"

using namespace std;
using namespace cv;

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

    // Scales, calculates absolute values, and converts the result to 8-bit.
    Mat imgScaled;
    convertScaleAbs(
        zImage, // Input array
        imgScaled, // Result array
        255 / (maxZ - minZ), // Multiplicative scale factor(optional)
        -minZ * 255 / (maxZ - minZ) // Additive offset factor(optional)
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
        cerr << "Missing argument! Arguments are live, record and show-recording." << endl;
        return 1;
    }

    char* command = argv[1];
    if (strcmp(command, "live") == 0) {
        // Capture camera, show it in a window
        // and show analysed frames in another window
        cout << "Showing live camera analysis." << endl;
        cout << "Press any key to stop." << endl;
        camera.addFrameListener([](Mat frame){
            imshow("Live-zImage", frame);
            imshow("Live-zImage-Modified", modifyImage(frame));
        });
        camera.startCapture();
        waitKey(0);
        camera.stopCapture();
    } else if (strcmp(command, "record") == 0) {
        // Capture camera and show in window
        // and save it to file (via VideoWriter)
        if (argc != 5) {
            cerr << "Wrong usage! Expected ... record <video-name> <max-size-px> <max-fps>" << endl;
            return 1;
        }
        cout << "Recording camera..." << endl;
        cout << "Press any key to stop." << endl;
        char *videoName = argv[2];
        auto finalName = string(videoName).append(".avi");
        int maxSize = atoi(argv[3]);
        double maxFps = atoi(argv[4]) + 0.0;
        VideoWriter videoWriter = VideoWriter(finalName,
                                               VideoWriter::fourcc('P', 'I', 'M', '1'), maxFps,
                                               Size(maxSize, maxSize), true);
        if (!videoWriter.isOpened()) {
            cerr << "Could not open the VideoWriter for: " << finalName << endl;
            return 1;
        }
        camera.addFrameListener([&videoWriter](Mat frame){
            videoWriter << frame;
        });
        camera.startCapture();
        waitKey(0);
        camera.stopCapture();
    } else if (strcmp(command, "show-recording") == 0) {
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
    } else {
        cerr << "Invalid argument! Allowed arguments are live, record and show-recording." << endl;
        return 1;
    }

    return 0;
}
