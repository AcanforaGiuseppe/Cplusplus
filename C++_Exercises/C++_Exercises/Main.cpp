#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#include "stb_image_write.h"
#include <iostream>
#include <memory>
#include <algorithm>

struct IValid
{
	virtual bool IsValid() const = 0;
};

template <typename T>
struct Color
{
	T r;
	T g;
	T b;
	T a;

	Color(unsigned char new_r, unsigned char new_g, unsigned char new_b, unsigned char new_a)
	{
		r = new_r;
		g = new_g;
		b = new_b;
		a = new_a;
	}

	Color(float new_r, float new_g, float new_b, float new_a)
	{
		r = std::clamp(new_r, 0.f, 1.f) * 255;
		g = std::clamp(new_g, 0.f, 1.f) * 255;
		b = std::clamp(new_b, 0.f, 1.f) * 255;
		a = std::clamp(new_a, 0.f, 1.f) * 255;
	}
};

template<int CHANNELS>
struct Image : IValid
{
	int width;
	int height;
	int stride_in_bytes;
	unsigned char* data;

	Image() = delete;

	void put_pixel(const int x, const int y, const Color<unsigned char>& color)
	{
		int pixel_index = y * stride_in_bytes + x * CHANNELS;
		data[pixel_index++] = color.r;
		data[pixel_index++] = color.g;
		data[pixel_index++] = color.b;
		data[pixel_index] = color.a;
	};

	Image(const int in_width, const int in_height/*, bool& error*/) : width(in_width), height(in_height)
	{
		stride_in_bytes = CHANNELS * width;
		data = nullptr;
	}

	~Image()
	{
		if (data != nullptr)
			free(data);
	}

	virtual bool IsValid() const
	{
		return data != nullptr;
	}
};

template<int CHANNELS>
struct ImageFactory
{
	static std::shared_ptr<Image<CHANNELS>> Create(const int width, const int height)
	{
		auto image = std::make_shared<Image<CHANNELS>>(width, height);

		if (image == nullptr)
			return nullptr;

		image->data = reinterpret_cast<unsigned char*>(malloc((width * height) * CHANNELS));

		if (image->data == nullptr)
			return nullptr;

		return image;
	}
};

int main(int argc, char** argv)
{
	const int width = 64;
	const int height = 64;

	auto image = ImageFactory<4>::Create(width, height);

	if (image == nullptr)
		return -1;

	int result = stbi_write_png("red_square.png", width, height, 4, reinterpret_cast<void*>(image->data), width * image->stride_in_bytes);
	std::cout << result << std::endl;

	return 0;
}