/*
 *  IDistance.h
 *  ExploringSfMWithOpenCV
 *
 *  Created by Roy Shilkrot on 4/15/12.
 *  Copyright 2012 MIT. All rights reserved.
 *
 */

#pragma once

class IDistance {
public:
	virtual void OnlyMatchFeatures() = 0;
	virtual void RecoverDepthFromImages() = 0;
	virtual std::vector<cv::Point3d> getPointCloud() = 0;
	virtual const std::vector<cv::Vec3b>& getPointCloudRGB() = 0;
};