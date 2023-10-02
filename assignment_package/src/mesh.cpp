#include "mesh.h"
#include<iostream>

Mesh::Mesh(OpenGLContext* context)
    : Drawable(context), halfEdges{}, vertices{}, faces{}
{}

Mesh::~Mesh()
{}

int Mesh::getEdgeNum(Face* face)
{
    int num = 0;
    HalfEdge* curr = face->halfEdge;
    do {
        num++;
        curr = curr->nextHE;
    } while (curr != face->halfEdge);

    return num;
}

void Mesh::create()
{
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> nor;
    std::vector<glm::vec4> color;
    std::vector<GLuint> indices;
    std::vector<glm::ivec2> jointID;
    std::vector<glm::vec2> jointWeight;

    std::cout << "Mesh::create" << std::endl;

    for (uPtr<Face> &f : faces)
    {
        int currPosCount = pos.size();

        int edgeNum = getEdgeNum(f.get());

        for(int i = 0; i < edgeNum - 2; i++)
        {
            indices.emplace_back(currPosCount);
            indices.emplace_back(currPosCount + i + 1);
            indices.emplace_back(currPosCount + i + 2);
        }

        HalfEdge *currHE = f->halfEdge;

        do
        {
            //nor
            glm::vec3 v1 = currHE->nextHE->vertex->pos - currHE->vertex->pos;
            glm::vec3 v2 = currHE->nextHE->nextHE->vertex->pos - currHE->nextHE->vertex->pos;

            HalfEdge* tp_currHE = currHE;

            while (glm::length(glm::cross(v1, v2)) < FLT_EPSILON)
            {
               tp_currHE = tp_currHE->nextHE;
               v1 = tp_currHE->nextHE->vertex->pos - tp_currHE->vertex->pos;
               v2 = tp_currHE->nextHE->nextHE->vertex->pos - tp_currHE->nextHE->vertex->pos;
            }

            glm::vec4 normal = glm::vec4(glm::normalize(glm::cross(v1, v2)), 0);
            nor.push_back(normal);

            //pos
            pos.push_back(glm::vec4(currHE->vertex->pos, 1));

            //color
            color.push_back(glm::vec4(f->color, 1));

            Vertex* vt = currHE->vertex;
            if (!vt->jWeights.empty())
            {
                jointID.push_back(glm::ivec2(vt->jWeights[0].first, vt->jWeights[1].first));
                jointWeight.push_back(glm::vec2(vt->jWeights[0].second, vt->jWeights[1].second));
            }

            currHE = currHE->nextHE;

        }
        while(currHE != f->halfEdge);
    }

    //init count in creat()
    count = indices.size();

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    generatePos();
    bindPos();
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateNor();
    bindNor();
    mp_context->glBufferData(GL_ARRAY_BUFFER, nor.size() * sizeof(glm::vec4), nor.data(), GL_STATIC_DRAW);

    generateCol();
    bindCol();
    mp_context->glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(glm::vec4), color.data(), GL_STATIC_DRAW);

    generateJointID();
    bindJointID();
    mp_context->glBufferData(GL_ARRAY_BUFFER, jointID.size() * sizeof(glm::ivec2), jointID.data(), GL_STATIC_DRAW);

    generateJointWeight();
    bindJointWeight();
    mp_context->glBufferData(GL_ARRAY_BUFFER, jointWeight.size() * sizeof(glm::vec2), jointWeight.data(), GL_STATIC_DRAW);

}

void Mesh::destroy()
{
    for(uPtr<Vertex> &vt : this->vertices)
    {
        vt->clearCount();
    }

    for(uPtr<Face> &f : this->faces)
    {
        f->clearCount();
    }

    for(uPtr<HalfEdge> &he : this->halfEdges)
    {
        he->clearCount();
    }
    this->vertices.clear();
    this->faces.clear();
    this->halfEdges.clear();

    Drawable::destroy();
}

void Mesh::splitEdge(HalfEdge* he)
{
    Vertex* vt1 = he->vertex;
    Vertex* vt2 = he->sym->vertex;
    splitEdge(he, (vt1->pos + vt2->pos) / 2.f);
}

void Mesh::splitEdge(HalfEdge* he, const glm::vec3& pos)
{
    Vertex* vt1 = he->vertex;
    Vertex* vt2 = he->sym->vertex;
    HalfEdge* hesym = he->sym;

    uPtr<Vertex> v3 = mkU<Vertex>();
    v3->pos = pos;

    uPtr<HalfEdge> he1 = mkU<HalfEdge>();
    uPtr<HalfEdge> he2 = mkU<HalfEdge>();

    if(he1->halfEdgeId == 168 || he2->halfEdgeId == 168)
    {
        std::cout<<he->halfEdgeId<<std::endl;
    }
    //set sym
    he1->vertex = vt1;
    he2->vertex = vt2;
    vt1->halfEdge = he1.get();
    vt2->halfEdge = he2.get();

    he->sym = he2.get();
    he2->sym = he;

    hesym->sym = he1.get();
    he1->sym = hesym;

    //set new cycle
    he1->nextHE = he->nextHE;
    he->nextHE = he1.get();

    he2->nextHE = hesym->nextHE;
    hesym->nextHE = he2.get();
    he->vertex = v3.get();
    hesym->vertex = v3.get();

    he1->face = he->face;
    he2->face = hesym->face;
    he->face->halfEdge = he1.get();
    hesym->face->halfEdge = he2.get();
    v3->halfEdge = he1.get();

    //update vectors
    vertices.push_back(std::move(v3));
    halfEdges.push_back(std::move(he1));
    halfEdges.push_back(std::move(he2));
}

void Mesh::triangulate(Face* f)
{
    int edgeNum = getEdgeNum(f);
    for(int i = 0; i < edgeNum - 3; i++)
    {
        HalfEdge* he_0 = f->halfEdge;
        HalfEdge* he_1 = he_0->nextHE;
        HalfEdge* he_2 = he_1->nextHE;

        Vertex* vt1 = he_0->vertex;
        Vertex* vt2 = he_2->vertex;

        uPtr<HalfEdge> he_A = mkU<HalfEdge>();
        uPtr<HalfEdge> he_B = mkU<HalfEdge>();
        he_A->vertex = vt1;
        he_B->vertex = vt2;
        he_A->sym = he_B.get();
        he_B->sym = he_A.get();

        uPtr<Face> face2 = mkU<Face>();
        face2->halfEdge = he_2;
        he_1->face = face2.get();
        he_2->face = face2.get();
        he_A->face = face2.get();
        he_B->face = f;

        he_B->nextHE = he_2->nextHE;
        he_2->nextHE = he_A.get();
        he_A->nextHE = he_1;
        he_0->nextHE = he_B.get();

        halfEdges.push_back(std::move(he_A));
        halfEdges.push_back(std::move(he_B));
        faces.push_back(std::move(face2));
    }
}

void Mesh::computeCentroidPoints(std::vector<uPtr<Vertex>> *newVertices,
                                 std::map<Face*, Vertex*> *ctPointMap)
{
    for(uPtr<Face>& f : faces)
    {
        uPtr<Vertex> ctPoint = mkU<Vertex>();
        glm::vec3 posSum = glm::vec3();
        HalfEdge* currHE = f->halfEdge;

        do
        {
            posSum += currHE->vertex->pos;
            currHE = currHE->nextHE;
        } while (currHE != f->halfEdge);

        posSum /= getEdgeNum(f.get());
        ctPoint->pos = posSum;

        //std::cout << "CT: " << ctPoint->vertexId << " : " << ctPoint->pos.x << "," << ctPoint->pos.y << "," << ctPoint->pos.z << "," << std::endl;
        ctPointMap->insert({f.get(), ctPoint.get()});
        newVertices->push_back(std::move(ctPoint));
    }
}

void  Mesh::computeMidpoints(const std::map<Face*, Vertex*>* ctPointMap,
                           std::map<HalfEdge*, Vertex*>* midPointMap)
{
    for (HalfEdge *he: singleEdges)
    {
        glm::vec3 f1 = ctPointMap->find(he->face)->second->pos;
        glm::vec3 v1 = he->vertex->pos;
        glm::vec3 v2 = he->sym->vertex->pos;;
        glm::vec3 f2 = ctPointMap->find(he->sym->face)->second->pos;

        glm::vec3 pos = (v1 + v2 + f1 + f2) / 4.0f;
        splitEdge(he, pos);

        midPointMap->insert({vertices[vertices.size() - 1].get()->halfEdge,
                             vertices[vertices.size() - 1].get()});
    }
}

std::vector<Vertex*> getAdjacentVerts(Vertex* vt)
{
    HalfEdge* currHE = vt->halfEdge;
    std::vector<Vertex*> neighbors;

    do
    {
        currHE = currHE->nextHE;
        neighbors.push_back(currHE->vertex);
        currHE = currHE->sym;
    }while(currHE != vt->halfEdge);

    return neighbors;
}

std::vector<Face*> getAdjacentFaces(Vertex* vt)
{
    HalfEdge* currHE = vt->halfEdge;
    std::vector<Face*> neighbors;

    do
    {
        currHE = currHE->nextHE;
        neighbors.push_back(currHE->face);
        currHE = currHE->sym;
    }while(currHE != vt->halfEdge);

    return neighbors;
}

void Mesh::smoothVertex(const std::map<Face*, Vertex*> *ctPointMap,
                        const unsigned int& count)
{
    for (unsigned int i = 0; i < count; ++i)
    {
       Vertex* vt = vertices[i].get();

       HalfEdge *currHE = vt->halfEdge;
       glm::vec3 eSum = glm::vec3();
       glm::vec3 fSum = glm::vec3();
       float count = 0;

       do
       {
          glm::vec3 midPointPos = currHE->nextHE->vertex->pos;
          glm::vec3 ctPointPos = ctPointMap->find(currHE->face)->second->pos;

          eSum += midPointPos;
          fSum += ctPointPos;
          currHE = currHE->nextHE->sym;
          count++;
       } while (currHE != vt->halfEdge);

      eSum /= pow(count, 2);
      fSum /= pow(count, 2);

      vt->pos = ((count - 2.f)*vt->pos / count) + eSum + fSum;
    }

}

void Mesh::quadrangulate(std::map<Face*, Vertex*> *ctPointMap,
                         std::vector<uPtr<HalfEdge>> *newHalfEdges,
                         std::vector<uPtr<Face>> *newFaces)
{
    for (uPtr<Face> &f : faces)
    {
        Vertex* ctPoint = ctPointMap->find(f.get())->second;

        HalfEdge* originHE = f->halfEdge;
        HalfEdge* currHE = originHE;
        HalfEdge* nextHE = currHE->nextHE;
        HalfEdge* nextCycleHE = nextHE->nextHE;
        HalfEdge* lastSymSetHE = nullptr;
        HalfEdge* symSetHE = nullptr;
        bool faceUsed = false;
        do
        {
            uPtr<HalfEdge> heIn = mkU<HalfEdge>();
            uPtr<HalfEdge> heOut = mkU<HalfEdge>();

            heIn->vertex = ctPoint;
            ctPoint->halfEdge = heIn.get();
            heIn->nextHE = heOut.get();

            heOut->vertex = currHE->sym->vertex;
            heOut->nextHE = currHE;

            nextHE->nextHE = heIn.get();

            if(!faceUsed)
            {
                faceUsed = true;
                currHE->face = f.get();
                nextHE->face = f.get();
                heIn->face = f.get();
                heOut->face = f.get();
                lastSymSetHE = heOut.get();
            }
            else
            {
                uPtr<Face> newFace = mkU<Face>();
                newFace->setRdmColor();
                newFace->halfEdge = currHE;
                currHE->face = newFace.get();
                nextHE->face = newFace.get();
                heIn->face = newFace.get();
                heOut->face = newFace.get();
                newFaces->push_back(std::move(newFace));

                heOut->sym = symSetHE;
                symSetHE->sym = heOut.get();

                if(nextCycleHE == originHE)
                {
                    heIn->sym = lastSymSetHE;
                    lastSymSetHE->sym = heIn.get();
                }
            }

            symSetHE = heIn.get();

            currHE = nextCycleHE;
            nextHE = currHE->nextHE;
            nextCycleHE = nextHE->nextHE;

            newHalfEdges->push_back(std::move(heIn));
            newHalfEdges->push_back(std::move(heOut));

        }while(currHE != originHE);


    }
}

void Mesh::subdivide()
{
    std::vector<uPtr<Vertex>> newVertices;
    std::vector<uPtr<HalfEdge>> newHalfEdges;
    std::vector<uPtr<Face>> newFaces;

    std::map<Face*, Vertex*> ctPointMap;
    std::map<HalfEdge*, Vertex*> midpointMap;
    singleEdges.clear();
    for (uPtr<HalfEdge> &he: halfEdges)
    {
        if (std::find(singleEdges.begin(), singleEdges.end(),he.get()->sym) == singleEdges.end())
        {
            singleEdges.push_back(he.get());
        }
    }

    computeCentroidPoints(&newVertices, &ctPointMap);
    unsigned int vCount = vertices.size();
    computeMidpoints(&ctPointMap, &midpointMap);

    smoothVertex(&ctPointMap, vCount);
    quadrangulate(&ctPointMap, &newHalfEdges, &newFaces);

    for (uPtr<Vertex> &vt : newVertices)
    {
        vertices.push_back(std::move(vt));
    }

    for (uPtr<HalfEdge> &he : newHalfEdges)
    {
        halfEdges.push_back(std::move(he));
    }

    for (uPtr<Face> &face: newFaces)
    {
        faces.push_back(std::move(face));
    }
}

GLenum Mesh::drawMode()
{
    return GL_TRIANGLES;
}
