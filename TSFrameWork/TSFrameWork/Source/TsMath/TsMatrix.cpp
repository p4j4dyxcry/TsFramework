#include "TsMatrix.h"
#include "TsVector.h"
#include "TsQuaternion.h"

const TsMatrix TsMatrix::identity = XMFLOAT4X4( 1 , 0 , 0 , 0 ,
											  0 , 1 , 0 , 0 ,
											  0 , 0 , 1 , 0 ,
											  0 , 0 , 0 , 1 );
//! �ʒu�Ɖ�]����s��𐶐�����R���X�g���N�^
TsMatrix::TsMatrix(TsVector3& p , TsQuaternion& q)
{
	*this = CreateRotate( q ) * CreateTranslate( p );
}

//! �ʒu�Ɖ�]�Ɗg�k����s��𐶐�����R���X�g���N�^
TsMatrix::TsMatrix( TsVector3& p , TsQuaternion& q , TsVector3& scale )
{
	*this = CreateScale( scale ) * CreateRotate( q ) * CreateTranslate( p );
}

//! XNA�s�񂩂�̕ϊ��p
TsMatrix&  TsMatrix::operator =( const XMMATRIX& i )
{
	memcpy( m , i.m , sizeof( FLOAT ) * 16 );
	return *this;
}

//! �s��̊|���Z�v�Z��XNAVECTOR��simd�𗘗p����
TsMatrix  TsMatrix::operator *(const TsMatrix& matrix )const
{
	return TsMatrix(ToXMMATRIX() * matrix.ToXMMATRIX());
}

TsMatrix& TsMatrix::operator*= (const TsMatrix& matrix )
{
	return (*this = *this * matrix);
}

//! XNA�̍s��ɕϊ�����
XMMATRIX  TsMatrix::ToXMMATRIX()const
{
	return XMLoadFloat4x4( this );
}

//! �N�H�[�^�j�I���ɕϊ�����
TsQuaternion TsMatrix::ToQuaternion()const
{	
	// �ő听��������
	float elem[4]; // 0:x, 1:y, 2:z, 3:w
	elem[0] =  _11 - _22 - _33 + 1.0f;
	elem[1] = -_11 + _22 - _33 + 1.0f;
	elem[2] = -_11 - _22 + _33 + 1.0f;
	elem[3] =  _11 + _22 + _33 + 1.0f;

	unsigned biggestIndex = 0;
	for( int i = 1; i < 4; i++ ) {
		if( elem[i] > elem[biggestIndex] )
			biggestIndex = i;
	}

	if( elem[biggestIndex] < 0.0f )
		return TsQuaternion(); // �����̍s��ɊԈႢ����I

	// �ő�v�f�̒l���Z�o
	TsQuaternion q;
	float v = sqrtf( elem[biggestIndex] ) * 0.5f;
	
	float mult = 0.25f / v;
	switch( biggestIndex ) {
		case 0: // x
			q.x =  v;
			q.y = ( _12 + _21 ) * mult;
			q.z = ( _31 + _13 ) * mult;
			q.w = ( _32 - _23 ) * mult;
			break;
		case 1: // y
			q.x = ( _12 + _21 ) * mult;
			q.y = v;
			q.z = ( _23 + _32 ) * mult;
			q.w = ( _13 - _31 ) * mult;
			break;
		case 2: // z
			q.x = ( _31 + _13 ) * mult;
			q.y = ( _23 + _32 ) * mult;
			q.z = v;
			q.w = ( _21 - _12 ) * mult;
			break;
		case 3: // w
			q.x = ( _32 -_23  ) * mult;
			q.y = ( _13 -_31  ) * mult;
			q.z = ( _21 -_12  ) * mult;
			q.w = v;
			break;
	}
	return q;
}

TsMatrix& TsMatrix::Transpose()
{
	return *this = Transposed();
}

TsMatrix& TsMatrix::Inverse()
{
	return *this = Inversed();
}

TsMatrix TsMatrix::Transposed()const
{
	return XMMatrixTranspose( ToXMMATRIX() );
}

TsMatrix TsMatrix::Inversed()
{
	return XMMatrixInverse( nullptr , ToXMMATRIX() );
}

//!*******************************************************
//!	[���\�b�h] static�@���\�b�h

//! ���s�ړ��s��𐶐�����
TsMatrix TsMatrix::CreateTranslate( FLOAT x , FLOAT y , FLOAT z )
{
	return TsMatrix( XMMatrixTranslation( x , y , z ) );
}

TsMatrix TsMatrix::CreateTranslate( const TsVector3& offset )
{
	return TsMatrix::CreateTranslate(offset.x,offset.y,offset.z);
}

//! ��]�s��𐶐�����
TsMatrix TsMatrix::CreateRotate( const TsQuaternion& q )
{
	return TsMatrix( XMMatrixRotationQuaternion( q.ToXMVECTOR() ) );
}

//! �g�k�s��𐶐�����
TsMatrix TsMatrix::CreateScale( FLOAT x, FLOAT y ,FLOAT z )
{
	return TsMatrix( XMMatrixScaling( x , y , z ));
}

TsMatrix TsMatrix::CreateScale( const TsVector3& scale )
{
	return TsMatrix::CreateScale(scale.x,scale.y,scale.z);
}

TsMatrix TsMatrix::CreateLookAt( const TsVector3& eye , const TsVector3& at , const TsVector3& up )
{
	return TsMatrix( XMMatrixLookAtLH( eye.ToXMVECTOR() , at.ToXMVECTOR() , up.ToXMVECTOR() ));
}

TsVector3 TsMatrix::TransformCoord(const TsVector3& v)
{
	return XMVector3TransformCoord(v.ToXMVECTOR(), ToXMMATRIX());
}
TsVector3 TsMatrix::TransformVector(const TsVector3& v)
{
	return TsVector3(XMVector3Transform(v.ToXMVECTOR(), ToXMMATRIX()));
}
TsVector3 TsMatrix::TransformPoint(const TsVector3& v)
{
	TsVector3 result;
	result.x = v.x * m[0][0] +
			   v.y * m[1][0] +
			   v.z * m[2][0] +
				     m[3][0] ;

	result.y = v.x * m[0][1] +
			   v.y * m[1][1] +
			   v.z * m[2][1] +
			         m[3][1] ;

	result.z = v.x * m[0][2] +
			   v.y * m[1][2] +
			   v.z * m[2][2] +
			         m[3][2] ;

	return result;
}

TsVector4 TsMatrix::TransformVector(const TsVector4& v)
{
	return TsVector4(XMVector4Transform(v.ToXMVECTOR(), ToXMMATRIX()));;
}

TsBool TsMatrix::operator==(const TsMatrix& matrix)
{
	return  m[0][0] == matrix.m[0][0] && 
			m[0][1] == matrix.m[0][1] &&
			m[0][2] == matrix.m[0][2] &&
			m[0][3] == matrix.m[0][3] &&
			m[1][0] == matrix.m[1][0] &&
			m[1][1] == matrix.m[1][1] &&
			m[1][2] == matrix.m[1][2] &&
			m[1][3] == matrix.m[1][3] &&
			m[2][0] == matrix.m[2][0] &&
			m[2][1] == matrix.m[2][1] &&
			m[2][2] == matrix.m[2][2] &&
			m[2][3] == matrix.m[2][3] &&
			m[3][0] == matrix.m[3][0] &&
			m[3][1] == matrix.m[3][1] &&
			m[3][2] == matrix.m[3][2] &&
			m[3][3] == matrix.m[3][3] ;
}

TsBool TsMatrix::operator!=(const TsMatrix& matrix)
{
	return !(*this == matrix);
}