module;

#ifdef __APPLE__
#	include <OpenGL/gl.h>
#else
#	include <GL/gl.h>
#endif

#include <filesystem>
#include <cstdint>

export module disxx.ui.utility.ImageLoader;

export import disxx.ui.utility.Image;

export namespace disxx::ui::utility
{
	class ImageLoader
	{
	  private:
		#pragma pack(push, 1)
			struct BMPHeader
			{
				char signature[2];
    			uint32_t fileSize;
    			uint16_t reserved1;
    			uint16_t reserved2;
    			uint32_t dataOffset;
    			uint32_t headerSize;
    			int32_t width; 
    			int32_t height;
    			uint16_t planes;
    			uint16_t bitsPerPixel;
    			uint32_t compression;
    			uint32_t imageSize;
    			int32_t xPixelsPerMeter;
    			int32_t yPixelsPerMeter;
    			uint32_t colorsUsed; 
    			uint32_t colorsImportant;
			} *m_pHeader;
		#pragma pack(pop)

	  public:
		explicit ImageLoader(void) noexcept(false);
		
		// Prevent some memory issues
		ImageLoader(const ImageLoader &) = delete;
		ImageLoader &operator=(const ImageLoader &) = delete;

		~ImageLoader(void) noexcept;
		Image Load(const std::filesystem::path &) noexcept(false);
	};
} /* disxx::ui::utility */
