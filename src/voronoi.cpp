#include "voronoi.hpp"
#include "stdio.h"

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define CDECL __attribute__ ((cdecl))
    #define DLL_PUBLIC __attribute__ ((dllexport))
  #else
    #define CDECL __cdecl
    #define DLL_PUBLIC __declspec(dllexport)
  #endif
#else
  #define CDECL __attribute__ ((cdecl))
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
  #else
    #define DLL_PUBLIC
  #endif
#endif

extern "C" {
  DLL_PUBLIC void CDECL get_edges(boost::int64_t** ps, size_t pointCount,
                                  boost::int64_t** ss, size_t segmentCount,
                                  void* vertexArray,
                                  void(*appendVertex)(void* outputArray, c_Vertex vertex),
                                  void* edgeArray,
                                  void(*appendEdge)(void* outputArray, c_Edge edge)) {
    boost::polygon::voronoi_builder<boost::int64_t, my_voronoi_ctype_traits> vb;

    for (size_t i = 0; i < pointCount; i++)
      vb.insert_point(ps[i][0], ps[i][1]);

    for (size_t i = 0; i < segmentCount; i++)
      vb.insert_segment(ss[i][0], ss[i][1], ss[i][2], ss[i][3]);
      
    boost::polygon::voronoi_diagram<double> vd;
    vb.construct(&vd);

    std::set<const boost::polygon::voronoi_edge<double> *> visited;
    std::map<const boost::polygon::voronoi_vertex<double> *, long long> vertexMap;

    size_t vertexCount = 1;

    for (boost::polygon::voronoi_diagram<double>::const_edge_iterator it = vd.edges().begin();  it != vd.edges().end(); ++it) {
      if (visited.find(&(*it)) != visited.end())
        continue;

      const boost::polygon::voronoi_vertex<double> *start = it->vertex0();
      const boost::polygon::voronoi_vertex<double> *end = it->vertex1();

      size_t startIndex = 0;
      if (start != 0) {
        std::map<const boost::polygon::voronoi_vertex<double> *, long long>::iterator it = vertexMap.find(start);

        if (it == vertexMap.end())
        {
          c_Vertex startVertex = c_Vertex(start->x(), start->y());
          startIndex = vertexCount++;
          appendVertex(vertexArray, startVertex);
          vertexMap[start] = startIndex;
        }
        else {
          startIndex = it->second;
        }
      }

      size_t endIndex = 0;
      if (end != 0) {
        std::map<const boost::polygon::voronoi_vertex<double> *, long long>::iterator it = vertexMap.find(end);

        if (it == vertexMap.end())
        {
          c_Vertex endVertex = c_Vertex(end->x(), end->y());
          endIndex = vertexCount++;
          appendVertex(vertexArray, endVertex);
          vertexMap[end] = endIndex;
        }
        else {
          endIndex = it->second;
        }
      }

      size_t firstIndex = it->cell()->source_index() + 1;

      const boost::polygon::voronoi_edge<double> *twin = it->twin();
      visited.insert(twin);

      size_t secondIndex = twin->cell()->source_index() + 1;

      appendEdge(edgeArray, c_Edge(startIndex, endIndex, it->is_primary(), firstIndex, secondIndex));
    }
  }
}
