#pragma once

namespace ogl
{
struct Image
{
	std::vector<u8> pixels;
	u32 width;
	u32 height;
	u32 channels;

	[[nodiscard]] static Image make(const char* inPath, const u32 inTargetChannels = 0);

private:
	static std::string getImagePath(const char* inPath);
};
}
