///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017, Carnegie Mellon University and University of Cambridge,
// all rights reserved.
//
// ACADEMIC OR NON-PROFIT ORGANIZATION NONCOMMERCIAL RESEARCH USE ONLY
//
// BY USING OR DOWNLOADING THE SOFTWARE, YOU ARE AGREEING TO THE TERMS OF THIS LICENSE AGREEMENT.  
// IF YOU DO NOT AGREE WITH THESE TERMS, YOU MAY NOT USE OR DOWNLOAD THE SOFTWARE.
//
// License can be found in OpenFace-license.txt

//     * Any publications arising from the use of this software, including but
//       not limited to academic journal and conference publications, technical
//       reports and manuals, must cite at least one of the following works:
//
//       OpenFace 2.0: Facial Behavior Analysis Toolkit
//       Tadas Baltrušaitis, Amir Zadeh, Yao Chong Lim, and Louis-Philippe Morency
//       in IEEE International Conference on Automatic Face and Gesture Recognition, 2018  
//
//       Convolutional experts constrained local model for facial landmark detection.
//       A. Zadeh, T. Baltrušaitis, and Louis-Philippe Morency,
//       in Computer Vision and Pattern Recognition Workshops, 2017.    
//
//       Rendering of Eyes for Eye-Shape Registration and Gaze Estimation
//       Erroll Wood, Tadas Baltrušaitis, Xucong Zhang, Yusuke Sugano, Peter Robinson, and Andreas Bulling 
//       in IEEE International. Conference on Computer Vision (ICCV),  2015 
//
//       Cross-dataset learning and person-specific normalisation for automatic Action Unit detection
//       Tadas Baltrušaitis, Marwa Mahmoud, and Peter Robinson 
//       in Facial Expression Recognition and Analysis Challenge, 
//       IEEE International Conference on Automatic Face and Gesture Recognition, 2015 
//
///////////////////////////////////////////////////////////////////////////////


// FeatureExtraction.cpp : Defines the entry point for the feature extraction console application.

// Local includes
#include <opencv2/core/core.hpp>
#include "LandmarkCoreIncludes.h"
#include <Face_utils.h>
#include <FaceAnalyser.h>

#ifndef CONFIG_DIR
#define CONFIG_DIR "~"
#endif

LandmarkDetector::FaceModelParameters det_parameters;
FaceAnalysis::FaceAnalyserParameters face_analysis_params("./bin");
LandmarkDetector::CLNF *face_model;
FaceAnalysis::FaceAnalyser *face_analyser;

std::vector<std::string> get_arguments(int argc, char **argv)
{

	std::vector<std::string> arguments;

	for (int i = 0; i < argc; ++i)
	{
		arguments.push_back(std::string(argv[i]));
	}
	return arguments;
}
int main(int argc, char **argv)
{

	//Convert arguments to more convenient vector form
	// std::vector<std::string> arguments = get_arguments(argc, argv);
    //init



    face_analysis_params.OptimizeForImages();
    face_model = new LandmarkDetector::CLNF(det_parameters.model_location);
    if (!face_model->loaded_successfully)
    {
        std::cout << "ERROR: Could not load the landmark detector" << std::endl;
    }
    face_analyser = new FaceAnalysis::FaceAnalyser(face_analysis_params);
    face_model->face_detector_MTCNN.Read(det_parameters.mtcnn_face_detector_location);
    face_model->mtcnn_face_detector_location = det_parameters.mtcnn_face_detector_location;

    //give a image and get the au result.
    // open the camera
    cv::VideoCapture capture(0);
    // cv::Mat img = cv::imread("../exe/FeatureExtraction/test2.jpg");

    //get the micro expression
    while (true)
    {
        cv::Mat frame;
        capture >> frame;
        cv::Mat grayscale_image;
        bool detection_success = LandmarkDetector::DetectLandmarksInVideo(frame, *face_model, det_parameters, grayscale_image);
        std::cout<<detection_success<<std::endl;
        if(detection_success)
        {
            face_analyser->PredictStaticAUsAndComputeFeatures(frame, face_model->detected_landmarks);
            std::vector<std::pair<std::string, double>> AUsClass = face_analyser->GetCurrentAUsClass();
            std::map<std::string, double> mapAUsClass;
            for (size_t i = 0; i < AUsClass.size(); i++)
            {
                mapAUsClass.insert(AUsClass[i]);
            }
            if (mapAUsClass["AU01"] + mapAUsClass["AU02"] + mapAUsClass["AU04"] + mapAUsClass["AU05"] + mapAUsClass["AU07"] + mapAUsClass["AU20"] + mapAUsClass["AU25"] > 5.9)
                std::cout<<"result:"<<"fear"<<std::endl;     
            else if (mapAUsClass["AU01"] + mapAUsClass["AU02"] + mapAUsClass["AU05"] + mapAUsClass["AU25"] + mapAUsClass["AU26"] > 3.9)
                std::cout<<"result:"<<"suprise"<<std::endl;      
            else if (mapAUsClass["AU01"] + mapAUsClass["AU04"] + mapAUsClass["AU07"] + mapAUsClass["AU15"] + mapAUsClass["AU17"] > 3.9)
                std::cout<<"result:"<<"sad"<<std::endl;       
            else if (mapAUsClass["AU04"] + mapAUsClass["AU05"] + mapAUsClass["AU07"] + mapAUsClass["AU15"] > 2.9)
                std::cout<<"result:"<<"angry"<<std::endl;       
            else if (mapAUsClass["AU09"] + mapAUsClass["AU10"] + mapAUsClass["AU17"] > 2.9)
                std::cout<<"result:"<<"hate"<<std::endl;       
            else if (mapAUsClass["AU06"] + mapAUsClass["AU12"] > 1.9)
                std::cout<<"result:"<<"happy"<<std::endl;     
            else
                std::cout<<"result:"<<"normal"<<std::endl;
        }
        else std::cout<<"result:"<<"normal"<<std::endl;
        cv::imshow("video",frame);
        cv::waitKey(30);

    }
    
    
    return 0;
}