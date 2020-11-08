//
// Created by eliwar-5 on 2017-12-09.
//

#include "HE_mesh_V3.h"

HE_mesh_V3::HE_mesh_V3(std::vector<float> positions, std::vector<float> texCoords, std::vector<float> normals,
                       std::vector<int> indexing, int faceCornerCount)
{
    for(int i = 0; i < positions.size(); i+=3)
    {
        HE_vert *vertex = verts.New();
        vertex->pos[0] = positions[i];
        vertex->pos[1] = positions[i + 1];
        vertex->pos[2] = positions[i + 2];
    }

    for(int i = 0; i < indexing.size();)
    {
        HE_vert *vertex = verts[indexing[i]-1];
        HE_edge *edge = edges.New();

        vertex->edge = edges.listSize - 1;
        edge->start = indexing[i]-1;
        i++;

        if (texCoords.size() != 0)
        {
            for(int j = 0; j < 2; j++)
            {
                edge->uv[j] = texCoords[(indexing[i]-1)*2 + j];
            }
            i++;
        }

        float normal[3];
        if (normals.size() != 0)
        {
            for(int j = 0; j < 3; j++)
            {
                normal[j] = normals[(indexing[i]-1)*3 + j];
            }
            i++;
        }

        int modulo = edges.listSize % faceCornerCount;
        if (modulo == 1) //if the edge is the FIRST edge made for a face
        {

            HE_face *face = faces.New();
            face->edge = edges.listSize-1;
            if (normals.size() != 0)
            {
                for(int j = 0; j < 3; j++)
                {
                    face->norm[j] = normal[j];
                }
            }
        }

    }

    for(int i = 0; i < edges.listSize; i++)
    {
        for(int j = i; j < edges.listSize; j++)
        {
            if (edges[i]->start == edges[NextEdge(j)]->start && edges[j]->start == edges[NextEdge(i)]->start)
            {
                edges[i]->pair = j;
                edges[j]->pair = i;
                break;
            }
        }
    }
}

HE_mesh_V3::~HE_mesh_V3()
{
    for (HE_vert *vertBlocks : verts.blockPointers)
    {
        delete vertBlocks;
    }

    for (HE_edge *edgeBlocks : edges.blockPointers)
    {
        delete edgeBlocks;
    }

    for (HE_face *faceBlocks : faces.blockPointers)
    {
        delete faceBlocks;
    }
}

HE_mesh_V3 *HE_mesh_V3::LoadFromOBJ(const char *fromPath)
{
    std::vector<float> positions = std::vector<float>();
    std::vector<float> normals = std::vector<float>();
    std::vector<float> texCoords = std::vector<float>();
    std::vector<int> indexing = std::vector<int>();

    //open file
    FILE *file = fopen(fromPath, "r");
    if (file != nullptr)
    {
        int faceCornerCount = 0;
        while (true)
        {
            char string[256];
            int read = fscanf(file, "%255s", string);
            if (read == EOF)
            {
                break;
            }

            if (strcmp(string, "v") == 0)
            {
                float position[3];
                fscanf(file, "%f %f %f\n", &position[0], &position[1], &position[2]);
                positions.push_back(position[0]);
                positions.push_back(position[1]);
                positions.push_back(position[2]);
            }
            else if (strcmp(string, "vn") == 0)
            {
                float normal[3];
                fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
                normals.push_back(normal[0]);
                normals.push_back(normal[1]);
                normals.push_back(normal[2]);
            }
            else if (strcmp(string, "vt") == 0)
            {
                float texCoord[2];
                fscanf(file, "%f %f\n", &texCoord[0], &texCoord[1]);
                texCoords.push_back(texCoord[0]);
                texCoords.push_back(texCoord[1]);
            }
            else if (strcmp(string, "f") == 0)
            {
                if (normals.size() == 0 && texCoords.size() == 0)
                {
                    int indexes[4];
                    int assignments = fscanf(file, "%d %d %d %d\n", &indexes[0], &indexes[1], &indexes[2], &indexes[3]);

                    if (assignments == 3)
                        faceCornerCount = 3;
                    else if (assignments == 4)
                        faceCornerCount = 4;

                    for (int i = 0; i < assignments; i++)
                    {
                        indexing.push_back(indexes[i]);
                    }
                }
                else if (normals.size() != 0 && texCoords.size() == 0)
                {
                    int indexes[8];
                    int assignments = fscanf(file, "%d%*[//]%d %d%*[//]%d %d%*[//]%d %d%*[//]%d\n",
                                             &indexes[0], &indexes[1], &indexes[2],
                                             &indexes[3], &indexes[4], &indexes[5],
                                             &indexes[6], &indexes[7]);
                    if (assignments == 6)
                        faceCornerCount = 3;
                    else if (assignments == 8)
                        faceCornerCount = 4;

                    for (int i = 0; i < assignments; i++)
                    {
                        indexing.push_back(indexes[i]);
                    }
                }
                else if (texCoords.size() != 0 && normals.size() == 0)
                {
                    //do stuff
                }
                else if (texCoords.size() != 0 && normals.size() != 0)
                {
                    int indexes[12];
                    int assignments = fscanf(file, "%d%*[/]%d%*[/]%d %d%*[/]%d%*[/]%d %d%*[/]%d%*[/]%d %d%*[/]%d%*[/]%d\n",
                                             &indexes[0], &indexes[1], &indexes[2],
                                             &indexes[3], &indexes[4], &indexes[5],
                                             &indexes[6], &indexes[7], &indexes[8],
                                             &indexes[9], &indexes[10], &indexes[11]);
                    if (assignments == 9)
                        faceCornerCount = 3;
                    else if (assignments == 12)
                        faceCornerCount = 4;

                    for (int i = 0; i < assignments; i++)
                    {
                        indexing.push_back(indexes[i]);
                    }
                }

            }
        }

        fclose(file);
        return new HE_mesh_V3(positions, texCoords, normals, indexing, faceCornerCount);
    }

    return nullptr;
}

void HE_mesh_V3::ToMeshResource(std::vector<float> *outVBO, std::vector<int> *outEBO)
{
    std::vector<float> vbo;
    std::vector<int> ebo;

    int latestVertex = -1; //No vertices in vbo
    for(int i = 0; i < faces.listSize; i++)
    {
        HE_face *face = faces[i];
        HE_edge *edge = edges[face->edge];
        int originFace = latestVertex + 1; //First vertex of face is the last vertex of the previous face + 1

        int j = 0;
        while (j < 3)
        {
            latestVertex++;
            for(float position : verts[edge->start]->pos)
                vbo.push_back(position);
            for(float texCoord : edge->uv)
                vbo.push_back(texCoord);
            for(float normal : face->norm)
                vbo.push_back(normal);


            ebo.push_back(latestVertex);

            j++;
            edge = edges[face->edge+j];
        }

        while(j < 4)
        {
            ebo.push_back(originFace);
            ebo.push_back(latestVertex);

            latestVertex++;
            for(float position : verts[edge->start]->pos)
                vbo.push_back(position);
            for(float texCoord : edge->uv)
                vbo.push_back(texCoord);
            for(float normal : face->norm)
                vbo.push_back(normal);

            ebo.push_back(latestVertex);

            j++;
            edge = edges[face->edge+j];
        }
    }

    *outVBO = std::vector<float>(vbo);
    *outEBO = std::vector<int>(ebo);
}

void HE_mesh_V3::Subdivide()
{
    int originalPointCount = verts.listSize;
    int originalEdgeCount = edges.listSize;
    int originalFaceCount = faces.listSize;

    for (int i = 0; i < originalFaceCount; i++)
    {
        HE_face face = *faces[i]; //Current face
        HE_face face1, face2, face3; //New faces

        //Edges of current face
        HE_edge e0 = *edges[face.edge+0];
        HE_edge e1 = *edges[face.edge+1];
        HE_edge e2 = *edges[face.edge+2];
        HE_edge e3 = *edges[face.edge+3];

        //New first(_1), second(_2) and third(_3) next edge of edges e0, e1, e2 and e3
        HE_edge e0_1, e0_2, e0_3;
        HE_edge e1_1, e1_2, e1_3;
        HE_edge e2_1, e2_2, e2_3;
        HE_edge e3_1, e3_2, e3_3;

        //Pair of current edges
        HE_edge pair0 = *edges[e0.pair];
        HE_edge pair1 = *edges[e1.pair];
        HE_edge pair2 = *edges[e2.pair];
        HE_edge pair3 = *edges[e3.pair];

        //Starting points of current edges, all the corners in the face
        HE_vert point0 = *verts[e0.start];
        //verts[e0.start]->edge = face.edge;
        HE_vert point1 = *verts[e1.start];
        verts[e1.start]->edge = edges.listSize;
        HE_vert point2 = *verts[e2.start];
        verts[e2.start]->edge = edges.listSize+4;
        HE_vert point3 = *verts[e3.start];
        verts[e3.start]->edge = edges.listSize+8;

        //Get real pairs
        int e0Pair = GetRealPair(e0.pair, 3, 3, originalEdgeCount);
        int e1Pair = GetRealPair(e1.pair, 3, 3, originalEdgeCount);
        int e2Pair = GetRealPair(e2.pair, 3, 3, originalEdgeCount);
        int e3Pair = GetRealPair(e3.pair, 3, 3, originalEdgeCount);

        int e0_3Pair = GetRealPair(e3.pair, -4, 0, originalEdgeCount);
        int e1_3Pair = GetRealPair(e0.pair, -4, 0, originalEdgeCount);
        int e2_3Pair = GetRealPair(e1.pair, -4, 0, originalEdgeCount);
        int e3_3Pair = GetRealPair(e2.pair, -4, 0, originalEdgeCount);

        //Create face point
        HE_vert pointFace;
        pointFace.edge = face.edge+2;
        for (int j = 0; j < 3; j++)
        {
            //Average of all the corners in the face
            pointFace.pos[j] = ( point0.pos[j] + point1.pos[j] + point2.pos[j] + point3.pos[j] ) * 0.25f;
        }
        *verts.New() = pointFace;

        //Index to edge points on e0, e1, e2, e3 and index to face point
        int pointIndexFace = verts.listSize-1; //Index of face point is equal to the index of the next HE_vert created with verts
        int pointIndexE0 = GetEdgePointIndex(e0Pair, e1_3Pair, point0, point1, pointFace);
        int pointIndexE1 = GetEdgePointIndex(e1Pair, e2_3Pair, point1, point2, pointFace);
        int pointIndexE2 = GetEdgePointIndex(e2Pair, e3_3Pair, point2, point3, pointFace);
        int pointIndexE3 = GetEdgePointIndex(e3Pair, e0_3Pair, point3, point0, pointFace);

        //uv values for edges with pointIndexes E0, E1, E2, E3 and Face as their .start
        float uvPIE0[2] = { (e0.uv[0] + e1.uv[0]) * 0.5f , (e0.uv[1] + e1.uv[1]) * 0.5f }; //Average uv of e0 and e1
        float uvPIE1[2] = { (e1.uv[0] + e2.uv[0]) * 0.5f , (e1.uv[1] + e2.uv[1]) * 0.5f }; //Average uv of e1 and e2
        float uvPIE2[2] = { (e2.uv[0] + e3.uv[0]) * 0.5f , (e2.uv[1] + e3.uv[1]) * 0.5f }; //Average uv of e2 and e3
        float uvPIE3[2] = { (e3.uv[0] + e0.uv[0]) * 0.5f , (e3.uv[1] + e0.uv[1]) * 0.5f }; //Average uv of e3 and e0
        float uvFace[2] = { (e0.uv[0] + e1.uv[0] + e2.uv[0] + e3.uv[0]) * 0.25f ,          //Average uv of e0, e1, e2 and e3
                            (e0.uv[1] + e1.uv[1] + e2.uv[1] + e3.uv[1]) * 0.25f };

        ///Then check if there is a realpair that is >= edges.listSize. This means that a new edge point has to be made

        //Set values for e0 and all the new edges coming after e0
        e0.pair = e0Pair;
        e0_1 = CreateEdge(uvPIE0, edges.listSize+2, pointIndexE0);
        e0_2 = CreateEdge(uvFace, edges.listSize+9, pointIndexFace);
        e0_3 = CreateEdge(uvPIE3, e0_3Pair, pointIndexE3);

        //Set values for e1 and all the new edges coming after e1
        e1.pair = e1Pair;
        e1_1 = CreateEdge(uvPIE1, edges.listSize+6, pointIndexE1);
        e1_2 = CreateEdge(uvFace, face.edge+1, pointIndexFace);
        e1_3 = CreateEdge(uvPIE0, e1_3Pair, pointIndexE0);

        //Set values for e2 and all the new edges coming after e2
        e2.pair = e2Pair;
        e2_1 = CreateEdge(uvPIE2, edges.listSize+10, pointIndexE2);
        e2_2 = CreateEdge(uvFace, edges.listSize+1, pointIndexFace);
        e2_3 = CreateEdge(uvPIE1, e2_3Pair, pointIndexE1);

        //Set values for e3 and all the new edges coming after e3
        e3.pair = e3Pair;
        e3_1 = CreateEdge(uvPIE3, face.edge+2, pointIndexE3);
        e3_2 = CreateEdge(uvFace, edges.listSize+5, pointIndexFace);
        e3_3 = CreateEdge(uvPIE2, e3_3Pair, pointIndexE2);

        //Set value of all new faces (face0 would have been identical to the original face)
        for (int j = 0; j < 3; j++)
        {
            face1.norm[j] = face.norm[j];
            face2.norm[j] = face.norm[j];
            face3.norm[j] = face.norm[j];
        }
        face1.edge = edges.listSize+0;
        face2.edge = edges.listSize+4;
        face3.edge = edges.listSize+8;

        //Save all edges and faces.
        *edges[face.edge+0] = e0;
        *edges[face.edge+1] = e0_1;
        *edges[face.edge+2] = e0_2;
        *edges[face.edge+3] = e0_3;

        *edges.New() = e1;
        *edges.New() = e1_1;
        *edges.New() = e1_2;
        *edges.New() = e1_3;

        *edges.New() = e2;
        *edges.New() = e2_1;
        *edges.New() = e2_2;
        *edges.New() = e2_3;

        *edges.New() = e3;
        *edges.New() = e3_1;
        *edges.New() = e3_2;
        *edges.New() = e3_3;

        *faces.New() = face1;
        *faces.New() = face2;
        *faces.New() = face3;
    }

    //Set all the corners, edge points and face normals to their correct values
    for (int i = 0; i < originalPointCount; i++)
    {
        HE_vert point = *verts[i];
        /*std::vector<HE_vert> fpoints;
        std::vector<HE_vert> epoints;
        std::vector<int> faceIndexes;*/
        float avEdgePoints[3] = {0,0,0};
        float avFacePoints[3] = {0,0,0};

        int loops = 0;
        int edgeIndex = point.edge;
        do
        {
            ///Get edges
            HE_edge e0 = *edges[edgeIndex];
            HE_edge e1 = *edges[edgeIndex+1];
            HE_edge e2 = *edges[edgeIndex+2];

            ///Do face point stuff
            HE_vert facePoint = *verts[e2.start];
            avFacePoints[0] += facePoint.pos[0];
            avFacePoints[1] += facePoint.pos[1];
            avFacePoints[2] += facePoint.pos[2];

            ///Do edge point stuff
            HE_vert edgePoint = *verts[e1.start];
            avEdgePoints[0] += edgePoint.pos[0];
            avEdgePoints[1] += edgePoint.pos[1];
            avEdgePoints[2] += edgePoint.pos[2];

            ///Do face stuff
            /*fpoints.push_back(facePoint);
            epoints.push_back(edgePoint);
            faceIndexes.push_back(edgeIndex/4);*/

            loops++;
            edgeIndex = e0.pair-3;
        } while(edgeIndex != point.edge);

        float denominator = 1.0f/loops;
        point.pos[0] = ( ( 4*avEdgePoints[0] - avFacePoints[0] ) * denominator + (loops-3)*point.pos[0] ) * denominator;
        point.pos[1] = ( ( 4*avEdgePoints[1] - avFacePoints[1] ) * denominator + (loops-3)*point.pos[1] ) * denominator;
        point.pos[2] = ( ( 4*avEdgePoints[2] - avFacePoints[2] ) * denominator + (loops-3)*point.pos[2] ) * denominator;
        *verts[i] = point;

        /*int l = loops-1;
        for (int j = 0; j < loops; j++)
        {
            //Vectors describing the face
            float a[3] = {epoints[l].pos[0]-fpoints[j].pos[0], epoints[l].pos[1]-fpoints[j].pos[1], epoints[l].pos[2]-fpoints[j].pos[2]};
            float b[3] = {epoints[j].pos[0]-fpoints[j].pos[0], epoints[j].pos[1]-fpoints[j].pos[1], epoints[j].pos[2]-fpoints[j].pos[2]};

            //Crossproduct to get the normal
            float normal[3] = {
                    a[1]*b[2]-a[2]*b[1],
                    a[2]*b[0]-a[0]*b[2],
                    a[0]*b[1]-a[1]*b[0]
            };

            //Get the length of the vector
            float magnitude = 1.0f / sqrtf(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);

            //Normalize and assign new normal to face
            for (int k = 0; k < 3; k++)
            {
                //Update face normals
                faces[faceIndexes[j]]->norm[k] = normal[k] * magnitude;
            }

            l = j;
        }*/
    }
}

int HE_mesh_V3::GetEdgePointIndex(HE_edge pair, HE_edge edge, int edgeIndex, int startEdge, int originalPointCount, HE_vert edgeStart, HE_vert edgeNextStart, HE_vert pointFace)
{
    if (pair.pair != edgeIndex || pair.start >= originalPointCount)
    {
        int index;
        switch (edge.pair % 4)
        {
            case 0:
                index = edges[edge.pair+1]->start;
                break;
            case 1:
                index = edges[pair.pair-1]->start;
                break;
            case 2:
                index = edges[pair.pair+2]->start;
                break;
            case 3:
                index = pair.start;
                break;
        }
        for (int i = 0; i < 3; i++)
        {
            //verts[index]->pos[i] += pointFace.pos[i] * 0.25f;
        }
        return index;
    }
    else
    {
        HE_vert edgePoint;
        edgePoint.edge = startEdge;
        for (int i = 0; i < 3; i++)
        {
            edgePoint.pos[i] = ( edgeStart.pos[i] + edgeNextStart.pos[i] + pointFace.pos[i] ) * 0.25f;
            //edgePoint.pos[i] = ( edgeStart.pos[i] + edgeNextStart.pos[i] ) * 0.5f;
        }

        *verts.New() = edgePoint;
        return verts.listSize-1;
    }
}

int HE_mesh_V3::GetEdgePointIndex(int pair_0, int pair_3, HE_vert edgeStart, HE_vert edgeNextStart, HE_vert pointFace)
{
    if (pair_0 < edges.listSize && pair_3 < edges.listSize)
    {
        //EdgePoint already exists
        int index = edges[pair_0]->start;
        for (int i = 0; i < 3; i++)
        {
            verts[index]->pos[i] += pointFace.pos[i] * 0.25f;
        }
        return index;
    }
    else
    {
        //EdgePoint does not exist
        HE_vert edgePoint;
        edgePoint.edge = pair_0;
        for (int i = 0; i < 3; i++)
        {
            edgePoint.pos[i] = ( edgeStart.pos[i] + edgeNextStart.pos[i] + pointFace.pos[i] ) * 0.25f;
        }

        *verts.New() = edgePoint;
        return verts.listSize-1;
    }
}

HE_edge HE_mesh_V3::CreateEdge(float *uv, int pair, int start)
{
    HE_edge edge;
    edge.uv[0] = uv[0];
    edge.uv[1] = uv[1];
    edge.pair = pair;
    edge.start = start;
    return edge;
}

int HE_mesh_V3::GetRealPair(int pair, int offset, int badMod, int originalEdgeCount)
{
    int face = pair / 4;
    int mod = pair % 4;
    return mod == badMod ? pair : originalEdgeCount + face*12 + mod*4 + offset;
    //Det blir 3*4 nya edges för varje face och för varje ny face läggs 4 nya edges så man kan
    //få en face edgeIndex med faceIndex * 4. Det ger att e3.pair - (e3.pair % 4) blir den första
    //edgen i det facet. Och då vet du hur många faces som kommer att skapas mellan det facet du
    //är på nu och facet för e3.pair så då kan du även ta reda på vilket index varje ny edge i
    //facet för e3.pair som kommer att skapas
}

int HE_mesh_V3::NextEdge(int edge)
{
    return edge % 4 != 3 ? edge + 1 : edge - 3;
}