#include <glm/ext.hpp>
#include <algorithm>
#include <chrono>
#include <random>
#include "Texture.hpp"
#include "A5.hpp"
#include <pthread.h>

#define ANTIALIAS 1
#define ADAPTIVETHRESHOLD 0.17
#define MAXRECURSE 20
#define EPSILON 0.0001
#define GLOSSENABLE 0
#define GLOSSYNUM 6
#define SOFTSHADOW 16 // 8by8 sample
#define NUMTHREADS 16
#define MOTIONBLUR 20
#define NUMDOF 10
#define SQRSIDE 0.1
#define FOCALDIST 10.0

using namespace std::chrono;
// Store image in a global variable so all threads have access to it
Image *img;

//Structure to store info
struct ThreadData {
	int startingRow;
	int numRows;
	SceneNode *root;
	uint width;
	uint height;
	glm::vec3 eye;
	glm::mat4 transformDeviceToWorld;
	glm::vec3 ambient;
	std::list<Light *>lights;
	ThreadData():ambient(glm::vec3()), lights(){};
	double **pixelDiff;
};

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
) {

  // Fill in raytracing code here...

	std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

	// Track when program started
	const auto begin = high_resolution_clock::now(); 
	
	// Calculate coordinates in thems of world coordinate system
	size_t ny = image.height();
	size_t nx = image.width();
	float d = FOCALDIST;
	size_t h = 2 * d * glm::tan(glm::radians(fovy/2));

	glm::mat4 t1 = T1(nx,ny,d);
	glm::mat4 s2 = S2(h,ny);
	glm::mat4 r3 = R3(up,eye,view);
	glm::mat4 t4({{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {eye.x, eye.y, eye.z, 1}});
	
	glm::mat4 transformDeviceToWorld = t4 * r3 * s2 * t1;


	// Store info for multithreading
	img = &image;
	pthread_t threads[NUMTHREADS];
	struct ThreadData td[NUMTHREADS];

	int rowsToRender = image.height();
	int startingRow = 0;
	// Start each thread, while changing which rows to render
	for(uint i = 0; i <NUMTHREADS; i++){
		td[i].root = root;
		td[i].height = image.height();
		td[i].width = image.width();
		td[i].eye = eye;
		td[i].transformDeviceToWorld = transformDeviceToWorld;
		td[i].ambient = ambient;
		td[i].lights = lights;
		int numRows = (i == NUMTHREADS-1) ? rowsToRender : image.height() / NUMTHREADS;
		td[i].startingRow = startingRow;
		td[i].numRows = numRows; 
		pthread_create(&threads[i], NULL, renderRows, &td[i]);
		startingRow += numRows;
		rowsToRender -= numRows;
	}

	// Wait for all threads to finish
	for (unsigned int i = 0; i < NUMTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	std::cout << "Done" << std::endl;

	// Start adaptive anti aliasing
	// Simple algorithm to detect change in colour around a pixel
	double **pixelDiff = new double *[ny];
	for(int i=0; i<ny;i++)
		pixelDiff[i] = new double[nx];

	// For each pixel check the square of pixels around
	for (uint y = 0; y < ny; ++y) {
		for (uint x = 0; x < nx; ++x) {
			pixelDiff[y][x] = 0.0;
			double totalNum = 0.0;
			for(int i=-2;i<3;i++) {
				for(int j=-2;j<3;j++) {
					// we want the following pixels checked
					// XXX
					// XOX
					// XXX
					if(i==0 && j==0)
						continue;
					// only check valid indices
					if(i+x >=0 && i+x < nx && j+y >= 0 && j+y < ny){
						totalNum++;
						pixelDiff[y][x] += image(x,y,0) + image(x,y,1) + image(x,y,2) - image(x+i,y+j,2) - image(x+i,y+j,1) - image(x+i,y+j,0);
					}
				}
			}
			pixelDiff[y][x] /= totalNum;
		}
	}

	std::cout << "Start adaptive ANTIALIAS" << std::endl;
	// reset index trackers
	rowsToRender = image.height();
	startingRow = 0;

	// start each thread for adaptive anti-aliasing
	for(uint i = 0; i <NUMTHREADS; i++){
		int numRows = (i == NUMTHREADS-1) ? rowsToRender : image.height() / NUMTHREADS;
		td[i].startingRow = startingRow;
		td[i].numRows = numRows; 
		td[i].pixelDiff = pixelDiff;
		pthread_create(&threads[i], NULL, adaptiveAntiAlias, &td[i]);
		startingRow += numRows;
		rowsToRender -= numRows;
	}

	// Wait for thread to finish
	for (unsigned int i = 0; i < NUMTHREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	// free memory
	for(int i=0; i<ny;i++)
		delete pixelDiff[i];
	delete pixelDiff;

	// Check time taken
  	auto time = high_resolution_clock::now() - begin;
 	std::cout << "Elapsed time: " << duration<double, std::milli>(time).count() << std::endl;
}


// renderRows is what each thread does
// Will create the rays to then check if there's an intersection
void *renderRows(void * args) {
	// Set the parameters
	ThreadData *td = (ThreadData *) args;
	SceneNode * root = td->root;
	glm::mat4 transformDeviceToWorld = td->transformDeviceToWorld;
	size_t nx = td->width; 
	size_t ny = td->height;
	int numRows = td->numRows;
	const glm::vec3 & eye = td->eye;
	const glm::vec3 & ambient = td->ambient;
	const std::list<Light *> & lights = td->lights;
	int startingRow = td->startingRow;

	// Iterate through each pixel of the image plane
	for (uint y = startingRow; y < startingRow + numRows; ++y) {
		// std::cout << (double) y/ny << std::endl;
		for (uint x = 0; x < nx; ++x) {
			// create ray and world coordinates
			glm::vec4 worldCoord = calcPinWorld(x,y,transformDeviceToWorld);
			Ray r = Ray(glm::vec4(eye, 1), worldCoord-glm::vec4(eye,1));

			// NORMAL STUFF
			ColourHit ch =  rayColour(r, lights, root, ambient, 0);
			if(ch.hit){
				glm::vec3 colour = ch.colour;
				(*img)(x,y,0) = colour.r;
				(*img)(x,y,1) = colour.g;
				(*img)(x,y,2) = colour.b;
			} else{
				// cyanish background
				(*img)(x,y,0) = 0.51;
				(*img)(x,y,1) = 0.746;
				(*img)(x,y,2) = 0.785;
				// float height = (float)y/ny;

				// (*img)(x,y,0) = std::max(height-0.1,0.0);
				// (*img)(x,y,1) = height*height;
				// (*img)(x,y,2) = std::max(0.176*(height)+0.25*(height)*(height),0.0);
			}
		}
	}
}

// Function for each thread to do anti-aliasing
void *adaptiveAntiAlias(void *args){
	// Unpack data
	ThreadData *td = (ThreadData *) args;
	SceneNode * root = td->root;
	glm::mat4 transformDeviceToWorld = td->transformDeviceToWorld;
	const glm::vec3 & eye = td->eye;
	const glm::vec3 & ambient = td->ambient;
	const std::list<Light *> & lights = td->lights;
	size_t nx = td->width; 
	size_t ny = td->height;
	int startingRow = td->startingRow;
	int numRows = td->numRows;
	double **pixelDiff = td->pixelDiff; 

	// iterate through each pixel
	for (uint y = startingRow; y < startingRow + numRows; ++y) {
		for (uint x = 0; x < nx; ++x) {
			// Check if the pixel is a critical area
			if(pixelDiff[y][x] > ADAPTIVETHRESHOLD){
				glm::vec3 finalColour(0,0,0);
				finalColour.r = (*img)(x,y,0);
				finalColour.g = (*img)(x,y,1);
				finalColour.b = (*img)(x,y,2);
				glm::vec4 pWorldCoord = calcPinWorld(x,y,transformDeviceToWorld);
				// Get 3x3 grid around the pixel in the world coordinate system
				for(int i=-1; i<2; i++){
					for(int j=-1; j<2; j++){
						if(i==0 && j==0)
							continue;

						glm::vec4 worldCoord = calcPinWorld(x+i,y+j,transformDeviceToWorld);
						// Get the pixels surrounding the main pixel in the world by taking inbetween the pixel
						worldCoord = (worldCoord + pWorldCoord) * 0.5;
						Ray r = Ray(glm::vec4(eye, 1), worldCoord-glm::vec4(eye,1));

						ColourHit ch =  rayColour(r, lights, root, ambient, 0);
						if(ch.hit){
							finalColour += ch.colour;
						} else{
							// Simple cyan background
							finalColour.r += 0.51;
							finalColour.g += 0.746;
							finalColour.b += 0.785;
							// float height = (float)(y)/ny;
							// glm::vec3 background(std::max(height-0.2,0.0),height*height,std::max(0.176*(height)+0.25*(height)*(height),0.0));
							// finalColour += background;
						}
					}
				}
				// Average the colour
				finalColour = finalColour/9.0f;

				(*img)(x,y,0) = finalColour.r;
				(*img)(x,y,1) = finalColour.g;
				(*img)(x,y,2) = finalColour.b;
			}
		}
	}

}

// device to world transformation matrix, taken from notes in class
glm::mat4 R3(glm::vec3 up, glm::vec3 eye, glm::vec3 view){
	glm::vec3 w = view;
	w = glm::normalize(w);
	
	glm::vec3 u = glm::cross(up, w);
	u = glm::normalize(u);
	glm::vec3 v = glm::cross(u, w);

	glm::mat4 r3({{u.x, u.y, u.z, 0}, {v.x, v.y, v.z, 0}, {w.x, w.y, w.z, 0}, {0, 0, 0, 1}});

	return r3;
}

// device to world transformation matrix, taken from notes in class
glm::mat4 T1(double nx, double ny, double d){
	return glm::translate(glm::mat4(), glm::vec3(-nx/2, -ny/2, d));
}

// device to world transformation matrix, taken from notes in class
glm::mat4 S2(double h, double ny){
	return glm::scale(glm::mat4(), glm::vec3(-h/ny, h/ny, 1));
}

// device to world transformation matrix, taken from notes in class
glm::vec4 calcPinWorld(double x, double y, glm::mat4 trans){
	glm::vec4 point = glm::vec4(x,y,0,1);
	return trans * point;
}

// Magic happens here
// Function to test if there's a hit for the ray, and determine the colour
ColourHit rayColour(const Ray &r, const std::list<Light *> & lights, SceneNode * root, const glm::vec3 & ambient, int maxHits){
	glm::vec3 colour(0,0,0);

	// check if we hit the limit
	if(maxHits > MAXRECURSE)
		return {false,colour};

	// intersection check
	Intersection firstIntersect = root->intersect(r);

	if(firstIntersect.hit){
		// if there's a hit start to calculate colour from lights and etc
		PhongMaterial * mat = firstIntersect.mat;
		// get ambient
		colour += mat->getDiffuse() * ambient; 
		// calculate light sources
		colour += directLight(lights, firstIntersect, root, ambient, maxHits);
		return {true,colour};	
	}
	// show background instead	
	return {false, colour};
}

// Function to determine colour from lighting sources
glm::vec3 colourLighting(const Intersection &intersect, Light * l){
	glm::vec4 intersectToLight = glm::vec4(l->position,1) - intersect.intersectPoint;

	double distToLight = glm::length(intersectToLight); 
	intersectToLight = glm::normalize(intersectToLight);

	double diffBrightness = std::max(0.0f, glm::dot(intersect.norm,intersectToLight)); 
	glm::vec3 kd = intersect.mat->getDiffuse();

	// If the node hit has a texture, use that for kd instead of the material
	Texture *t = intersect.getTexture();
	if(t){
		kd = t->colour(intersect.textureMapCoords.x,intersect.textureMapCoords.y);
	}

	glm::vec3 diffuse = diffBrightness* kd * l->colour;

	glm::vec4 reflected = glm::normalize(-intersectToLight + 2*(glm::dot(intersectToLight,intersect.norm))*intersect.norm);
	glm::vec4 surfaceToEye = glm::normalize(intersect.incident.origin - intersect.intersectPoint);
	double specBrightness = 0.0;
	// can't have specular without diffuse
	if(diffBrightness > 0.0)
		specBrightness = pow(std::max(0.0f, glm::dot(surfaceToEye,reflected)), intersect.mat->getShiny());
	glm::vec3 specular = specBrightness * intersect.mat->getSpec() * l->colour;

	double totalFalloff = 1.0 / (l->falloff[0] + l->falloff[1]*distToLight + l->falloff[2]*(distToLight*distToLight));

	return (diffuse + specular)/totalFalloff;
	
}

// Calculate reflection, refraction and lighting colours
glm::vec3 directLight(const std::list<Light *> &lights, const Intersection &intersect, SceneNode *root, const glm::vec3 & ambient, int maxHits){
	glm::vec3 colour(0,0,0);
	// If there's a refraction index
	if(intersect.mat->getRefractIdx() > 0){
		// Get reflectance from a simple fresnel model, taken from ggRefract handout
		double reflectance = simpleFresnel(intersect.norm, glm::normalize(intersect.incident.dir), intersect.fromMat->getRefractIdx(), intersect.mat->getRefractIdx());
		
		// Here as an easy way to get a mirror like object
		if(intersect.mat->getReflectance() != 0.0)
			reflectance = intersect.mat->getReflectance();

		// Reflection
		glm::vec3 Ri = glm::normalize(glm::vec3(intersect.incident.dir));
		glm::vec3 N = glm::normalize(glm::vec3(intersect.norm));
		glm::vec3 Rr = Ri - 2.0 * N * glm::dot(Ri, N);

		// check if the material is suppose to be glossy reflected/refracted
		if(maxHits < 5 && intersect.mat->getGloss() > 0){
			glm::vec3 reflectionColour(0,0,0);
			// get perturbed rays and do reflection
			std::vector<glm::vec3> randomReflected = randomRays(Rr, glm::vec3(intersect.norm), intersect.mat->getGloss());
			for(auto ray : randomReflected){
				ray = glm::normalize(ray);
				Ray reflectedRay(intersect.intersectPoint + (float)EPSILON * glm::vec4(ray,0), glm::vec4(ray,0));
				ColourHit ch =  rayColour(reflectedRay, lights, root, ambient, maxHits+1);
				reflectionColour += ch.colour * intersect.mat->getSpec() * reflectance;//intersect.mat->getReflect() * ch.colour;
			}
			// take average colour gotten
			colour += reflectionColour / (double) GLOSSYNUM;//Maybe need something else for glossy coefficients

			// get perturbed rays and do refraction with each one
			glm::vec3 refractedColour(0,0,0);
			Ray refracted = findRefraction(intersect.incident, intersect);
			std::vector<glm::vec3> randomRefracted = randomRays(glm::vec3(refracted.dir), -glm::vec3(intersect.norm), intersect.mat->getGloss());
			for(auto ray : randomRefracted){
				ray = glm::normalize(ray);
				refracted.mat = intersect.mat;

				Ray refractedRay(intersect.intersectPoint + (float)EPSILON * glm::vec4(ray,0), glm::vec4(ray,0));
				ColourHit ch =  rayColour(refractedRay, lights, root, ambient, maxHits+1);
				refractedColour += ch.colour ;
			}
			colour += refractedColour* (1-reflectance) / (double) GLOSSYNUM;//Maybe need something else for glossy coefficients

		} else {
			// Normal reflection and refraction

			// Reflection
			glm::vec4 RrDir = glm::vec4(glm::normalize(Rr), 0);

			Ray reflection(intersect.intersectPoint + (float)EPSILON * RrDir, RrDir);

			ColourHit ch =  rayColour(reflection, lights, root, ambient, maxHits+1);
			colour += ch.colour * intersect.mat->getSpec() * reflectance;

			// Refraction

			Ray refracted = findRefraction(intersect.incident, intersect);
			refracted.mat = intersect.mat;
			ColourHit refractCH = rayColour(refracted, lights, root, ambient, maxHits+1);
			if(refractCH.hit) {
				colour +=  refractCH.colour * (1-reflectance);
			}
		}
	}

	// Normal diffuse lighting
	for (std::list<Light *>::const_iterator it=lights.begin(); it != lights.end(); ++it) {
		Light * l = *it;
		int loops = l->isAreaLight() ? SOFTSHADOW : 1;
		for(int i = 0; i<loops; i++){
			glm::vec3 lightPos = l->isAreaLight()? l->randomPoint(i) : l->position;	
			glm::vec4 shadowRayDir = glm::vec4(lightPos,1) - intersect.intersectPoint;
			// Use offset to test shadow ray
			glm::vec4 norm = intersect.intersectPoint + EPSILON * intersect.norm;
			Ray shadow(norm, shadowRayDir);
			Intersection shadowIntersect = root->intersect(shadow);
 			
 			// Do a dot product here to confirm that the hit location is INBETWEEN light source and first intersection point
			if (shadowIntersect.hit && glm::dot(glm::vec4(lightPos,1) - shadowIntersect.intersectPoint, shadowRayDir) > 0){
				continue;
			}
			colour += colourLighting(intersect, l)/loops;
		}
	}
	return colour;
}

Intersection hit(const Ray &r, SceneNode *root){
	Intersection res = root->intersect(r);
	return res;
}

// function to calculate refraction ray, using snell's law
Ray findRefraction(const Ray &incident, const Intersection &intersect){
	// Taken from ggRefract handout
	Ray normalized(incident.origin, glm::normalize(incident.dir));
	double n1 = intersect.fromMat->getRefractIdx();
	double n2 = intersect.mat->getRefractIdx();
	double n = n1/n2;

	double cosTheta = -glm::dot(intersect.norm, normalized.dir);

	double sinTheta = 1 - n*n*(1- cosTheta*cosTheta);

	if(sinTheta < 0)
		//total internal refraction
		return Ray(glm::vec4({0,0,0,1}),glm::vec4(0,0,0,0));
	glm::vec4 refractVec = n * normalized.dir + intersect.norm * (n * cosTheta - sqrt(sinTheta));
	glm::vec4 refractDir = glm::normalize(refractVec);

	Ray refract(intersect.intersectPoint + EPSILON * refractDir, refractDir);
	refract.mat = intersect.mat;
	return refract;
}

// function to generate perturbed rays of r
std::vector<glm::vec3> randomRays(glm::vec3 r, glm::vec3 norm, double m_gloss){
	std::vector<glm::vec3> rays;
	r = glm::normalize(r);
	rays.push_back(r);

	while(rays.size() < GLOSSYNUM) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator (seed);
		std::uniform_real_distribution<double> distribution(0.0,1.0);
		glm::vec3 randVec = glm::normalize(glm::vec3(distribution(generator),distribution(generator),distribution(generator)));

		glm::vec3 w = glm::normalize(r);
		// get square base
		// glm::vec3 u = glm::normalize(glm::cross(w,randVec));

		glm::vec3 u = glm::normalize(glm::cross(w,glm::vec3(0,1,0)));
		if(glm::length(u)<0.1){
			u = glm::normalize(glm::cross(w,glm::vec3(0,0,1)));
		}
		glm::vec3 v = glm::normalize(glm::cross(w,u));
		double phi = 2 * glm::pi<double>() * distribution(generator);
		double cosTheta = glm::pow(distribution(generator), 1.0/(m_gloss+1.0));
		double sinTheta = glm::sqrt(1 - cosTheta*cosTheta);
		double cosPhi = glm::cos(phi);
		double sinPhi = glm::sin(phi);

		glm::vec3 newDir = w *cosTheta + u*cosPhi * sinTheta + v * sinPhi * sinTheta;

		if(glm::dot(newDir,norm) > 0 ){
			rays.push_back(newDir);
		}

	}
	return rays;
}

// equations from ggRefract sheet
double simpleFresnel(const glm::vec4 norm, const glm::vec4 incident, double fromRefractIdx, double toRefractIdx) {
	double n = fromRefractIdx / toRefractIdx;

	// check for total internal refraction
	double cosIncident = -glm::dot(incident, norm);
	// Snell's law
	double sinT2 = n * n * (1 - cosIncident * cosIncident);
	if(sinT2 > 1){
		return 1.0;
	}

	double cosT = glm::sqrt(1.0 - sinT2);

	double rs = (fromRefractIdx * cosIncident - toRefractIdx * cosT) / (fromRefractIdx * cosIncident + toRefractIdx * cosT);
	double rp = (fromRefractIdx * cosT - toRefractIdx * cosIncident) / (fromRefractIdx * cosT + toRefractIdx * cosIncident);

	rs *= rs;
	rp *= rp;
	return (rs+rp)/2.0;
}