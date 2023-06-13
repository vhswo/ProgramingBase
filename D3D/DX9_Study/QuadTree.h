#pragma once

class QuadTree
{
private:
    enum class Location {
    FRUSTUM_OUT = 0, /// ����ü�� ������ �ܺ�.
    FRUSTUM_PARTIALLY_IN = 1, /// ����ü�� �κ��� ��, �ܺ�.
    FRUSTUM_COMPLETELY_IN = 2, /// ����ü�� ������ ����.
    UNKNOWN = -1 /// �˼� ���� ����.
};
    enum CornerType { CornerType_TL = 0, CornerType_TR, CornerType_BL, CornerType_BR };
    int center;
    int corners[4];
    bool isCulled;
    float radius;/// ��豸�� ������.
    QuadTree* childs[4];
    bool IsVisible() { return (1 >= (corners[CornerType_TR] - corners[CornerType_TL])); }
    // pIndex : ������(triangle) �ε��� ���� output
    // return : �׸��� �� ������(triangle)�� ��.
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
    bool SubDivied(); // 4���� ���� ���� ����.
public:
    void Build(LPD3DXVECTOR3 pVertices);// ����Ʈ�� ����.
    int GetIndices(LPVOID pIndices, LPD3DXVECTOR3 pVertices);
};
