module;

#ifdef __APPLE__
#   include <OpenGL/gl.h>
#	include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#endif

#include <filesystem>
#include <fstream>
#include <cstdint>

module disxx.ui.utility.ImageLoader;

namespace disxx::ui::utility
{
	ImageLoader::ImageLoader(void) noexcept(false)
		: m_pHeader{new BMPHeader}
	{}

	ImageLoader::~ImageLoader(void) noexcept
	{
		if (this->m_pHeader) [[likely]]
			delete this->m_pHeader;
	}
	
	Image ImageLoader::Load(const std::filesystem::path &rPath) noexcept(false)
	{
		std::fstream f{rPath, std::fstream::in | std::fstream::binary};
		if (!f.is_open()) [[unlikely]]
   	        throw std::runtime_error("ImageLoadingError");
		f.read((char *)this->m_pHeader, sizeof(BMPHeader));

    	if (this->m_pHeader->signature[0] != 'B' || this->m_pHeader->signature[1] != 'M') [[unlikely]]
    	    throw std::runtime_error("ImageLoadingError");
    	else if (this->m_pHeader->bitsPerPixel != 24 || this->m_pHeader->compression != 0) [[unlikely]]
    	    throw std::runtime_error("ImageLoadingError");
    	
    	auto pImage
		{
			std::make_unique<std::uint8_t[]>
			(
				this->m_pHeader->width *
				abs(this->m_pHeader->height) *
				3
			)
		};
   	 
    	f.seekg(this->m_pHeader->dataOffset);
    	auto pBuff{std::make_unique<std::uint8_t[]>((this->m_pHeader->width * 3 + 3) & ~3)};
    	for (unsigned int y{0}; y < static_cast<unsigned int>(abs(this->m_pHeader->height)); ++y)
    	{
    	    f.read(reinterpret_cast<char *>(pBuff.get()), (this->m_pHeader->width * 3 + 3) & ~3);
    	    
    	    auto targetY{(this->m_pHeader->height > 0) ? abs(this->m_pHeader->height) - 1 - y : y};
    	    std::uint8_t *pTarget{pImage.get() + targetY * this->m_pHeader->width * 3};
    	    
    	    // BGR -> RGB
    	    for (int x{0}; x < this->m_pHeader->width; ++x)
    	    {
    	        pTarget[x * 3 + 0] = pBuff[x * 3 + 2]; // R
    	        pTarget[x * 3 + 1] = pBuff[x * 3 + 1]; // G
    	        pTarget[x * 3 + 2] = pBuff[x * 3 + 0]; // B
    	    }
    	}
    
    	GLuint id;
    	glGenTextures(1, &id);
    	glBindTexture(GL_TEXTURE_2D, id);
   	 
    	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    	glTexImage2D
		(
    	    GL_TEXTURE_2D,
    	    0,
    	    GL_RGB,
    	    this->m_pHeader->width,
    	    abs(this->m_pHeader->height),
    	    0,
    	    GL_RGB,
    	    GL_UNSIGNED_BYTE,
    	    pImage.get()
    	);
   
    	if (auto err{glGetError()}; err != GL_NO_ERROR) [[unlikely]]
    	    throw std::runtime_error("ImageLoadingError");
	    
		return Image{id, this->m_pHeader->width, abs(this->m_pHeader->height)};
	}
} /* disxx::ui::utils */
