#ifndef MESH_H
#define MESH_H

#include "smartpointerhelp.h"
#include"drawable.h"
#include "halfedge.h"
#include "face.h"
#include "vertex.h"

class Mesh : public Drawable
{
public:
        std::vector<uPtr<HalfEdge>> halfEdges;
        std::vector<uPtr<Vertex>> vertices;
        std::vector<uPtr<Face>> faces;
        std::vector<HalfEdge*> singleEdges;
        \
        Mesh(OpenGLContext* context);
        ~Mesh();

        void create() override;
        void destroy();
        GLenum drawMode() override;
        void splitEdge(HalfEdge* he);
        void splitEdge(HalfEdge* he, const glm::vec3& pos);
        void triangulate(Face* f);
        int getEdgeNum(Face* face);
        void subdivide();
        void computeCentroidPoints(std::vector<uPtr<Vertex>>*,
                                   std::map<Face*, Vertex*>*);
        void computeMidpoints(const std::map<Face*, Vertex*>* ctPointMap,
                              std::map<HalfEdge*, Vertex*>* midPointMap);
        void smoothVertex(const std::map<Face*, Vertex*>* ctPointMap,
                          const unsigned int& count);
        void quadrangulate( std::map<Face*, Vertex*> *centroidMap,
                            std::vector<uPtr<HalfEdge>> *newHalfEdges,
                            std::vector<uPtr<Face>> *newFaces);

private:


};

#endif // MESH_H
