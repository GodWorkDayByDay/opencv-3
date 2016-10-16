/*
 *  MultiCameraPnP.cpp
 *  ExploringSfMWithOpenCV
 *
 *  Created by Roy Shilkrot on 4/27/12.
 *  Copyright 2012 MIT. All rights reserved.
 *
 */

#include "MultiCameraPnP.h"
#include "BundleAdjuster.h"

using namespace std;

void MultiCameraPnP::RecoverDepthFromImages() {
	if(!features_matched) 
		OnlyMatchFeatures();
	
	std::cout << "======================================================================\n";
	std::cout << "======================== Depth Recovery Start ========================\n";
	std::cout << "======================================================================\n";
	
	cv::Matx34d P(1,0,0,0,
				  0,1,0,0,
				  0,0,1,0);
	Pmats[std::make_pair(0,0)] = P;
	
	std::vector<CloudPoint> tmp_pcloud;
	//Reconstruct from first two views
	FindCameraMatrices(K, Kinv, 
					   imgpts[0], 
					   imgpts[1], 
					   imgpts_good[0],
					   imgpts_good[1], 
					   P, 
					   Pmats[std::make_pair(0,1)],
					   matches_matrix[std::make_pair(0,1)],
					   tmp_pcloud
#ifdef __SFM__DEBUG__
					   ,imgs[0],imgs[1]
#endif
					   );
	//TODO: what if reconstrcution of first two views is bad? fallback to another pair
	
	
	//TODO: if the P1 matrix is far away from identity rotation - the solution is probably invalid...
	//so use an identity matrix?
	
	std::cout << "=========================== Baseline triangulation ===========================\n";
	std::vector<CloudPoint> pcloud;
	double reproj_error;
	{
		std::vector<cv::KeyPoint> pt_set1,pt_set2;
		
		std::vector<cv::DMatch> matches = matches_matrix[std::make_pair(0,1)];
		GetAlignedPointsFromMatch(imgpts[0],imgpts[1],matches,pt_set1,pt_set2);
		
		reproj_error = TriangulatePoints(pt_set1, 
										 pt_set2, 
										 Kinv, 
										 P, 
										 Pmats[std::make_pair(0,1)], 
										 pcloud, 
										 correspImg1Pt);
		
		std::cout << "pt_set1.size() " << pt_set1.size() << " pt_set2.size() " << pt_set2.size() << " matches.size() " << matches.size() << std::endl;
		
		for (unsigned int i=0; i<pcloud.size(); i++) {
			pcloud[i].imgpt_for_img = std::vector<int>(imgs.size(),-1);
			//matches[i] corresponds to pointcloud[i]
			pcloud[i].imgpt_for_img[0] = matches[i].queryIdx;
			pcloud[i].imgpt_for_img[1] = matches[i].trainIdx;
		}
	}
	std::cout << "triangulation reproj error " << reproj_error << std::endl;
	
	cv::Matx34d P1 = Pmats[std::make_pair(0,1)];
	
	cv::Mat_<double> distcoeff(1,4,0.0);
	cv::Mat_<double> t = (cv::Mat_<double>(1,3) << P1(0,3), P1(1,3), P1(2,3));
	cv::Mat_<double> R = (cv::Mat_<double>(3,3) << P1(0,0), P1(0,1), P1(0,2), 
						  P1(1,0), P1(1,1), P1(1,2), 
						  P1(2,0), P1(2,1), P1(2,2));
	cv::Mat_<double> rvec(1,3); Rodrigues(R, rvec);
	
	//loop images to incrementally recover more cameras 
	for (unsigned int i=2; i < imgs.size(); i++) {
		std::cout << "-------------------------- " << imgs_names[0] << " -> " << imgs_names[i] << " --------------------------\n";

		std::vector<cv::Point3f> ppcloud;
		std::vector<cv::Point2f> imgPoints;

//		for (
			 unsigned int view = 0; 
//			 view < i; view++) 
		{
			
			//Update the match between i and <view> using the Fundamental matrix
			GetFundamentalMat( imgpts[view], 
							  imgpts[i], 
							  imgpts_good[view],
							  imgpts_good[i], 
							  matches_matrix[std::make_pair(view,i)]
							  );
			
			//check for matches between i'th frame and 0'th frame (and thus the current cloud)
			std::vector<cv::DMatch> matches = matches_matrix[std::make_pair(view,i)];
			for (unsigned int pt_img_view=0; pt_img_view<matches.size(); pt_img_view++) {
				// the index of the matching point in <view>
				int matches_img0_queryIdx = matches[pt_img_view].queryIdx;
				
				for (unsigned int pcldp=0; pcldp<pcloud.size(); pcldp++) {
					// see if corresponding point was found in cloud
					if (matches_img0_queryIdx == pcloud[pcldp].imgpt_for_img[view]) {
						//point in cloud
						ppcloud.push_back(pcloud[pcldp].pt);
						//point in image i
						imgPoints.push_back(imgpts[i][matches[pt_img_view].trainIdx].pt);
						
						break;
					}
				}
			}
		}
		
		
		cv::solvePnPRansac(ppcloud, imgPoints, K, distcoeff, rvec, t, false);
		//			cv::solvePnP(ppcloud, imgPoints, K, distcoeff, rvec, t, false, CV_EPNP);
		
		Rodrigues(rvec, R);
		
		std::cout << "found t = " << t << "\nR = \n"<<R<<std::endl;
		
		P1 = cv::Matx34d(R(0,0),R(0,1),R(0,2),t(0),
						 R(1,0),R(1,1),R(1,2),t(1),
						 R(2,0),R(2,1),R(2,2),t(2));
		
		Pmats[std::make_pair(0,i)] = P1;
		
		
		std::vector<cv::KeyPoint> pt_set1,pt_set2;
		std::vector<cv::DMatch> matches = matches_matrix[std::make_pair(0,i)];
		GetAlignedPointsFromMatch(imgpts[0],imgpts[i],matches,pt_set1,pt_set2);
		
		unsigned int start_i = pcloud.size();
		
		//adding more triangulated points to general cloud
		double reproj_error = TriangulatePoints(pt_set1, pt_set2, Kinv, P, P1, pcloud, correspImg1Pt);
		std::cout << "triangulation reproj error " << reproj_error << std::endl;
		std::cout << "before triangulation: " << start_i << " after " << pcloud.size() << std::endl;
		
		int found_other_views_count = 0;
		for (unsigned int j = 0; j<matches.size(); j++) {
			pcloud[start_i + j].imgpt_for_img = std::vector<int>(imgs.size(),-1);
			//matches[i] corresponds to pointcloud[i]
			pcloud[start_i + j].imgpt_for_img[0] = matches[j].queryIdx;
			bool found_in_other_view = false;
			for (unsigned int view = 1; view < i; view++) {
				std::vector<cv::DMatch> submatches = matches_matrix[std::make_pair(0,view)];
				for (unsigned int ii = 0; ii < submatches.size(); ii++) {
					if (submatches[ii].queryIdx == matches[j].queryIdx) {
						pcloud[start_i + j].imgpt_for_img[view] = submatches[ii].trainIdx;
						found_in_other_view = true;
						break;
					}
				}
			}
			if (found_in_other_view) {
				found_other_views_count++;
			}
			pcloud[start_i + j].imgpt_for_img[i] = matches[j].trainIdx;
		}
		std::cout << matches.size() << "/" << found_other_views_count << " points were found in other views\n";
	}

	for (unsigned int i=0; i<pcloud.size(); i++) {
		pointcloud_beforeBA.push_back(pcloud[i]);
		pointCloudRGB_beforeBA.push_back(imgs_orig[0].at<cv::Vec3b>(imgpts[0][pcloud[i].imgpt_for_img[0]].pt));
	}
	
	cout << "======================== Bundle Adjustment ==========================\n";
	
	BundleAdjuster BA;
	BA.adjustBundle(pcloud,cam_matrix,imgpts,Pmats);
	
	for (unsigned int i=0; i<pcloud.size(); i++) {
		pointcloud.push_back(pcloud[i]);
		pointCloudRGB.push_back(imgs_orig[0].at<cv::Vec3b>(imgpts[0][pcloud[i].imgpt_for_img[0]].pt));
	}
	
	cout << "======================================================================\n";
	cout << "========================= Depth Recovery DONE ========================\n";
	cout << "======================================================================\n";
}