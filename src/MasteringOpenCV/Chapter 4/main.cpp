/*
 *  main.cpp
 *  ExploringSfMWithOpenCV
 *
 *  Created by Roy Shilkrot on 4/27/12.
 *  Copyright 2012 MIT. All rights reserved.
 *
 */
#include "dirent.h"
#include <iostream>

#include "Distance.h"
#include "MultiCameraPnP.h"
#include "Visualization.h"

using namespace std;

std::vector<cv::Mat> images;
std::vector<std::string> images_names;

bool hasEnding (std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void open_imgs_dir(char* dir_name) {
	if (dir_name == NULL) {
		return;
	}
	std::vector<std::string> files_;
	DIR *dp;
	struct dirent *ep;     
	dp = opendir (dir_name);
	
	if (dp != NULL)
	{
		while (ep = readdir (dp)) {
			if (ep->d_name[0] != '.')
				files_.push_back(ep->d_name);
		}
		
		(void) closedir (dp);
	}
	else {
		std::cerr << ("Couldn't open the directory");
		return;
	}
	for (unsigned int i=0; i<files_.size(); i++) {
		if (files_[i][0] == '.' || !(hasEnding(files_[i],"jpg")||hasEnding(files_[i],"png"))) {
			continue;
		}
		cv::Mat m_ = cv::imread(std::string(dir_name) + "/" + files_[i]);
		images_names.push_back(files_[i]);
		images.push_back(m_);
	}
}

int main(int argc, char** argv) {
	if (argc != 2) {
		cerr << "USAGE: " << argv[0] << " <path_to_images>" << endl;
		return 0;
	}
	
	open_imgs_dir(argv[1]);
	
	cv::Ptr<MultiCameraPnP> distance = new MultiCameraPnP(images,images_names);
	distance->RecoverDepthFromImages();
	
	RunVisualization(distance->getPointCloud(), 
					 distance->getPointCloudRGB(),
					 distance->getPointCloudBeforeBA(),
					 distance->getPointCloudRGBBeforeBA()
					 );
}