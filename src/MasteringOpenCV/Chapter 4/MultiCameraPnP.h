/*
 *  MultiCameraPnP.h
 *  ExploringSfMWithOpenCV
 *
 *  Created by Roy Shilkrot on 4/27/12.
 *  Copyright 2012 MIT. All rights reserved.
 *
 */
#pragma once

#include "MultiCameraDistance.h"

#include "Visualization.h"

#include "Common.h"

class MultiCameraPnP : public MultiCameraDistance {
	std::vector<CloudPoint> pointcloud_beforeBA;
	std::vector<cv::Vec3b> pointCloudRGB_beforeBA;

public:
	MultiCameraPnP(const std::vector<cv::Mat>& imgs_, const std::vector<std::string>& imgs_names_):
	MultiCameraDistance(imgs_,imgs_names_) 
	{
	}
	
	virtual void RecoverDepthFromImages();
	
	std::vector<cv::Point3d> getPointCloudBeforeBA() { return CloudPointsToPoints(pointcloud_beforeBA); }
	const std::vector<cv::Vec3b>& getPointCloudRGBBeforeBA() { return pointCloudRGB_beforeBA; }
	
};