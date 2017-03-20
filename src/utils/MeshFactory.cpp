#include "MeshFactory.hpp"
#include "VertexUtils.hpp"
#include "Mesh.hpp"
#include <graphics/TextureAtlas.hpp>

namespace pumpkin {

bool getTextureFileNameFromNode(FbxNode *pNode, std::string & filename) {

	int lMaterialCount = pNode->GetMaterialCount();

    for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; lMaterialIndex++) {
		std::cout << lMaterialIndex << "/" << lMaterialCount << std::endl;
        FbxSurfaceMaterial *lMaterial = pNode->GetSrcObject<FbxSurfaceMaterial>(lMaterialIndex);
        bool lDisplayHeader = true;

        //go through all the possible textures
        if(lMaterial){

			FbxProperty lProperty;
            int lTextureIndex;
            FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
            {
		    lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
		    FbxFileTexture* lTexture = lProperty.GetSrcObject<FbxFileTexture>(lTextureIndex);
		    if(lTexture)
		    {
			    filename = lTexture->GetFileName();
			    return true;
		    }
            }

        }//end if(lMaterial)

    }// end for lMaterialIndex

	return false;
}

Mesh generateMesh(FbxNode* pNode) {

	Mesh mesh;
	mesh.m_decl = PosNormalTexCoordVertex::ms_decl;

	FbxMesh* pMesh = (FbxMesh*) pNode->GetNodeAttribute();
	int num_vertices = pMesh->GetControlPointsCount();
	FbxVector4* controlPoints = pMesh->GetControlPoints();
	int num_triangles = pMesh->GetPolygonCount();

	// Layer 0?
	FbxGeometryElementUV* leUV = pMesh->GetElementUV(0);

	assert(pMesh->GetElementUVCount() == 1);

	std::vector<PosNormalTexCoordVertex> vertices;
	std::vector<uint16_t> indices;

	Group g;
	for (int i = 0; i < num_vertices; i++)
	{
		pumpkin::PosNormalTexCoordVertex p;
		p.m_x = controlPoints[i][0];
		p.m_y = controlPoints[i][1];
		p.m_z = controlPoints[i][2];
		p.m_normal = pumpkin::NORMAL_POSZ;
		FbxVector2 uvCoord = leUV->GetDirectArray().GetAt(i);
		p.m_u = uvCoord[1]*0x7fff;
		p.m_v = uvCoord[0]*0x7fff;
		vertices.push_back(p);

		std::cout << uvCoord[0] << "," << uvCoord[1] << std::endl;
	}

	for (int i = 0; i < num_triangles; i++) {
		for (int t = 0; t < pMesh->GetPolygonSize(i); t++) {
			int vertexIndex = pMesh->GetPolygonVertex(i, t);
			indices.push_back(vertexIndex);
		}
	}

	mesh.m_groups.push_back(g);

//	FIX THIS FOR FUCK SAKE
	Group & gref = mesh.m_groups[0];

	const bgfx::Memory* mem;
	mem = bgfx::copy(&vertices[0], sizeof(PosNormalTexCoordVertex) * vertices.size() );
	gref.m_dvbh = bgfx::createDynamicVertexBuffer(mem, PosNormalTexCoordVertex::ms_decl);
//	g.m_dvbh = bgfx::createVertexBuffer(mem, PosNormalTexCoordVertex::ms_decl);
	mem = bgfx::copy(&indices[0], sizeof(uint16_t) * indices.size() );
	gref.m_dibh = bgfx::createDynamicIndexBuffer(mem);
//	g.m_dibh = bgfx::createIndexBuffer(mem);


	return mesh;
}


std::vector<Mesh> loadMeshes(FbxNode *pRootNode) {

	std::vector<Mesh> mesh_vector;

	if(pRootNode)
	{
		for(int i = 0; i < pRootNode->GetChildCount(); i++)
		{

			FbxNode* pNode = pRootNode->GetChild(i);
			if(pNode->GetNodeAttribute() == NULL)
			{
				FBXSDK_printf("NULL Node Attribute\n\n");
			}
			else {

				FbxNodeAttribute::EType attributeType =
					(pNode->GetNodeAttribute()->GetAttributeType());

				switch (attributeType)
				{
				case FbxNodeAttribute::eMesh:
					std::cout << "generating mesh:" << i << std::endl;
					mesh_vector.push_back(generateMesh(pNode));
					break;
				default:
					assert(false);
				}
			}
		}
	}

	return mesh_vector;
}

//TODO move this to TextureAtlasFactory
std::vector<TextureAtlas::Ptr> loadTextures(FbxNode *pRootNode) {

	std::vector<TextureAtlas::Ptr> textures;

	if(pRootNode)
	{
		for(int i = 0; i < pRootNode->GetChildCount(); i++)
		{
			std::string filename;
			FbxNode* pNode = pRootNode->GetChild(i);
			if (getTextureFileNameFromNode(pNode, filename)) {

				std::string name = std::string(TEXTURE_FILE_PATH) + filename;

				bgfx::TextureInfo info;
				bgfx::TextureHandle handle =
					loadTexture(name.c_str(),
						    BGFX_TEXTURE_NONE, 0, &info);

				TextureAtlas::Ptr tex = std::make_shared<TextureAtlas>();

				// diffuse channel
				tex->m_texture_handle[0] = handle;
				tex->m_stage[0] = 0;
				tex->m_sprite_width = info.width;
				tex->m_sprite_height = info.height;
				tex->m_grid_height = 1;
				tex->m_grid_width = 1;
				tex->m_atlas_file[0] = name;
				tex->m_num_textures = 1;
				tex->m_flags[0] = UINT32_MAX;
				tex->u_sampler[0] = bgfx::createUniform("s_texColor",  bgfx::UniformType::Int1);
				tex->u_texOffset = bgfx::createUniform("packed_input",  bgfx::UniformType::Vec4);
				tex->u_flip = bgfx::createUniform("flip", bgfx::UniformType::Int1);

				tex->m_offset[0] = 0.0f;
				tex->m_offset[1] = 0.0f;

				textures.push_back(tex);
			}
		}
	}
	return textures;
}

}
