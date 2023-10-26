#ifndef OPENGLTEXTUREMANAGER_H
#define OPENGLTEXTUREMANAGER_H

class OpenGLTextureManager
{
	OpenGLTextureManager();
	OpenGLTextureManager(const OpenGLTextureManager &);
	~OpenGLTextureManager();
public:
	static OpenGLTextureManager &GetInstance();
	unsigned int GetTextureIndex(const std::string &_filename);
	void ReloadAllTextures();
};

#endif