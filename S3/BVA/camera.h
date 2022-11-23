//
// Created by arman on 21.11.2022.
//

#ifndef P1_CAMERA_H
#define P1_CAMERA_H

#ifdef USE_LIBROYALE
#include <royale.hpp>
#endif
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>

using namespace std;
using namespace cv;


#ifdef USE_LIBROYALE
class MyListener : public royale::IDepthDataListener {
        public:
    int command = 0;
    cv::VideoWriter videoWriter;
    bool skipFrame = false;

    void onNewData(const royale::DepthData *data) {
        if (skipFrame) return;
        // this callback function will be called for every new depth frame
        has_data = false;
        cv::Mat zImage(data->height, data->width, CV_32FC1, cv::Scalar(0.0f));
        int k = 0;
        for (int y = 0; y < zImage.rows; y++) {
            for (int x = 0; x < zImage.cols; x++) {
                const royale::DepthPoint curPoint = data->points.at(k);
                if (curPoint.depthConfidence > 80 && curPoint.z < 3.5f) {
                    // if the point is valid
                    zImage.at<float>(y, x) = curPoint.z;
                } //else zImage.at<float>(y, x) = NAN; // NAN gets ignored in OpenCV functions like minMaxLoc()
                k++;
            }
        }
        {
            std::lock_guard <std::mutex> lck(mtx);
            undistort(zImage, MyListener::zImage, cameraMatrix, distortionCoefficients);
        }
        has_data = true;
    }

    void setLensParameters(const royale::LensParameters &lensParameters) {
        // Construct the camera matrix
        // (fx   0    cx)
        // (0    fy   cy)
        // (0    0    1 )
        cameraMatrix = (cv::Mat1d(3, 3) << lensParameters.focalLength.first, 0.0f, lensParameters.principalPoint.first,
                0.0f, lensParameters.focalLength.second, lensParameters.principalPoint.second,
                0.0f, 0.0f, 1.0f);

        // Construct the distortion coefficients
        // k1 k2 p1 p2 k3
        distortionCoefficients = (cv::Mat1d(1, 5) << lensParameters.distortionRadial[0],
                lensParameters.distortionRadial[1],
                lensParameters.distortionTangential.first,
                lensParameters.distortionTangential.second,
                lensParameters.distortionRadial[2]);
    }

    bool hasData() const {
        return has_data;
    }

    cv::Mat zImage;
    std::mutex mtx;

private:
    bool has_data = false;

    // lens matrices used for the undistortion of the image
    cv::Mat cameraMatrix;
    cv::Mat distortionCoefficients;
};

MyListener listener;
// this represents the main camera device object
std::unique_ptr <royale::ICameraDevice> cameraDevice;

int initCamera() {
    // the camera manager will query for a connected camera
    {
        royale::CameraManager manager;

        // try to open the first connected camera
        royale::Vector <royale::String> camlist(manager.getConnectedCameraList());
        std::cout << "Detected " << camlist.size() << " camera(s)." << std::endl;

        if (!camlist.empty()) {
            cameraDevice = manager.createCamera(camlist[0]);
        } else {
            std::cerr << "No suitable camera device detected." << std::endl
                      << "Please make sure that a supported camera is plugged in, all drivers are " << std::endl
                      << "installed, and you have proper USB permission" << std::endl;
            return 1;
        }

        camlist.clear();
    }
    // the camera device is now available and CameraManager can be deallocated here

    if (cameraDevice == nullptr) {
        // no cameraDevice available
        std::cerr << "Cannot create the camera device" << std::endl;
        return 1;
    }

    // call the initialize method before working with the camera device
    royale::CameraStatus status = cameraDevice->initialize();
    if (status != royale::CameraStatus::SUCCESS) {
        std::cerr << "Cannot initialize the camera device, error string : " << getErrorString(status) << std::endl;
        return 1;
    }

    cameraDevice->setExposureMode(royale::ExposureMode::AUTOMATIC);

    // retrieve the lens parameters from Royale
    royale::LensParameters lensParameters;
    status = cameraDevice->getLensParameters(lensParameters);
    if (status != royale::CameraStatus::SUCCESS) {
        std::cerr << "Can't read out the lens parameters" << std::endl;
        return 1;
    }
    listener.setLensParameters(lensParameters);

    // register a data listener
    if (cameraDevice->registerDataListener(&listener) != royale::CameraStatus::SUCCESS) {
        std::cerr << "Error registering data listener" << std::endl;
        return 1;
    }
    return 0;
}

int startCapture() {
    if (cameraDevice->startCapture() != royale::CameraStatus::SUCCESS) {
        std::cerr << "Error starting the capturing" << std::endl;
        return 1;
    }
    return 0;
}

int stopCapture() {
    if (cameraDevice->stopCapture() != royale::CameraStatus::SUCCESS) {
        std::cerr << "Error stopping the capturing" << std::endl;
        return 1;
    }
    return 0;
}
#endif

class camera{
public:
    std::thread captureThread;
#ifdef USE_LIBROYALE
    MyListener royaleListener = {};
#endif
    int cameraId;
    bool inCapture = false;
    std::vector<function<void(cv::Mat)>> listeners = {};

    camera(int cameraId) : cameraId(cameraId) {
#ifdef USE_LIBROYALE
        if (royaleListener.initCamera() != 0)
            throw "Failed to initialize camera, commands 1 and 2 will not work.";
        royaleListener.startCapture();
        royaleListener.stopCapture();
#else

#endif
    }

    void startCapture(){
        inCapture = true;
#ifdef USE_LIBROYALE
        royaleListener.startCapture();
#else
        captureThread = std::thread([this](){
            cv::VideoCapture cap = {};
            cap.open(this->cameraId);
            if (!cap.isOpened()) {
                std::cerr << "ERROR! Unable to open camera\n";
                return;
            }
            cv::Mat frame;
            for (;;)
            {
                if(!inCapture) break;
                // wait for a new frame from camera and store it into 'frame'
                cap.read(frame);
                // check if we succeeded
                if (frame.empty()) {
                    std::cerr << "ERROR! blank frame grabbed\n";
                    break;
                }
                std::for_each(listeners.begin(), listeners.end(), [&](const auto& function){
                    function(frame);
                });
                if (cv::waitKey(5) >= 0)
                    break;
            }
        });
#endif
    }

    void stopCapture(){
        inCapture = false;
#ifdef USE_LIBROYALE
        royaleListener.stopCapture();
#endif
    }

    void addFrameListener(function<void(cv::Mat)> func) {
        listeners.push_back(func);
    }
};


#endif //P1_CAMERA_H
