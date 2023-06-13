#pragma once

class QuadTree
{
private:
    enum class Location {
    FRUSTUM_OUT = 0, /// 절두체의 완전한 외부.
    FRUSTUM_PARTIALLY_IN = 1, /// 절두체의 부분적 내, 외부.
    FRUSTUM_COMPLETELY_IN = 2, /// 절두체의 완전한 내부.
    UNKNOWN = -1 /// 알수 없는 상태.
};
    enum CornerType { CornerType_TL = 0, CornerType_TR, CornerType_BL, CornerType_BR };
    int center;
    int corners[4];
    bool isCulled;
    float radius;/// 경계구의 반지름.
    QuadTree* childs[4];
    bool IsVisible() { return (1 >= (corners[CornerType_TR] - corners[CornerType_TL])); }
    // pIndex : 폴리곤(triangle) 인덱스 정보 output
    // return : 그리게 될 폴리곤(triangle)의 수.
    int GenerateIndices(LPVOID pIndices, int triangles = 0);
    Location FrustumLocation(LPD3DXVECTOR3 pVertices);
    void ProcessFrustumCull(LPD3DXVECTOR3 pVertices);

public:
    QuadTree(int cx, int cy);
    ~QuadTree();
private:
    QuadTree();
    void SetCorners(int cornerTL, int cornerTR, int cornerBL, int cornerBR);
    QuadTree* AddChild(int cornerTL, int cornerTR, int cornerBL, int cornerBR);
    bool SubDivied(); // 4개의 하위 노드로 분할.
public:
    void Build(LPD3DXVECTOR3 pVertices);// 쿼드트리 구축.
    int GetIndices(LPVOID pIndices, LPD3DXVECTOR3 pVertices);
};
