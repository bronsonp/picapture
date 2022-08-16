#include <opencv2/opencv.hpp>
#include <sys/time.h>

int main()
{
    // Open the video camera.
    // Remove the videoflip element if rotation is not needed. https://gstreamer.freedesktop.org/documentation/videofilter/videoflip.html?gi-language=c
    std::string pipeline = "libcamerasrc ! video/x-raw, width=800, height=600, framerate=15/1 ! videoflip method=rotate-180 ! videoconvert ! appsink";
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    if(!cap.isOpened()) {
        printf("Could not open camera.\n");
        return 1;
    }

    // Create the OpenCV window
    cv::namedWindow("Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat frame;

    // Measure the frame rate - initialise variables
    int frame_id = 0;
    timeval start, end;
    gettimeofday(&start, NULL);

    for(;;) {
        if (!cap.read(frame)) {
            printf("Could not read a frame.\n");
            break;
        }

        //show frame
        cv::imshow("Camera", frame);
        cv::waitKey(1);

        // Measure the frame rate
        frame_id++;
        if (frame_id >= 30) {
            gettimeofday(&end, NULL);
            double diff = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
            printf("30 frames in %f seconds = %f FPS\n", diff, 30/diff);
            frame_id = 0;
            gettimeofday(&start, NULL);
        }
    }

    // Free the camera 
    cap.release();
    return 0;
}

