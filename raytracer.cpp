#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <float.h>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "plane.h"
#include "material.h"
#include "rect.h"

using namespace std;

vec3 color(const ray& r, hitable *world, int depth){
	hit_record rec;
	if (world->hit(r, 0.00001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted();
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
			return emitted+attenuation * color(scattered, world, depth + 1);		//Scatter again and calculate color
		}
		else {
			return emitted;															//Return already scattered color (depth reached)
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(0.5, 1.0, 1.0) + t*vec3(0.8, 0.2, 1.0);				//Missed objects, return background gradient
	}
}

//Object list
hitable *simple_light() {
	hitable **list = new hitable*[4];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
	list[1] = new sphere(vec3(1.5, 0, -1.5), 0.3, new metal(vec3(0.5, 0.3, 0.5)));
	list[2] = new plane(vec3(-1, -0.5, -10), vec3(0, -0.5, -1), vec3(1, -0.5, -1), new lambertian(vec3(0.2, 0.4, 0.3)));
	list[3] = new sphere(vec3(3,6, -2), 2, new diffuse_light(vec3(4, 4, 4)));
	list[4] = new xy_rect(-14, -10, 3, 7, -10, new diffuse_light(vec3(4, 4, 4)));
	return new hitable_list(list, 5);

}
int main() {
	ofstream helloworld;
	helloworld.open("raytracer.ppm");		//Output file
	int nx = 200;							//Horizontal
	int ny = 100;							//Vertical
	int ns = 100;							//Antialiasing sample number
	helloworld << "P3\n" << nx << " " << ny << "\n255\n";			//PPM file header
	
	camera cam;
	hitable *world = simple_light();

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++){							//Supersample anti-aliasing
				float u = float(i + get_rand()) / float(nx);
				float v = float(j + get_rand()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}
			col /= float(ns);										//Average anti-aliasing result
			col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );	//Gamma 2 correction
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			helloworld << ir << " " << ig << " " << ib << "\n";		//PPM file rbg values
		}
	}
	return 0;
}