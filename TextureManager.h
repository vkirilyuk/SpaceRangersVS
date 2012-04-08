//---------------------------------------------------------------------------

#ifndef TextureManagerH
#define TextureManagerH
//---------------------------------------------------------------------------

#include <d3dx9.h>

#include <string>
#include <map>

using namespace std;

struct TextureParam
{
	string Path;
	string Name;
	LPDIRECT3DTEXTURE9 Texture;
	bool InFile;
	int Height;
	int Width;
	D3DCOLOR Color;
	bool Ex;
};

class nTextureManager
{
	private:
		map<string,TextureParam> Map;
		LPDIRECT3DDEVICE9* OurDevice;
		HMODULE Source;

		int Create(TextureParam & texParam);

	public:
		void Init(LPDIRECT3DDEVICE9* Device, HMODULE TexSource);

		int Create(string tName, string tPath, bool inFile);
		int Create(string tName, string tPath, bool inFile, int width, int height, D3DCOLOR color);
		void Free(string tName);
		LPDIRECT3DTEXTURE9* Get(string tName);

		void ReloadAll();

		void FreeAll();
};
//---------------------------------------------------------------------------

class TextureManager
{
    private:
		map<string, LPDIRECT3DTEXTURE9> loaded;
        LPDIRECT3DDEVICE9 device;
        HMODULE source;
    public:
		TextureManager(LPDIRECT3DDEVICE9 device, HMODULE source);
        LPDIRECT3DTEXTURE9 load(string name, string path, bool inFile = false);
        LPDIRECT3DTEXTURE9 load(string name, string path, int width, int height, D3DCOLOR color, bool inFile = false);
		LPDIRECT3DTEXTURE9 get(string name);
		void set(string name, LPDIRECT3DTEXTURE9 texture);
        void freeAll();
        void reloadAll();
		void free(string name);
};

#endif