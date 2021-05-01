#pragma once

#include <iostream>

using namespace std;


#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
using namespace glm;

#include "uv_camera.h"

#include <GL/glut.h>

uv_camera main_camera;

GLint win_id = 0;
GLint win_x = 800, win_y = 600;
float camera_w = 12;
float camera_fov = 45;
float camera_x_transform = 0;
float camera_y_transform = 0;
double u_spacer = 0.01;
double v_spacer = 0.5 * u_spacer;
double w_spacer = 0.1;
double camera_near = 0.1;
double camera_far = 1000.0;

bool lmb_down = false;
bool mmb_down = false;
bool rmb_down = false;
int mouse_x = 0;
int mouse_y = 0;

vector_3 background_colour(1.0, 1.0, 1.0);

vector_3 test_particle_pos(0, 100, 0);
float test_particle_radius = 1;

vector<vector_3> ray_dirs;
vector<vector_3> intersection_positions;

size_t num_rays = 10000000;





// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool line_sphere_intersect(const vec3 orig, const vec3 dir, const vec3 center, const float radius, float& t)
{
	float t0, t1; // solutions for t if the ray intersects 

	vec3 L = center - orig;
	float tca = dot(L, dir);

	if (tca < 0)
		return false;

	float d2 = dot(L, L) - tca * tca;

	float radius2 = radius * radius;

	if (d2 > radius2)
		return false;

	float thc = sqrt(radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0)
	{
		t0 = t1; // if t0 is negative, let's use t1 instead 

		if (t0 < 0)
			return false; // both t0 and t1 are negative 
	}

	t = t0;

	return true;
}




void idle_func(void)
{
	glutPostRedisplay();
}

void init_opengl(const int& width, const int& height)
{
	win_x = width;
	win_y = height;

	if (win_x < 1)
		win_x = 1;

	if (win_y < 1)
		win_y = 1;

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(win_x, win_y);
	win_id = glutCreateWindow("2D v4");

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glDepthMask(GL_TRUE);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_POLYGON_SMOOTH);

	glClearColor(static_cast<float>(background_colour.x), static_cast<float>(background_colour.y), static_cast<float>(background_colour.z), 1.0f);
	glClearDepth(1.0f);

	main_camera.Set(0, 0, camera_w, camera_fov, win_x, win_y, camera_near, camera_far);
}

void reshape_func(int width, int height)
{
	win_x = width;
	win_y = height;

	if (win_x < 1)
		win_x = 1;

	if (win_y < 1)
		win_y = 1;

	glutSetWindow(win_id);
	glutReshapeWindow(win_x, win_y);
	glViewport(0, 0, win_x, win_y);

	main_camera.Set(main_camera.u, main_camera.v, main_camera.w, main_camera.fov, win_x, win_y, camera_near, camera_far);
}

void display_func(void)
{
	glPushMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glDisable(GL_LIGHTING);

	glColor3f(0, 1, 0);

	glPushMatrix();
	glTranslated(test_particle_pos.x, test_particle_pos.y, test_particle_pos.z);
	glutSolidSphere(test_particle_radius, 16, 16);
	glPopMatrix();

	glColor3f(0, 0.5, 1);

	glPushMatrix();
	glTranslated(0, 0, 0);
	glutSolidSphere(0.25, 16, 16);
	glPopMatrix();

	glBegin(GL_POINTS);

	glPointSize(4.0);

	glColor3f(1, 0, 0);

	for (size_t i = 0; i < intersection_positions.size(); i++)
		glVertex3d(intersection_positions[i].x, intersection_positions[i].y, intersection_positions[i].z);

	glEnd();


	//glEnable(GL_ALPHA);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glLineWidth(1);

	//glBegin(GL_LINES);

	//glColor4f(0, 0, 0, 0.1f);

	//for (size_t i = 0; i < ray_dirs.size(); i++)
	//{
	//	glVertex3d(0, 0, 0);
	//	glVertex3d(ray_dirs[i].x*5, ray_dirs[i].y*5, ray_dirs[i].z*5);
	//}

	//glEnd();

	//glDisable(GL_BLEND);
	//glDisable(GL_ALPHA);



	// Draw axis
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(3);

	//glBegin(GL_LINES);

	////glColor4f(0, 0, 0, 0.5);

	//glColor4f(1, 0, 0, 0.125);
	//glVertex3f(0, 0, 0);
	//glVertex3f(4, 0, 0);
	//glColor4f(0, 1, 0, 0.125);
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 4, 0);
	//glColor4f(0, 0, 1, 0.125);
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 0, 4);

	////glColor4f(0, 0, 0, 0.25);
	////glVertex3f(0, 0, 0);
	////glVertex3f(-1, 0, 0);
	////glVertex3f(0, 0, 0);
	////glVertex3f(0, -1, 0);
	////glVertex3f(0, 0, 0);
	////glVertex3f(0, 0, -1);

	//glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA);

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void keyboard_func(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'a':
		break;

	default:
		break;
	}
}

void mouse_func(int button, int state, int x, int y)
{
	if (GLUT_LEFT_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			lmb_down = true;
		else
			lmb_down = false;
	}
	else if (GLUT_MIDDLE_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			mmb_down = true;
		else
			mmb_down = false;
	}
	else if (GLUT_RIGHT_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			rmb_down = true;
		else
			rmb_down = false;
	}
}

void motion_func(int x, int y)
{
	int prev_mouse_x = mouse_x;
	int prev_mouse_y = mouse_y;

	mouse_x = x;
	mouse_y = y;

	int mouse_delta_x = mouse_x - prev_mouse_x;
	int mouse_delta_y = prev_mouse_y - mouse_y;

	if (true == lmb_down && (0 != mouse_delta_x || 0 != mouse_delta_y))
	{
		main_camera.u -= static_cast<float>(mouse_delta_y) * u_spacer;
		main_camera.v += static_cast<float>(mouse_delta_x) * v_spacer;
	}
	else if (true == rmb_down && (0 != mouse_delta_y))
	{
		main_camera.w -= static_cast<float>(mouse_delta_y) * w_spacer;
	}

	main_camera.Set(); // Calculate new camera vectors.
}

void passive_motion_func(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}





