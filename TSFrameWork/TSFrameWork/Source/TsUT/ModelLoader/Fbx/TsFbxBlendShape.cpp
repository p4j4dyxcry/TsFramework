#include "../../TsUT.h"
#include "TsFbxHeader.h"

TsFbxShape::TsFbxShape(FbxShape* pShape , )
{


}

TsBool TsFbxShape::ParseBlendShape(FbxMesh  * pFbxMesh,
						FbxAnimLayer * pAnimLayer)
{
	SetName(pShape->GetName());


	FbxBlendShape* pBlendShape = (FbxBlendShape*)pFbxMesh->GetDeformer(0, FbxDeformer::eBlendShape);
	TsInt channelCount = pBlendShape->GetBlendShapeChannelCount();
	for (TsInt shapeIdx = 0; shapeIdx < channelCount; ++shapeIdx)
	{
		FbxBlendShapeChannel* shapeCH = pBlendShape->GetBlendShapeChannel(shapeIdx);
		TsInt shapeCount = shapeCH->GetTargetShapeCount();
		for (TsInt chIdx = 0; chIdx < shapeCount; ++chIdx)
		{
			TsInt  shapeIndexCount = pShape->GetControlPointIndicesCount();
			TsInt* shapeIndexPtr = pShape->GetControlPointIndices();
			TsInt  fbxPosCount = pShape->GetControlPointsCount();
			FbxVector4* fbxPositionList = pShape->GetControlPoints();

			m_shape.resize(shapeIndexCount);
			for (TsInt i = 0; i < shapeIndexCount; ++i)
			{
				m_shape[i].index = shapeIndexPtr[i];
				m_shape[i].pos.x = (float)fbxPositionList[shapeIndexPtr[i]][0];
				m_shape[i].pos.y = (float)fbxPositionList[shapeIndexPtr[i]][1];
				m_shape[i].pos.z = (float)fbxPositionList[shapeIndexPtr[i]][2];

				FbxAnimCurve* lFCurve = pFbxMesh->GetShapeChannel(shapeIdx, chIdx, pAnimLayer);
			}

		}
	}

}

TsVector<TsFbxShape::Shape> TsFbxShape::GetShapes()const
{
	return m_shape;
}
/*
FbxAnimCurve* lFCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);
if (!lFCurve) continue;
double lWeight = lFCurve->Evaluate(pTime);

// Find the two shape indices for influence calculation according to the weight.
// Consider index of base geometry as -1.

int lShapeCount = lChannel->GetTargetShapeCount();
double* lFullWeights = lChannel->GetTargetShapeFullWeights();

// Find out which scope the lWeight falls in.
int lStartIndex = -1;
int lEndIndex = -1;
for (int lShapeIndex = 0; lShapeIndex<lShapeCount; ++lShapeIndex)
{
	if (lWeight > 0 && lWeight <= lFullWeights[0])
	{
		lEndIndex = 0;
		break;
	}
	if (lWeight > lFullWeights[lShapeIndex] && lWeight < lFullWeights[lShapeIndex + 1])
	{
		lStartIndex = lShapeIndex;
		lEndIndex = lShapeIndex + 1;
		break;
	}
}

FbxShape* lStartShape = NULL;
FbxShape* lEndShape = NULL;
if (lStartIndex > -1)
{
	lStartShape = lChannel->GetTargetShape(lStartIndex);
}
if (lEndIndex > -1)
{
	lEndShape = lChannel->GetTargetShape(lEndIndex);
}

//The weight percentage falls between base geometry and the first target shape.
if (lStartIndex == -1 && lEndShape)
{
	double lEndWeight = lFullWeights[0];
	// Calculate the real weight.
	lWeight = (lWeight / lEndWeight) * 100;
	// Initialize the lDstVertexArray with vertex of base geometry.
	memcpy(lDstVertexArray, lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
	for (int j = 0; j < lVertexCount; j++)
	{
		// Add the influence of the shape vertex to the mesh vertex.
		FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j]) * lWeight * 0.01;
		lDstVertexArray[j] += lInfluence;
	}
}
//The weight percentage falls between two target shapes.
else if (lStartShape && lEndShape)
{
	double lStartWeight = lFullWeights[lStartIndex];
	double lEndWeight = lFullWeights[lEndIndex];
	// Calculate the real weight.
	lWeight = ((lWeight - lStartWeight) / (lEndWeight - lStartWeight)) * 100;
	// Initialize the lDstVertexArray with vertex of the previous target shape geometry.
	memcpy(lDstVertexArray, lStartShape->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
	for (int j = 0; j < lVertexCount; j++)
	{
		// Add the influence of the shape vertex to the previous shape vertex.
		FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lStartShape->GetControlPoints()[j]) * lWeight * 0.01;
		lDstVertexArray[j] += lInfluence;
	}
}
*/