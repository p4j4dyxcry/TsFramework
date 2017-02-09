#include "TsFbxAfx.h"
#include "../../../../TsAfx.h"

TsFbxLoader::TsFbxLoader()
    :m_isLoadGeometory(TS_TRUE)
    , m_isLoadAnimation(TS_FALSE)
{
    m_pFbxContext = TsNew( TsFbxContext );
}
TsFbxLoader::~TsFbxLoader()
{
    TsSafeDelete( m_pFbxContext );
}

//! Load From File
TsBool TsFbxLoader::LoadFile(const TsChar* filename)
{
    Ts3DModelLoader::LoadFile(filename);
    TsFbxContext::TsFbxLoadOption fbxLoadOption;

    fbxLoadOption.loadAnimation = m_isLoadAnimation;
    fbxLoadOption.loadBiNormal  = 
    fbxLoadOption.loadMesh      = 
    fbxLoadOption.loadTangent   =
    fbxLoadOption.loadVertexColor = m_isLoadGeometory;

    return m_pFbxContext->LoadFBX( filename ,fbxLoadOption );
}
TsBool TsFbxLoader::CreateCommonData()
{
    TsFbxScene* pScene = m_pFbxContext->GetSceneByIndex(0);

    m_pSkeleton = pScene->CreateSkeleton();
    if (pScene->GetNodeList().size() > 0)
        m_pTransform = pScene->GetNodeList()[0]->GetTransform()->GetRootTransform();

    //----------------------------------------------------------
    //! Convert Material
    //----------------------------------------------------------
    std::vector<TsFbxMaterial>&& matList = pScene->GetMaterialList();
    do{
        m_materialCount = matList.size();
        if (m_materialCount == 0)
            break;

        m_pMaterials = TsNew(TsCommon3DMaterial[m_materialCount]);
        for (TsUint i = 0; i < m_materialCount; ++i)
        {
            TsCommon3DMaterial& common = m_pMaterials[i];
            TsFbxMaterial&      origin = matList[i];

            common.m_diffuse = origin.m_diffuse;
            common.m_alpha = origin.m_diffuse.w;
            common.m_ambient = origin.m_ambient;
            common.m_name = origin.GetName();
            common.m_specluer = origin.m_specular;
            common.m_power = origin.m_specularPower;
            common.m_albedoTexture = origin.GetAlbedoTextureName().GetFileName();
            common.m_normalTexture = origin.GetNormalTextureName().GetFileName();
            common.m_specluerTexture = origin.GetSpecularTextureName().GetFileName();
            common.m_specluerPowerTexture = "";
        }
    } while (0);

    //----------------------------------------------------------
    //! Convert Vertex
    //----------------------------------------------------------
    std::vector<TsFbxMesh*>&& fbxmesh = pScene->GetMeshList();
    do{
        if (m_isLoadGeometory == TS_FALSE)
            break;

        m_meshCount = fbxmesh.size();
        if (m_meshCount == 0)
            break;

        m_pMeshs = TsNew(TsCommon3DMesh[m_meshCount]);

        auto Float2ToVec2 = [](TsFloat2 f)
        {
            return TsVector2(f.x, f.y);
        };

        auto Float3ToVec3 = [](TsFloat3 f)
        {
            return TsVector3(f.x, f.y, f.z);
        };

        auto Float4ToVec4 = [](TsFloat4 f)
        {
            return TsVector4(f.x, f.y, f.z,f.w);
        };

        for (TsUint i = 0; i < m_meshCount; ++i)
        {
            TsCommon3DMesh& common = m_pMeshs[i];
            TsFbxMesh*      origin = fbxmesh[i];

            common.m_vertexCount = origin->GetVertexCount();
            auto&& fbxVertex = origin->GetVertexList();

            common.m_name = origin->GetName();

            common.m_pPositions  = TsNew(TsVector3[common.m_vertexCount]);
            common.m_pNormals    = TsNew(TsVector3[common.m_vertexCount]);
            common.m_pBinormals  = TsNew(TsVector3[common.m_vertexCount]);
            common.m_pTangents   = TsNew(TsVector3[common.m_vertexCount]);
            common.m_pTexcoords  = TsNew(TsVector2[common.m_vertexCount]);
            common.m_pColors     = TsNew(TsFloat4[common.m_vertexCount]);
            common.m_pWeights    = TsNew(TsVector4[common.m_vertexCount]);
            common.m_pBoneIndex  = TsNew(TsInt4[common.m_vertexCount]);

            common.m_pMaterialRef = &m_pMaterials[origin->GetMaterialIndex()];
            common.m_pTransoform = origin->GetTransform();

            for (TsUint i = 0; i < common.m_vertexCount; ++i)
            {
                common.m_pPositions[i]  = Float3ToVec3(fbxVertex[i].pos);
                common.m_pNormals[i]    = Float3ToVec3(fbxVertex[i].normal);
                common.m_pBinormals[i]  = Float3ToVec3(fbxVertex[i].binormal);
                common.m_pTangents[i]   = Float3ToVec3(fbxVertex[i].tangent);
                common.m_pTangents[i]   = Float3ToVec3(fbxVertex[i].tangent);
                common.m_pTexcoords[i]  = Float2ToVec2(fbxVertex[i].uv[0]);
                common.m_pColors[i]     = fbxVertex[i].color.m_color;
                common.m_pWeights[i]    = Float4ToVec4(fbxVertex[i].boneWeight);
                common.m_pBoneIndex[i]  = fbxVertex[i].boneIndex;

                common.m_aabb.AddPoint(common.m_pPositions[i]);
            }           
        }
    } while (0);

    return TS_TRUE;
}

TsTransformBakeAnimation* TsFbxLoader::CreateAnimation( TsInt no )
{
    TsFbxAnimation* pFbxAnim = m_pFbxContext->GetSceneByIndex( 0 )->GetAnimation( no );
    TsF32 frameRate = m_pFbxContext->GetSceneByIndex( 0 )->GetFrameRate( );
    TsTransformBakeAnimation* pTransAim = TsNew( TsTransformBakeAnimation );
    pTransAim->SetBakeAnimation( pFbxAnim->GetBoneFrameLibrary() );
    pTransAim->SetFrameRate( frameRate );
    return pTransAim;
}

void TsFbxLoader::SetLoadAnimationFlag(TsBool isLoadAnim)
{
    m_isLoadAnimation = isLoadAnim;
}
void TsFbxLoader::SetLoadGeometoryFlag(TsBool isLoadGeometory)
{
    m_isLoadGeometory = isLoadGeometory;
}