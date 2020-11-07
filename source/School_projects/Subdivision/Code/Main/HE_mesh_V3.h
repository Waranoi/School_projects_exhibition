//
// Created by eliwar-5 on 2017-12-09.
//

#ifndef GSCEPT_LAB_ENV_HE_MESH_V3_H
#define GSCEPT_LAB_ENV_HE_MESH_V3_H

#include <vector>
#include <cstdio>
#include <cstring>
#include "myMathLib.h"

template <int BLOCKSIZE, typename T>
struct BlockAlloc
{
    int listSize = 0;
    std::vector<T*> blockPointers = std::vector<T*>();

    inline T* New()
    {
        int index = listSize % BLOCKSIZE;
        listSize++;

        if (index != 0)
        {
            T *arr = blockPointers[blockPointers.size() - 1];
            return &arr[index];
        }
        else
        {
            T *arr = new T[BLOCKSIZE];
            blockPointers.push_back(arr);
            //printf("%d vertex blocks have been made\n", (int)blockPointers.size());
            return &arr[0];
        }

    }

    inline T* operator[](int index) //Improve performance by having a Next function that gets the next value of the last given index
    {
        int listIndex = index / BLOCKSIZE;
        int blockIndex = index % BLOCKSIZE;
        return &blockPointers[listIndex][blockIndex];
    }
};

struct HE_vert
{
    float pos[3] = {0,0,0};
    int edge = 0;
};

struct HE_edge
{
    float uv[2] = {0,0};
    int pair = 0;
    int start = 0;
};

struct HE_face
{
    float norm[3] = {0,0,0};
    int edge = 0;
};

class HE_mesh_V3
{
public:
    BlockAlloc<sizeof(HE_vert) * 10000, HE_vert> verts;
    BlockAlloc<sizeof(HE_edge) * 40000, HE_edge> edges;
    BlockAlloc<sizeof(HE_face) * 10000, HE_face> faces;

    HE_mesh_V3(std::vector<float> positions, std::vector<float> texCoords, std::vector<float> normals, std::vector<int> indexing, int faceCornerCount);
    ~HE_mesh_V3();

    static HE_mesh_V3* LoadFromOBJ(const char* fromPath);
    void ToMeshResource(std::vector<float> *outVBO, std::vector<int> *outEBO);
    void Subdivide();
    inline int GetEdgePointIndex(HE_edge pair, HE_edge edge, int edgeIndex, int startEdge, int originalPointCount, HE_vert edgeStart, HE_vert edgeNextStart, HE_vert pointFace);
    inline int GetEdgePointIndex(int pair_0, int pair_3, HE_vert edgeStart, HE_vert edgeNextStart, HE_vert pointFace);
    inline HE_edge CreateEdge(float uv[2], int pair, int start);
    inline int GetRealPair(int pair, int offset, int badMod, int originalEdgeCount);
    inline int NextEdge(int edge);
};


#endif //GSCEPT_LAB_ENV_HE_MESH_V3_H
