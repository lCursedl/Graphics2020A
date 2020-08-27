#pragma once

#include "Includes.h"
#include <string>

class CMaterial
{
public:

	CMaterial();
	~CMaterial();

	void shutdown();
	/** \fn void shutdown()
	*	\brief Releases allocated resources
	*/

	std::string			m_Normal;					/**< string with normal texture name */
	bool				m_HasNormal;				/**< bool which defines if material has normal texture */
	bool				m_HasDiffuse;				/**< bool which defines if material has diffuse texture */
	std::string			m_TextureDir;				/**< string with texture path */
	std::string			m_Diffuse;					/**< string with diffuse texture name */

#ifdef D3D11
	ID3D11ShaderResourceView*	m_TextureDiffuse;	/**< DirectX resource for diffuse texture */
	ID3D11ShaderResourceView*	m_TextureNormal;	/**< DirectX resource for normal texture */
	ID3D11ShaderResourceView*	m_TextureSpecular;
#endif // D3D11	
};
/** \class CMaterial
*	\brief Class which contains data for material creation/usage in a mesh.
*/