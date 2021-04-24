// Shawn Halayka -- shalayka@gmail.com
// June 26, 2010
//
// This code and data is in the public domain.


#include "uv_camera.h"


uv_camera::uv_camera(void)
{
	u = v = 0;
	w = 1;
	fov = 45;
	near_plane = 1.0;
	far_plane = 4.0;
	win_x = win_y = 0;

	//Transform();
}

void uv_camera::Set(const double u_rad, const double v_rad, const double w_metres, const double fov_deg, const int width_px, const int height_px, double src_near_plane, double src_far_plane)
{
	u = u_rad;
	v = v_rad;
	w = w_metres;
	near_plane = src_near_plane;
	far_plane = src_far_plane;

	static const double lock = numeric_limits<double>::epsilon() * 1000;

	if(u < -pi_half + lock)
		u = -pi_half + lock;
	else if(u > pi_half - lock)
		u = pi_half - lock;

	while(v < 0)
		v += pi_2;

	while(v > pi_2)
		v -= pi_2;

	if(w < 0)
		w = 0;
	else if(w > 10000)
		w = 10000;

	fov = fov_deg;
	win_x = width_px;
	win_y = height_px;

	Transform();
}


void uv_camera::Transform(void)
{
	Reset();
	Rotate();
	Translate();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(
		fov, 
		static_cast<GLfloat>(win_x)/static_cast<GLfloat>(win_y), 
		near_plane, far_plane);

	gluLookAt(
		eye.x, eye.y, eye.z, // Eye position.
		eye.x + look_at.x, eye.y + look_at.y, eye.z + look_at.z, // Look at position (not direction).
		up.x, up.y, up.z); // Up direction vector.
}

void uv_camera::Set(void)
{
	// Force a recalculation of the camera vectors and frustum.
	Set(u, v, w, fov, static_cast<int>(win_x), static_cast<int>(win_y), near_plane, far_plane);
}

void uv_camera::Set_Large_Screenshot(size_t num_cams, size_t cam_index_x, size_t cam_index_y)
{
	// No guarantees about the behaviour of this functionality. It wasn't tested a lot.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Image plane reference:
	// http://www.songho.ca/opengl/gl_transform.html
    const double deg_to_rad = (1.0/360.0)*2*pi;
	double aspect = win_x/win_y;
    double tangent = tan((fov/2.0)*deg_to_rad);
    double height = near_plane * tangent; // Half height of near_plane plane.
    double width = height * aspect; // Half width of near_plane plane.

	double cam_width = 2*width/num_cams;
	double cam_height = 2*height/num_cams;

	double left = -width + cam_index_x*cam_width;
	double right = -width + (cam_index_x + 1)*cam_width;
	double bottom = -height + cam_index_y*cam_height;
	double top = -height + (cam_index_y + 1)*cam_height;

	// Instead of gluPerspective...
    glFrustum(left, right, bottom, top, near_plane, far_plane);

	gluLookAt(
		eye.x, eye.y, eye.z, // Eye position.
		eye.x + look_at.x, eye.y + look_at.y, eye.z + look_at.z, // Look at position (not direction).
		up.x, up.y, up.z); // Up direction vector.
}

void uv_camera::Reset(void)
{
	eye.zero();
	look_at.zero();
	up.zero();
	right.zero();

	look_at.z = -1;
	up.y = 1;
	right.x = 1;
}

void uv_camera::Rotate(void)
{
	// Rotate about the world x axis
	look_at.rotate_x(u);
	up.rotate_x(u);
	// Right only rotates on the x axis

	// Rotate about the world y axis
	look_at.rotate_y(v);
	up.rotate_y(v);
	right.rotate_y(v);
}

void uv_camera::Translate(void)
{
	// Place the eye directly across the sphere from the look-at vector's "tip",
	// Then scale the sphere radius by w
	eye.x = -look_at.x*w;
	eye.y = -look_at.y*w;
	eye.z = -look_at.z*w;
}
