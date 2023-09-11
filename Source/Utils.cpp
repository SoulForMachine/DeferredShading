
#include "Utils.h"
#include <cstring>
#include <memory>
#if defined (_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
#elif defined (__linux__)
	#include <unistd.h>
#endif
#include <Math/transform.h>


#if defined (_WIN32)
std::string GetFullPath(const char* file_name)
{
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	const char* bslash = strrchr(path, '\\');
	return bslash ? std::string(path, bslash - path + 1) + file_name : std::string(file_name);
}
#elif defined (__linux__)
std::string GetFullPath(const char* file_name)
{
	char path[256];
	readlink("/proc/self/exe", path, sizeof(path));
	const char* slash = strrchr(path, '/');
	return slash ? std::string(path, slash - path + 1) + file_name : std::string(file_name);
}
#endif

std::string LoadShaderSource(const char* fileName)
{
	std::string relPath = std::string("../Source/Shaders/") + fileName;
	std::string fn = GetFullPath(relPath.c_str());
	FILE* file = fopen(fn.c_str(), "rb");
	if(!file)
		return std::string();

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	std::unique_ptr<char[]> source(new char[size + 1]);
	fseek(file, 0, SEEK_SET);
	fread(source.get(), size, 1, file);
	source[size] = '\0';
	fclose(file);
	return std::string(source.get());
}

void ExpandBounds(math3d::vec3f& minPt, math3d::vec3f& maxPt, const math3d::vec3f& newPt)
{
	if (newPt.x < minPt.x)
		minPt.x = newPt.x;
	else if (newPt.x > maxPt.x)
		maxPt.x = newPt.x;

	if (newPt.y < minPt.y)
		minPt.y = newPt.y;
	else if (newPt.y > maxPt.y)
		maxPt.y = newPt.y;

	if (newPt.z < minPt.z)
		minPt.z = newPt.z;
	else if (newPt.z > maxPt.z)
		maxPt.z = newPt.z;
}

int GetNumMipLevels(int imgWidth, int imgHeight)
{
	if (imgWidth < 0 || imgHeight < 0)
		return 0;

	int numMipLevels = 0;
	int maxDim = std::max(imgWidth, imgHeight);
	while (maxDim > 0)
	{
		maxDim >>= 1;
		numMipLevels++;
	}

	return numMipLevels;
}

void ExtractFrustumPlanes(const math3d::mat4f& projMat, std::vector<math3d::vec4f>& planes)
{
	planes.resize(18);

	planes[0] = projMat.get_col(3) + projMat.get_col(0);	// Left
	planes[1] = projMat.get_col(3) - projMat.get_col(0);	// Right
	planes[2] = projMat.get_col(3) + projMat.get_col(1);	// Bottom
	planes[3] = projMat.get_col(3) - projMat.get_col(1);	// Top
	planes[4] = projMat.get_col(3) + projMat.get_col(2);	// Near
	planes[5] = projMat.get_col(3) - projMat.get_col(2);	// Far
	
	for (int i = 0; i < 6; ++i)
	{
		float len = planes[i].rvec3().length();
		planes[i] /= len;
	}

	int indices[12][2] = {
		{ 0, 2 }, // left, bottom
		{ 0, 3 }, // left, top
		{ 1, 2 }, // right, bottom
		{ 1, 3 }, // right, top
		{ 4, 0 }, // near, left
		{ 4, 1 }, // near, right
		{ 4, 2 }, // near, bottom
		{ 4, 3 }, // near, top
		{ 5, 0 }, // far, left
		{ 5, 1 }, // far, right
		{ 5, 2 }, // far, bottom
		{ 5, 3 }, // far, top
	};

	for (int i = 0; i < 12; ++i)
	{
		math3d::vec4f& p1 = planes[indices[i][0]];
		math3d::vec4f& p2 = planes[indices[i][1]];

		math3d::vec3f a = p1.rvec3() + p2.rvec3();
		math3d::vec3f b = math3d::cross(p1.rvec3(), p2.rvec3());
		math3d::vec3f n = math3d::normalize(a - math3d::dot(a, b) * b);

		math3d::mat3f m(p1.rvec3(), p2.rvec3(), math3d::normalize(b));
		m.transpose();
		m.inverse();
		math3d::vec3f d(-p1.w, -p2.w, 0.0f);
		math3d::vec3f pt = d * m;
		float w = - math3d::dot(n, pt);

		planes[6 + i].set(n, w);
	}
}

bool ViewSpaceSphereInsideFrustum(const math3d::vec3f& centerPt, float radius, const std::vector<math3d::vec4f>& planes)
{
	for (auto& plane : planes)
	{
		if (math3d::dot(plane, math3d::vec4f(centerPt, 1.0f)) <= -radius)
			return false;
	}

	return true;
}

bool ViewSpaceBBoxInsideFrustum(const math3d::vec3f& centerPt, const math3d::vec3f& halfVecR, const math3d::vec3f& halfVecS, const math3d::vec3f& halfVecT, const std::vector<math3d::vec4f>& planes)
{
	for (auto& plane : planes)
	{
		float radius =
			std::abs(math3d::dot(halfVecR, plane.rvec3())) +
			std::abs(math3d::dot(halfVecS, plane.rvec3())) +
			std::abs(math3d::dot(halfVecT, plane.rvec3()));

		if (math3d::dot(plane, math3d::vec4f(centerPt, 1.0f)) <= -radius)
			return false;
	}

	return true;
}

bool AABBOverlapsSphere(const math3d::vec3f& minPt, const math3d::vec3f& maxPt, const math3d::vec3f& centerPt, float radius)
{
	float dist = 0;

	for (int i = 0; i < 3; i++)
	{
		if (centerPt[i] < minPt[i])
		{
			float s = centerPt[i] - minPt[i];
			dist += s * s;
		}
		else if (centerPt[i] > maxPt[i])
		{
			float s = centerPt[i] - maxPt[i];
			dist += s * s;
		}
	}

	return dist <= radius * radius;
}
