// Shawn Halayka -- shalayka@gmail.com
// June 26, 2010
//
// This code and data is in the public domain.


#ifndef uv_camera_h
#define uv_camera_h


#include "custom_math.h"
using custom_math::pi;
using custom_math::vector_3;
using custom_math::pi_half;
using custom_math::pi_2;

#include <iostream>
using std::cout;
using std::endl;



// UV camera
//
// latitude:     | longitude:    | radius:       |
//       *_*_    |        ___    |        ___    |
//      */   \   |       /   \   |       /   \   |
// u:  *|  x  |  |  v:  |**x**|  |  w:  |  x**|  |
//      *\___/   |       \___/   |       \___/   |
//       * *     |               |               |
// 

class uv_camera
{
public:
	// Use as read-only
	double u, v, w, fov;
	double win_x, win_y;
	vector_3 eye, look_at, up, right;
	double near_plane;
	double far_plane;

public:
	uv_camera(void);

	// Must initialize or change camera settings through these two functions
	void Set(const double u_rad, const double v_rad, const double w_metres, const double fov_deg, const int width_px, const int height_px, double src_near, double src_far);
	void Set(void);
	void Set_Large_Screenshot(size_t num_cams, size_t cam_num_x, size_t cam_num_y);
protected:
	void Transform(void);
	void Reset(void);
	void Rotate(void);
	void Translate(void);
};


#endif
