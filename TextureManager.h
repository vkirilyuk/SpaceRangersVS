//---------------------------------------------------------------------------

#ifndef TextureManagerH
#define TextureManagerH
//---------------------------------------------------------------------------

#include <d3dx9.h>

#include <string>
#include <vector>
#include <map>

using namespace std;

class TextureManager
{
public:
	class Declaration {
	public:
		string name;
		string path;
		D3DCOLOR color;
		bool inFile;
		Declaration() {}
		Declaration(string path, bool inFile = false): name(path), path(path), color(0), inFile(inFile){}
		Declaration(string name, string path, bool inFile = false): name(name), path(path), color(0), inFile(inFile){}
		Declaration(string path, D3DCOLOR color, bool inFile = false):
		name(path), path(path), color(color), inFile(inFile){}
		Declaration(string name, string path, D3DCOLOR color, bool inFile = false):
		name(name), path(path), color(color), inFile(inFile){}
	};
	TextureManager(LPDIRECT3DDEVICE9 device, HMODULE source);
	LPDIRECT3DTEXTURE9 get(string name);
	void addDeclaration(const Declaration declaration);
	void addDeclarations(vector<Declaration> & declarations);
	void addDeclarations(Declaration declarations[], int length);
	void set(string name, LPDIRECT3DTEXTURE9 texture);
    void freeAll();
    void reloadAll();
	void free(string name);

	bool loadConfiguration(string path);
private:
	map<string, LPDIRECT3DTEXTURE9> loaded;
	map<string, Declaration> declarations;
    LPDIRECT3DDEVICE9 device;
    HMODULE source;

	LPDIRECT3DTEXTURE9 load(const Declaration & declaration);
	LPDIRECT3DTEXTURE9 load(string path, bool inFile = false);
	LPDIRECT3DTEXTURE9 load(string path, D3DCOLOR color, bool inFile = false);
};

#endif