#include "main.h"






int main(int argc, char** argv)
{
	//glutInit(&argc, argv);
	//init_opengl(win_x, win_y);
	//glutReshapeFunc(reshape_func);
	//glutIdleFunc(idle_func);
	//glutDisplayFunc(display_func);
	//glutKeyboardFunc(keyboard_func);
	//glutMouseFunc(mouse_func);
	//glutMotionFunc(motion_func);
	//glutPassiveMotionFunc(passive_motion_func);

	enum program_mode { sphere_mode, circle_mode, beam_mode };

	program_mode mode = sphere_mode;
	//program_mode mode = circle_mode;
	//program_mode mode = beam_mode;

//	srand(0);

	size_t intersection_count = 0;

	for (size_t i = 0; i < num_rays; i++)
	{
		double u = rand() / static_cast<double>(RAND_MAX);
		double v = rand() / static_cast<double>(RAND_MAX);

		double theta = 2 * pi * u;
		double phi = acos(2 * v - 1.0);

		vector_3 pos;

		if (mode == sphere_mode)
		{
			// get pseudorandom vector on unit sphere
			// 3D
			pos.x = cos(theta) * sin(phi);
			pos.y = sin(theta) * sin(phi);
			pos.z = cos(phi);
		}
		else if (mode == circle_mode)
		{
			// get pseudorandom vector on unit circle
			// 2D
			pos.x = cos(theta);
			pos.y = sin(theta);
			pos.z = 0;
		}
		else if (mode == beam_mode)
		{
			// 1D
			pos.x = 0;
			pos.y = 1;
			pos.z = 0;
		}

		//ray_dirs.push_back(pos);

		float t = 0;

		if (line_sphere_intersect(
			vec3(0, 0, 0),
			vec3(pos.x, pos.y, pos.z),
			vec3(test_particle_pos.x, test_particle_pos.y, test_particle_pos.z),
			test_particle_radius,
			t))
		{
			//intersection_positions.push_back(ray_dirs[i] * t);
			intersection_count++;
		}
	}

	cout << "Interaction strength: " << fixed << endl;

	double normalized_interaction_strength = intersection_count / static_cast<double>(num_rays);
		
	cout << normalized_interaction_strength << endl;

	cout << "Inverse interaction strength: " << fixed << endl;

	cout << 1.0/ normalized_interaction_strength << endl;

	//glutMainLoop();

	//glutDestroyWindow(win_id);

	return 0;
}