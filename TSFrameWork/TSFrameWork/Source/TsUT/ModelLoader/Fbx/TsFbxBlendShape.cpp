#include "../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxShape::TsFbxShape( TsFbxContext* pFbxContext ,
                        TsFbxScene * pFbxScene)
                        :TsFbxObject( pFbxContext,pFbxScene )
{


}

TsBool TsFbxShape::ParseBlendShape(FbxMesh  * pFbxMesh,
                        FbxAnimLayer * pAnimLayer)
{
    TsInt deformerCount = pFbxMesh->GetDeformerCount();
    
    //todo ���݂͂P�̃u�����h�V�F�C�v�ɂ����Ή����Ȃ��B
    //�@�@�@�ނ��땡���̃u�����h�V�F�C�v�̃C���[�W�����Ȃ�
    if( deformerCount > 0 )
        deformerCount = 1;

    for( TsInt deformerIdx = 0; deformerIdx < deformerCount; ++deformerIdx )
    {
        FbxBlendShape* pBlendShape = ( FbxBlendShape* )pFbxMesh->GetDeformer( deformerIdx , FbxDeformer::eBlendShape );
        TsInt channelCount = pBlendShape->GetBlendShapeChannelCount();
        m_blendShapeLayers.resize( channelCount );
        for( TsInt chIdx = 0; chIdx < channelCount; ++chIdx )
        {
            FbxBlendShapeChannel* shapeCH = pBlendShape->GetBlendShapeChannel( chIdx );
            TsInt shapeCount = shapeCH->GetTargetShapeCount();

            TsFbxShapeArray shape;

            //�ό`��̒��_�������擾
            {
                for( TsInt shapeIdx = 0; shapeIdx < shapeCount; ++shapeIdx )
                {
                    FbxShape* pShape = shapeCH->GetTargetShape( shapeIdx );
                    TsInt  shapeIndexCount = pShape->GetControlPointIndicesCount();
                    TsInt* shapeIndexPtr = pShape->GetControlPointIndices();
                    TsInt  fbxPosCount = pShape->GetControlPointsCount();
                    FbxVector4* fbxPositionList = pShape->GetControlPoints();

                    for( TsInt i = 0; i < shapeIndexCount; ++i )
                    {
                        TsFbxShapeVertex shapeVtx;
                        shapeVtx.index = shapeIndexPtr[i];
                        //����n�Ή�
                        shapeVtx.pos.x = ( float )fbxPositionList[shapeIndexPtr[i]][0] * -1.0f;
                        shapeVtx.pos.y = ( float )fbxPositionList[shapeIndexPtr[i]][1];
                        shapeVtx.pos.z = ( float )fbxPositionList[shapeIndexPtr[i]][2];
                        shape.push_back( shapeVtx );
                    }
                }
                m_blendShapeLayers[chIdx].m_shape.push_back( shape );
            }

            //�ό`�A�j���[�V�����̎擾
            {
                FbxAnimCurve* pAnimCurve = pFbxMesh->GetShapeChannel( deformerIdx , chIdx , pAnimLayer );
                
                if( pAnimCurve )
                {
                    TsF64 * fullWeight = shapeCH->GetTargetShapeFullWeights();
                    for( TsInt i = 0; i < pAnimCurve->KeyGetCount(); ++i )
                    {
                        TsFbxBlendShapeKey key;
                        key.weight = pAnimCurve->KeyGetValue( i ) ;
                        key.time = (TsF32)pAnimCurve->KeyGetTime( i ).GetSecondDouble();

                        key.beginIndex =
                        key.endIndex = -1;
                        for( TsInt shapeIdx = 0; shapeIdx < shapeCount; ++shapeIdx )
                        {
                            //�P���ȂQ�̃u�����h�V�F�C�v
                            if( key.weight > 0 && key.weight <= fullWeight[0] )
                            {
                                key.endIndex = chIdx;
                                break;
                            }
                            //�R�ȏ�̃u�����h�V�F�C�v�Ή�
                            if( key.weight > fullWeight[shapeIdx] && key.weight < fullWeight[shapeIdx + 1] )
                            {							
                                //todo �Ή������f�[�^�\�����݂Ă���l����B

                                key.beginIndex = shapeIdx;
                                key.endIndex = shapeIdx + 1;
                                break;
                            }
                        }
                        m_blendShapeLayers[chIdx].m_keys.push_back( key );
                    }
                }
            }
        }
    }
    return TS_TRUE;
}

TsVector<TsFbxShapeArray> TsFbxShape::GetShapeLayer( TsInt index )const
{
    return m_blendShapeLayers[index].GetShapeList();
}

TsVector<TsFbxShape::BlendShapeLayer> TsFbxShape::GetBlendShapeKeys( )const
{
    return m_blendShapeLayers;
}

//! Blend Shape�̒��_�����ŏI�C���f�b�N�X�ɑΉ�������B
TsBool TsFbxShape::ConvertFinalIndex( const TsVector<TsFbxFace>& faceList)
{
    //todo �v�œK��
    for( TsUint i = 0; i < faceList.size(); ++i )
    for( TsUint j = 0; j < m_blendShapeLayers.size(); ++j )
    for( TsUint k = 0; k < m_blendShapeLayers[j].m_shape.size(); ++k )
    for( TsUint l = 0; l < m_blendShapeLayers[j].m_shape[k].size(); ++l )
    for( TsUint m = 0; m < 3; ++m )
    if( faceList[i].posIndex[m] == m_blendShapeLayers[j].m_shape[k][l].index )
        m_blendShapeLayers[j].m_shape[k][l].index = faceList[i].finalIndex[m];

    return TS_TRUE;
}