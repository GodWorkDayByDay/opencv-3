/*
 *  Visualization.cpp
 *  ExploringSfMWithOpenCV
 *
 *  Created by Roy Shilkrot on 12/23/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include "Visualization.h"

#include <pcl/common/common.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/io.h>
#include <pcl/io/file_io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree.h>

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;
 
void PopulatePCLPointCloud(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr& mycloud,
						   const vector<Point3d>& pointcloud, 
						   const std::vector<cv::Vec3b>& pointcloud_RGB,
						   bool write_to_file = false
						   );

#define pclp3(eigenv3f) pcl::PointXYZ(eigenv3f.x(),eigenv3f.y(),eigenv3f.z())

pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud,cloud1,cloud_no_floor,orig_cloud;
pcl::PointCloud<pcl::PointXYZ>::Ptr floorcloud;
pcl::RandomSampleConsensus<pcl::PointXYZRGB>::Ptr ransac;
Eigen::VectorXf coeffs[2];
pcl::IndicesPtr inliers;
pcl::PointCloud<pcl::Normal>::Ptr mls_normals;

void SORFilter() {
	
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZRGB>);
	
	std::cerr << "Cloud before SOR filtering: " << cloud->width * cloud->height << " data points" << std::endl;
	

	// Create the filtering object
	pcl::StatisticalOutlierRemoval<pcl::PointXYZRGB> sor;
	sor.setInputCloud (cloud);
	sor.setMeanK (50);
	sor.setStddevMulThresh (1.0);
	sor.filter (*cloud_filtered);
	
	std::cerr << "Cloud after SOR filtering: " << cloud_filtered->width * cloud_filtered->height << " data points " << std::endl;
	
	copyPointCloud(*cloud_filtered,*cloud);
}	

bool show_cloud = false;
bool sor_applied = false;
bool show_cloud_A = true;

void keyboardEventOccurred (const pcl::visualization::KeyboardEvent &event,
                            void* viewer_void)
{
	pcl::visualization::CloudViewer* viewer = static_cast<pcl::visualization::CloudViewer *> (viewer_void);
	if ((event.getKeySym () == "s" || event.getKeySym () == "S") && event.keyDown ())
	{
		cout << "s clicked" << endl;
		
		cloud->clear();
		copyPointCloud(*orig_cloud,*cloud);
		if (!sor_applied) {
			SORFilter();
			sor_applied = true;
		} else {
			sor_applied = false;
		}

		show_cloud = true;
	}
	if ((event.getKeySym () == "1") && event.keyDown ()) {
		show_cloud_A = true;
		show_cloud = true;
	}
	if ((event.getKeySym () == "2") && event.keyDown ()) {
		show_cloud_A = false;
		show_cloud = true;
	}
}


void RunVisualization(const vector<Point3d>& pointcloud,
					  const vector<Vec3b>& pointcloud_RGB,
					  const vector<Point3d>& pointcloud1,
					  const vector<Vec3b>& pointcloud1_RGB) 
{
	cloud.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
	cloud1.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
	orig_cloud.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
    
	PopulatePCLPointCloud(cloud,pointcloud,pointcloud_RGB);
	PopulatePCLPointCloud(cloud1,pointcloud1,pointcloud1_RGB);
	copyPointCloud(*cloud,*orig_cloud);
	
    pcl::visualization::CloudViewer viewer("Cloud Viewer");
    
    //blocks until the cloud is actually rendered
    viewer.showCloud(orig_cloud,"orig");
	
	viewer.registerKeyboardCallback (keyboardEventOccurred, (void*)&viewer);
	
    while (!viewer.wasStopped ())
    {
		if (show_cloud) {
			cout << "Show cloud\n";
			if(show_cloud_A)
				viewer.showCloud(cloud,"orig");
			else
				viewer.showCloud(cloud1,"orig");
			show_cloud = false;
		}
    }
}	

void PopulatePCLPointCloud(const pcl::PointCloud<pcl::PointXYZRGB>::Ptr& mycloud,
						   const vector<Point3d>& pointcloud, 
						   const std::vector<cv::Vec3b>& pointcloud_RGB,
						   bool write_to_file
						   )
	//Populate point cloud
{
	cout << "Creating point cloud...";
	double t = getTickCount();

	for (unsigned int i=0; i<pointcloud.size(); i++) {
		// get the RGB color value for the point
		Vec3b rgbv(255,255,255);
		if (pointcloud_RGB.size() >= i) {
			rgbv = pointcloud_RGB[i];
		}

		// check for erroneous coordinates (NaN, Inf, etc.)
		if (pointcloud[i].x != pointcloud[i].x || isnan(pointcloud[i].x) ||
			pointcloud[i].y != pointcloud[i].y || isnan(pointcloud[i].y) || 
			pointcloud[i].z != pointcloud[i].z || isnan(pointcloud[i].z) ||
			fabsf(pointcloud[i].x) > 10.0 || 
			fabsf(pointcloud[i].y) > 10.0 || 
			fabsf(pointcloud[i].z) > 10.0) {
			continue;
		}
		
		pcl::PointXYZRGB pclp;
		
		// 3D coordinates
		pclp.x = pointcloud[i].x;
		pclp.y = pointcloud[i].y;
		pclp.z = pointcloud[i].z;
		
		// RGB color, needs to be represented as an integer
		uint32_t rgb = ((uint32_t)rgbv[2] << 16 | (uint32_t)rgbv[1] << 8 | (uint32_t)rgbv[0]);
		pclp.rgb = *reinterpret_cast<float*>(&rgb);
		
		mycloud->push_back(pclp);
	}
	
	mycloud->width = (uint32_t) mycloud->points.size(); // number of points
	mycloud->height = 1;								// a list, one row of data
	
	t = ((double)getTickCount() - t)/getTickFrequency();
	cout << "Done. (" << t <<"s)"<< endl;
	
	// write to file
	if (write_to_file) {
		pcl::PLYWriter pw;
		pw.write("pointcloud.ply",*mycloud);
	}
}
