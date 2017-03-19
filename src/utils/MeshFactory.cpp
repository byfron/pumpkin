#include "MeshFactory.hpp"
#include "VertexUtils.hpp"

namespace pumpkin {

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
		p.m_u = uvCoord[0]*0x7fff;
		p.m_v = uvCoord[1]*0x7fff;
		vertices.push_back(p);
	}

	for (int i = 0; i < num_triangles; i++) {
		assert(pMesh->GetPolygonSize(i) == 3);
		for (int t = 0; t < pMesh->GetPolygonSize(i); t++) {
			int vertexIndex = pMesh->GetPolygonVertex(i, t);
			indices.push_back(vertexIndex);
		}
	}

	const bgfx::Memory* mem;
	mem = bgfx::makeRef(&vertices[0], sizeof(PosNormalTexCoordVertex) * vertices.size() );
	g.m_dvbh = bgfx::createDynamicVertexBuffer(mem, PosNormalTexCoordVertex::ms_decl);
	mem = bgfx::makeRef(&indices[0], sizeof(uint16_t) * indices.size() );
	g.m_dibh = bgfx::createDynamicIndexBuffer(mem);

	mesh.m_groups.push_back(g);
	
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

}
