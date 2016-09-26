//!*******************************************************
//!	CSkinAnimation.h
//!
//!	スキンアニメ
//!
//!	著作権	2016 Yuki Tsuneyama
//!	作成日時	16.01.23

#pragma once

#include <vector>
#include <unordered_map>

struct AnimationFrameInfo
{
	std::string m_name;
	CMatrix		m_matrix;
	int			m_frame;
};

class CSkinAnimation
{
protected:
	int		m_startFrame;
	int		m_endFrame;


public:
	std::vector <std::unordered_map<std::string , CMatrix >> m_lib;

	void Initialize( int startFrame , int endFrame )
	{
		m_startFrame = startFrame;
		m_endFrame = endFrame;
		m_lib.resize( endFrame );
	}

	void Insert( int frame ,const char * name ,CMatrix m)
	{
		m_lib[frame].insert( std::pair<std::string , CMatrix>( name,m ) );
	}

	CMatrix GetData( int frame , std::string name )const
	{
		if( ( unsigned )frame > ( unsigned )m_endFrame )
			return CMatrix::identity;

		auto lib = m_lib[frame];

		if( lib.find( name ) == lib.end() )
		{
			return CMatrix::identity;
		}
		return lib[name];
	}
};