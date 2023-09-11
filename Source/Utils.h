
#ifndef _SAMPLES_COMMON_SHADING_UTILS_H_
#define _SAMPLES_COMMON_SHADING_UTILS_H_

#include <string>
#include <vector>
#include <cstdint>
#include <Math/mat4.h>

std::string GetFullPath(const char* file_name);
std::string LoadShaderSource(const char* file_name);

template <typename T, size_t N>
constexpr int CountOf(T(&)[N])
{
	return static_cast<int>(N);
}

void ExpandBounds(math3d::vec3f& minPt, math3d::vec3f& maxPt, const math3d::vec3f& newPt);
int GetNumMipLevels(int imgWidth, int imgHeight);
void ExtractFrustumPlanes(const math3d::mat4f& projMat, std::vector<math3d::vec4f>& planes);
bool ViewSpaceSphereInsideFrustum(const math3d::vec3f& centerPt, float radius, const std::vector<math3d::vec4f>& planes);
bool ViewSpaceBBoxInsideFrustum(const math3d::vec3f& centerPt, const math3d::vec3f& halfVecR, const math3d::vec3f& halfVecS, const math3d::vec3f& halfVecT, const std::vector<math3d::vec4f>& planes);
bool AABBOverlapsSphere(const math3d::vec3f& minPt, const math3d::vec3f& maxPt, const math3d::vec3f& centerPt, float radius);

#endif // _SAMPLES_COMMON_SHADING_UTILS_H_
