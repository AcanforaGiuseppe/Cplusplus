#include <string>

class FileSystem { };

class ImageReader { };

class ImageWriter { };

class Painter { };

class PaintFacade
{
public:
	PaintFacade(FileSystem* fs, ImageReader* ir, ImageWriter* iw, Painter* p) { }

	void OpenFile(const std::string& filePath)
	{
		// Filesystem
		// ImageReader
	}

	void WriteFile(const std::string& filePath)
	{
		// Filesystem
		// ImageWriter
	}

	void DrawRect(int x1, int y1, int x2, int y2)
	{
		// Painter
	}
};

// UI Button command
class OpenFile
{
public:
	OpenFile(FileSystem* fs, ImageReader* ir) { }

	OpenFile(PaintFacade* f) { }

	void execute()
	{
		// File to open
		//f.OpenFile(file)
	}
};

class SaveFile
{
public:
	SaveFile(FileSystem* fs, ImageWriter* iw) { }

	SaveFile(PaintFacade* f) { }

	void execute()
	{
		// File to write to
		//f.SaveFile(file)
	}

};

class DrawRect
{
public:
	DrawRect(Painter* p) { }

	DrawRect(PaintFacade* f) { }

	void execute()
	{
		// Collect mouse input
		//f.DrawRect(x1, y1, x2, y2)
	}
};