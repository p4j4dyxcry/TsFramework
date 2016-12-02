//!*******************************************************
//! TsCollisionOctTree.h
//!
//! コリジョン8分木.
//!
//! © 2016 Yuki Tsuneyama
#pragma once

class TsCollisionTreeCell;

//----------------------------------------------------------
//! コライダをラッピングした循環リストオブジェクトクラス
//----------------------------------------------------------
class TsCollisionTreeForCollider
{
    //----------------------------------------------------------
    // Define
    //----------------------------------------------------------
    //! セルからポインタのすげ替えを許可する
    friend TsCollisionTreeCell;
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    //!　コンストラクタ
    TsCollisionTreeForCollider() :
        m_pCollider(nullptr),
        m_pNext(nullptr),
        m_pPrev(nullptr)
    {}

    //! コライダをセットする
    TsBool SetCollider(TsCollider* pCollider)
    {
        m_pCollider = pCollider; 
        return TS_TRUE;
    }

    //! コライダを取得する
    TsCollider* GetCollider(){ return m_pCollider; }

    //! 次の循環ポインタを取得する 
    TsCollisionTreeForCollider* Next(){ return m_pNext; }

    //! 後ろの循環ポインタを取得する
    TsCollisionTreeForCollider* Prev(){ return m_pPrev; }

    //! 自身を循環リストから削除する
    TsBool Remove()
    {
        if (m_pNext)
            m_pNext->m_pPrev = m_pPrev;
        if (m_pPrev)
            m_pPrev->m_pNext = m_pNext;

        m_pNext = nullptr;
        m_pPrev = nullptr;
    }
private:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsCollisionTreeForCollider* m_pNext;
    TsCollisionTreeForCollider* m_pPrev;
    TsCollider* m_pCollider;
};

//----------------------------------------------------------
//! 分割空間に含まれるオブジェクトの循環リストを持つクラス
//----------------------------------------------------------
class TsCollisionTreeCell
{
public:
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsCollisionTreeCell():
        m_pHead(nullptr)
    {

    }

    //! 登録されている全オブジェクトのリンクを破棄する
    TsBool Reset()
    {
        TsCollisionTreeForCollider* ptr = m_pHead;
        while (ptr)
        {
            TsCollisionTreeForCollider* next = ptr->Next();
            ptr->m_pNext = nullptr;
            ptr->m_pPrev = nullptr;
        }
        return TS_TRUE;
    }

    //! セルにオブジェクトを登録する
    TsBool Add(TsCollisionTreeForCollider* ptr)
    {
       if (m_pHead == nullptr)
        {
           //! Headがなかった場合自身をHeadとして登録する
           //  循環リストなのでHeadのNext,PrevともにHeadになる。
            m_pHead = ptr;
            m_pHead->m_pNext = ptr;
            m_pHead->m_pPrev = ptr;
        }
        else
        {
            //! 追加の高速化の為に循環リストを用いている。
            //  循環リストを使った場合はHeadの後ろがLastになるので
            //  そこのポインタを挿げ替える。
            TsCollisionTreeForCollider* tail = m_pHead->m_pPrev;
            tail->m_pNext = ptr;
            ptr->m_pNext = m_pHead;
            ptr->m_pPrev = tail;
            m_pHead->m_pPrev = ptr;
        }
        return TS_TRUE;
    }
    //! 先頭コライダの取得
    TsCollisionTreeForCollider* GetHead(){ return m_pHead; }
private:
    //----------------------------------------------------------
    // propery
    //----------------------------------------------------------
    TsCollisionTreeForCollider* m_pHead;
};

//----------------------------------------------------------
//! ８分木衝突判定クラス
//----------------------------------------------------------
class TsCollisionOctTree
{
public:
    static const TsInt MAX_DIV = 6; //! 最大分割レベル
    //----------------------------------------------------------
    // public method
    //----------------------------------------------------------
    TsCollisionOctTree();
    virtual ~TsCollisionOctTree();

    //! 初期化
    TsBool Initalize(const TsAABB3D& aabb, TsInt div = 3);

    //! コライダの登録
    TsBool Register(const TsAABB3D& aabb, TsCollisionTreeForCollider* pCollider);

    //! 引数のaabbと衝突する可能性のあるコライダ群を取得する
    TsVector<TsCollider*> GetCollisionList(const TsAABB3D& aabb);

protected:
    //! モートンオーダーを取得する
    //  モートンオーダーとは8分木空間をIndex化したときにO(1)で所属する
    //  セルのインデックスをAABBから特定するアルゴリズム。
    TsU64 GetMortonOrder(const TsAABB3D& range);

    //----------------------------------------------------------
    // peropery
    //----------------------------------------------------------
    TsAABB3D m_range;               // 8分木の全体領域
    TsInt    m_divLevel;            // 分割レベル

    TsCollisionTreeCell* m_pCells;   //セルの線形リスト
    TsInt    m_cellNum;             // セルの数
};