#include "Core/Image/Image.hpp"

#include "Core/Image/StbImage.hpp"

namespace ogl
{
/*****************************************************************************/
[[nodiscard]] Image Image::make(const char* inPath, const u32 inTargetChannels)
{
	Image img;

	auto resolvedPath = getImagePath(inPath);

	img.pixels.clear();

	i32 width = 0;
	i32 height = 0;
	i32 channels = 0;
	u8* pixels = stbi_load(resolvedPath.c_str(), &width, &height, &channels, static_cast<i32>(inTargetChannels));
	if (pixels)
	{
		img.width = static_cast<u32>(width);
		img.height = static_cast<u32>(height);
		img.channels = static_cast<u32>(channels);

		if (width > 0 && height > 0 && channels > 0)
		{
			img.pixels.resize(static_cast<size_t>(width * height * channels));
			::memcpy(img.pixels.data(), pixels, img.pixels.size());
		}

		stbi_image_free(pixels);
	}
	else
	{
		img.width = 0;
		img.height = 0;
		img.channels = 0;

		const char* reason = stbi_failure_reason();
		throw std::runtime_error(fmt::format("Failed to load image \"{}\". Reason {}", resolvedPath, reason));
	}

	return img;
}

/*****************************************************************************/
std::string Image::getImagePath(const char* inPath)
{
	return std::string("content/img/") + inPath;
}

}
