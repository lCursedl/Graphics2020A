#include "CModel.h"

#ifdef OPENGL
unsigned int TextureFromFile(const char * path, const std::string & directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		//std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}
#elif D3D11
//bool CModel::Load(HWND hwnd, ID3D11Device * dev, ID3D11DeviceContext * devcon, std::string filename) {
//	Assimp::Importer importer;
//
//	const aiScene* pScene = importer.ReadFile(filename,
//		aiProcess_Triangulate |
//		aiProcess_ConvertToLeftHanded);
//
//	if (pScene == NULL)
//		return false;
//
//	this->directory = filename.substr(0, filename.find_last_of("/\\"));
//
//	this->dev = dev;
//	this->devcon = devcon;
//	this->hwnd = hwnd;
//
//	processNode(pScene->mRootNode, pScene);
//
//	return true;
//}
//
//void CModel::Draw(ID3D11DeviceContext * devcon) {
//	for (int i = 0; i < meshes.size(); ++i )
//	{
//		meshes[i].Draw(devcon);
//	}
//}
//
//std::string textype;
//
//CMesh CModel::processMesh(aiMesh * mesh, const aiScene * scene) {
//	// Data to fill
//	std::vector<Vertex> vertices;
//	std::vector<UINT> indices;
//	std::vector<Texture> textures;
//
//	if (mesh->mMaterialIndex >= 0) {
//		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
//
//		if (textype.empty()) {
//            textype = determineTextureType(scene, mat);
//        }
//	}
//
//	// Walk through each of the CMesh's vertices
//	for (UINT i = 0; i < mesh->mNumVertices; i++) {
//		Vertex Vertex;
//
//		Vertex.X = mesh->mVertices[i].x;
//		Vertex.Y = mesh->mVertices[i].y;
//		Vertex.Z = mesh->mVertices[i].z;
//
//		if (mesh->mTextureCoords[0]) {
//			Vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
//			Vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
//		}
//
//		vertices.push_back(Vertex);
//	}
//
//	for (UINT i = 0; i < mesh->mNumFaces; i++) {
//		aiFace face = mesh->mFaces[i];
//
//		for (UINT j = 0; j < face.mNumIndices; j++)
//			indices.push_back(face.mIndices[j]);
//	}
//
//	if (mesh->mMaterialIndex >= 0) {
//		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//
//		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
//		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//	}
//
//	return CMesh(dev, vertices, indices, textures);
//}
//
//std::vector<Texture> CModel::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName, const aiScene * scene) {
//	std::vector<Texture> textures;
//	for (UINT i = 0; i < mat->GetTextureCount(type); i++) {
//		aiString str;
//		mat->GetTexture(type, i, &str);
//		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//		bool skip = false;
//		for (UINT j = 0; j < textures_loaded.size(); j++) {
//			if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0) {
//				textures.push_back(textures_loaded[j]);
//				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
//				break;
//			}
//		}
//		if (!skip) {   // If texture hasn't been loaded already, load it
//			HRESULT hr;
//			Texture texture;
//			if (textype == "embedded compressed texture") {
//				int textureindex = getTextureIndex(&str);
//				texture.texture = getTextureFromModel(scene, textureindex);
//			} else {
//				std::string filename = std::string(str.C_Str());
//				filename = directory + '/' + filename;
//				std::wstring filenamews = std::wstring(filename.begin(), filename.end());
//				hr = CreateWICTextureFromFile(dev, devcon, filenamews.c_str(), nullptr, &texture.texture);
//				if (FAILED(hr))
//					MessageBox(hwnd, L"Texture couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
//			}
//			texture.type = typeName;
//			texture.path = str.C_Str();
//			textures.push_back(texture);
//			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//		}
//	}
//	return textures;
//}
//
//void CModel::Close() {
//	for (auto& t : textures_loaded)
//		t.Release();
//
//	for (int i = 0; i < meshes.size(); i++) {
//		meshes[i].Close();
//	}
//}
//
//void CModel::processNode(aiNode * node, const aiScene * scene) {
//	for (UINT i = 0; i < node->mNumMeshes; i++) {
//		aiMesh* CMesh = scene->mMeshes[node->mMeshes[i]];
//		meshes.push_back(this->processMesh(CMesh, scene));
//	}
//
//	for (UINT i = 0; i < node->mNumChildren; i++) {
//		this->processNode(node->mChildren[i], scene);
//	}
//}
//
//std::string CModel::determineTextureType(const aiScene * scene, aiMaterial * mat) {
//	aiString textypeStr;
//	mat->GetTexture(aiTextureType_DIFFUSE, 0, &textypeStr);
//	std::string textypeteststr = textypeStr.C_Str();
//	if (textypeteststr == "*0" || textypeteststr == "*1" || textypeteststr == "*2" || textypeteststr == "*3" || textypeteststr == "*4" || textypeteststr == "*5") {
//		if (scene->mTextures[0]->mHeight == 0) {
//			return "embedded compressed texture";
//		} else {
//			return "embedded non-compressed texture";
//		}
//	}
//	if (textypeteststr.find('.') != std::string::npos) {
//		return "textures are on disk";
//	}
//
//    return ".";
//}
//
//int CModel::getTextureIndex(aiString * str) {
//	std::string tistr;
//	tistr = str->C_Str();
//	tistr = tistr.substr(1);
//	return stoi(tistr);
//}
//
//ID3D11ShaderResourceView * CModel::getTextureFromModel(const aiScene * scene, int textureindex) {
//	HRESULT hr;
//	ID3D11ShaderResourceView *texture;
//
//	int* size = reinterpret_cast<int*>(&scene->mTextures[textureindex]->mWidth);
//
//	hr = CreateWICTextureFromMemory(dev, devcon, reinterpret_cast<unsigned char*>(scene->mTextures[textureindex]->pcData), *size, nullptr, &texture);
//	if (FAILED(hr))
//		MessageBox(hwnd, L"Texture couldn't be created from memory!", L"Error!", MB_ICONERROR | MB_OK);
//
//	return texture;
//}
#endif // OPENGL