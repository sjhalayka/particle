#include "main.h"






int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	init_opengl(win_x, win_y);
	glutReshapeFunc(reshape_func);
	glutIdleFunc(idle_func);
	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);
	glutMotionFunc(motion_func);
	glutPassiveMotionFunc(passive_motion_func);

	size_t intersection_count = 0;

	for (size_t i = 0; i < num_rays; i++)
	{
		// get pseudorandom vector on unit sphere

		double u = rand() / static_cast<double>(RAND_MAX);
		double v = rand() / static_cast<double>(RAND_MAX);

		double theta = 2 * pi * u;
		double phi = acos(2 * v - 1.0);

		vector_3 pos;

		// 3D
		//pos.x = cos(theta) * sin(phi);
		//pos.y = sin(theta) * sin(phi);
		//pos.z = cos(phi);

		// 2D
		pos.x = cos(theta);
		pos.y = sin(theta);
		pos.z = 0;

		// 1D
		//pos.x = 0;
		//pos.y = 1;
		//pos.z = 0;

		ray_dirs.push_back(pos);

		float t = 0;

		if (line_sphere_intersect(
			vec3(0, 0, 0),
			vec3(pos.x, pos.y, pos.z),
			vec3(test_particle_pos.x, test_particle_pos.y, test_particle_pos.z),
			test_particle_radius,
			t))
		{
			intersection_count++;
			intersection_positions.push_back(ray_dirs[i] * t);
		}
	}

	cout << intersection_count / static_cast<double>(num_rays) << endl;







	glutMainLoop();

	glutDestroyWindow(win_id);

	return 0;
}