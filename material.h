#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "ray.h"
#include "hitable.h"

double get_rand(){
	return (double)rand() / RAND_MAX;
}

vec3 reflect(const vec3& v, const vec3& n){
	return v - 2 * dot(v, n)*n;
}

vec3 random_in_unit_sphere(){
	vec3 p;
	do{
		p = 2.0*vec3(get_rand(), get_rand(), get_rand()) - vec3(1, 1, 1);	//from 0:1 -> -1:1
	} while (p.squared_length() >= 1.0);
	return p;
}

class material  {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
	virtual vec3 emitted() const {
		return vec3(0, 0, 0);
	}
};

class lambertian : public material {
public:
	lambertian(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const  {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
	vec3 albedo;
};

class diffuse_light : public material {
public:
	diffuse_light(vec3 albedo) : albedo(albedo) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		return false;
	}
	virtual vec3 emitted() const {
		return albedo;
	}
	vec3 albedo;
};

class metal : public material {
public:
	metal(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	vec3 albedo;
};
#endif