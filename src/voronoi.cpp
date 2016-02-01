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
  DLL_PUBLIC void CDECL get_edges(int** ps, size_t pointCount,
                                  int** ss, size_t segmentCount,
                                  void* vertexArray,
                                  void(*appendVertex)(void* outputArray, c_Vertex vertex),
                                  void* edgeArray,
                                  void(*appendEdge)(void* outputArray, c_Edge edge)) {
    std::vector<Point> points;
    std::vector<Segment> segments;
    voronoi_diagram<double> vd;

    for (size_t i = 0; i < pointCount; i++)
      points.push_back(Point(ps[i][0], ps[i][1]));

    for (size_t i = 0; i < segmentCount; i++)
      segments.push_back(Segment(Point(ss[i][0], ss[i][1]), Point(ss[i][2], ss[i][3])));

    construct_voronoi(points.begin(), points.end(), segments.begin(), segments.end(), &vd);

    std::set<const voronoi_edge<double> *> visited;
    std::map<const voronoi_vertex<double> *, long long> vertexMap;

    size_t vertexCount = 1;

    for (voronoi_diagram<double>::const_edge_iterator it = vd.edges().begin();  it != vd.edges().end(); ++it) {
      if (visited.find(&(*it)) != visited.end())
        continue;

      const voronoi_vertex<double> *start = it->vertex0();
      const voronoi_vertex<double> *end = it->vertex1();

      size_t startIndex = 0;
      if (start != 0) {
        std::map<const voronoi_vertex<double> *, long long>::iterator it = vertexMap.find(start);

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
        std::map<const voronoi_vertex<double> *, long long>::iterator it = vertexMap.find(end);

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

      const voronoi_edge<double> *twin = it->twin();
      visited.insert(twin);

      size_t secondIndex = twin->cell()->source_index() + 1;

      appendEdge(edgeArray, c_Edge(startIndex, endIndex, it->is_primary(), firstIndex, secondIndex));
    }
  }
}
