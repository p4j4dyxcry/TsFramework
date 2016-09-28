#include "../TsUT/TsUT.h"
#include <functional>

#include "TsTransForm.h"
#include "TsMatrix.h"


using namespace std;

TsTransForm::TsTransForm(const TsMatrix& m)
{
	this->TsTransForm::TsTransForm();
	*this = m;
}
TsTransForm::~TsTransForm()
{
	if (m_parent)
	{
		RemoveOfParantChild();
	}
	auto p = m_firstChild;
	while (p != nullptr)
	{
		auto temp = p;
		p = p->m_subling;
		TsDelete(temp);
	}
}
TsTransForm& TsTransForm::operator=( const TsMatrix& m )
{
	//! �s��f�[�^�����f�[�^�E�ʒu�f�[�^�E�X�P�[���f�[�^�ɕ���

	//���f�[�^
	TsVector3 xA( m.m[0][0] , m.m[0][1] , m.m[0][2] );
	TsVector3 yA( m.m[1][0] , m.m[1][1] , m.m[1][2] );
	TsVector3 zA( m.m[2][0] , m.m[2][1] , m.m[2][2] );

	//�X�P�[���f�[�^
	float xScale = xA.Length();
	float yScale = yA.Length();
	float zScale = zA.Length();

	//�ʒu�f�[�^
	TsVector3 pos = TsVector3(m.m[3][0], m.m[3][1], m.m[3][2]);

	//��]�f�[�^�̌v�Z�͊g�k�����E���s�ړ���������������]�s��ɗ��Ƃ��Ă���N�H�[�^�j�I��������B
	TsMatrix rotate;

	//�����X�P�[���Ŋ��邱�Ƃŉ�]�s��̊e���������߂�B
	rotate.m[0][0] = xA.x / xScale;
	rotate.m[1][0] = xA.y / xScale;
	rotate.m[2][0] = xA.z / xScale;

	rotate.m[0][1] = yA.x / yScale;
	rotate.m[1][1] = yA.y / yScale;
	rotate.m[2][1] = yA.z / yScale;

	rotate.m[0][2] = zA.x / zScale;
	rotate.m[1][2] = zA.y / zScale;
	rotate.m[2][2] = zA.z / zScale;

	//�l�̐ݒ�i����͑S�ă��[�J���l�Ɖ��߂��邱�Ƃɂ���)
	m_localScale = TsVector3(xScale, yScale, zScale);
	m_localPosition = pos;
	m_localRotate = rotate.ToQuaternion();

	return *this;
}

TsTransForm TsTransForm::operator*( const TsMatrix& m )
{
	TsMatrix local = ToLocalMatrix();
	TsTransForm result( ( local * m ) );
	result.SetName( GetName() );
	result.m_parent = m_parent;
	return result;
}

TsTransForm& TsTransForm::operator*=( const TsMatrix& m )
{
	return *this = (*this * m);
}
//!*******************************************************
//!	[���\�b�h] ���[�J���s����擾����
TsMatrix TsTransForm::ToLocalMatrix()
{
	return TsMatrix( m_localPosition , m_localRotate ,m_localScale);
}

//!*******************************************************
//!	[���\�b�h] ���[���h�s����擾����
TsMatrix TsTransForm::ToWorldMatrix()
{
	TsMatrix result;
	TsTransForm* temp = this;

	TsStack<TsMatrix> matStack;

	//�e��H���ă��[�J���s����X�^�b�N�Ɋi�[����
	do 
	{
		matStack.push( temp->ToLocalMatrix() );
		temp = temp->m_parent;
	} while( temp != nullptr );

	//�X�^�b�N���烍�[�J���s������o�����[���h�s����v�Z����
	while( matStack.empty() == false )
	{
		result *= matStack.top();
		matStack.pop();
	}

	return result;
}

TsTransForm* TsTransForm::GetParent()const
{
	return m_parent;
}

TsVector3 TsTransForm::GetWorldPos()
{
	TsMatrix m = ToWorldMatrix();
	return TsVector3(m._42, m._42, m._43);
}

TsVector3 TsTransForm::GetWorldScale()
{
	TsTransForm t = ToWorldMatrix();
	return t.m_localScale;
}

TsQuaternion TsTransForm::GetWorldRotate()
{
	TsTransForm t = ToWorldMatrix();
	return t.m_localRotate;
}

TsBool TsTransForm::SetParent(__inout TsTransForm* parent)
{
	//���ɐe������ꍇ�͂��̐e�Ƃ̊֌W�𐴎Z���A�V�����e�Ɗ֌W���\�z����K�v������B
	if( m_parent )
	{		
		if (RemoveOfParantChild() == TS_FALSE)
			return TS_FALSE;
	}

	//�e�Ɏq�������Ȃ��ꍇ���q�������ɂ���
	if( parent->m_firstChild == nullptr )
		parent->m_firstChild = this;

	//�e�ɂ��łɎq��������ꍇ�͌Z��̖����Ɏ�����ǉ�����
	else
	{

		TsTransForm ** subling = &( m_parent->m_firstChild->m_subling );
		while( *subling != nullptr )
		{
			subling = &( ( *subling )->m_subling );
		}
		*subling = this;
	}

	m_parent = parent;
	return TS_TRUE;
}

TsBool TsTransForm::AddChild(TsTransForm* pTransform)
{
	return pTransform->SetParent(this);
}



TsBool	TsTransForm::RemoveOfParantChild()
{
	// �e�̑��q�������Ȃ炻�̂܂܃|�C���^�̌q���ς�
	if (m_parent->m_firstChild == this)
	{
		m_parent->m_firstChild = m_parent->m_firstChild->m_subling;
	}
	//�e�̑��q�ȍ~�������Ȃ�Z���H���ă|�C���^�̌q���ς�
	else if (m_subling)
	{
		auto s = m_parent->m_firstChild->m_subling;
		for (; s != nullptr; s = s->m_subling)
		{
			if (s->m_subling == this)
			{
				s->m_subling = m_subling;
			}
		}
	}
	else
	{
		//�؍\��������ɂł��Ă��Ȃ��B�G���[
		return TS_FALSE;
	}

	return TS_TRUE;
}

TsTransForm* TsTransForm::FindChildByhash(TS_HASH hash)
{
	std::function<TsTransForm*(TsTransForm*)> TreeSearch = [&](TsTransForm* p)
	{
		if (p->GetHashCode() == hash)
			return p;
		if (p->m_firstChild)
			return TreeSearch(p->m_firstChild);
		if (p->m_subling)
			return TreeSearch(p->m_subling);

		return (TsTransForm*)nullptr;
	};

	return TreeSearch(this);
}

TsTransForm* TsTransForm::FindChildByName(const TsString& name)
{
	TS_HASH hash = TSUT::StringToHash(name);

	return FindChildByhash(hash);
}