#include "TextureManager.h"
#include <json/json.h>
#include <fstream>
#include <sstream>

TextureManager::TextureManager(LPDIRECT3DDEVICE9 device, HMODULE source): device(device), source(source) {
}

LPDIRECT3DTEXTURE9 TextureManager::load(const Declaration & d) {
	return load(d.path, d.color, d.inFile);
}

LPDIRECT3DTEXTURE9 TextureManager::load(string path, D3DCOLOR color, bool inFile) {
	LPDIRECT3DTEXTURE9 texture;
	HRESULT result;
	if (inFile)
		result = D3DXCreateTextureFromFileExA(device, path.c_str(), 0, 0, 0, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED,
									D3DX_DEFAULT, D3DX_DEFAULT, color, NULL, NULL, &texture);
	else
		result = D3DXCreateTextureFromResourceEx(device, source, path.c_str(), 0, 0, 0, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED,
									D3DX_DEFAULT, D3DX_DEFAULT, color, NULL, NULL, &texture);
	if (result != D3D_OK)
		return NULL;
	return texture;
}

LPDIRECT3DTEXTURE9 TextureManager::get(string name) {
	map<string, LPDIRECT3DTEXTURE9>::iterator i = loaded.find(name);
	if (i != loaded.end())
		return i->second;
	map<string, Declaration>::iterator j = declarations.find(name);
	if (j == declarations.end()) {
		return NULL;
	}
	LPDIRECT3DTEXTURE9 texture = load(j->second);
	set(name, texture);
	return texture;
}

void TextureManager::set(string name, LPDIRECT3DTEXTURE9 texture) {
	loaded[name] = texture;
}

void TextureManager::addDeclaration(Declaration declaration) {
	declarations[declaration.name] = declaration;
}

void TextureManager::addDeclarations(vector<Declaration> & ds) {
	for (vector<Declaration>::iterator i = ds.begin(); i != ds.end(); i++)
		declarations[i->name] = *i;
}

void TextureManager::addDeclarations(Declaration ds[], int length) {
	for (int i = 0; i < length; i++) {
		Declaration & d = ds[i];
		declarations[d.name] = d;
	}
}

void TextureManager::reloadAll() {
}

void TextureManager::freeAll() {
}

bool TextureManager::loadConfiguration(string path) {
	Json::Value root;
	Json::Reader reader;

	bool parsingSuccessful = reader.parse(ifstream(path), root);
	if (!parsingSuccessful) {
		std::cout << "Failed to parse configuration\n"
			<< reader.getFormatedErrorMessages();
		return false;
	}
	Json::Value colors = root["colors"];

	Json::Value textures = root["textures"];
	for (Json::Value::iterator i = textures.begin(); i != textures.end(); i++) {
		Json::Value texture = *i;
		Declaration d( texture["path"].asString() );
		if (! texture["color"].isNull() ) {
			cout << "Found texture with color" << endl;
			string color = texture["color"].asString();
			string color_hex = colors[color].asString();
			stringstream st;
			st << std::hex << color_hex;
			st >> d.color;
			cout << color << " " << color_hex << " " << d.color << endl;
		}
		if (! texture["name"].isNull() ) {
			d.name = texture["name"].asString();
		}
		addDeclaration(d);
	}

	return true;
}