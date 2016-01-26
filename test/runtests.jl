using Voronoi
using Base.Test

points = Point[]
segments = Segment[Segment(Point(0, 0), Point(0, 1)),
                   Segment(Point(0, 1), Point(1, 1)),
                   Segment(Point(1, 1), Point(1, 0)),
                   Segment(Point(1, 0), Point(0, 0))]

vertices, edges =  get_edges(points, segments)

@test length(vertices) == 5
@test length(filter(e -> e.is_primary == true, edges)) == 4


segments = Segment[Segment(Point(0, 0), Point(0, 2)),
                   Segment(Point(0, 2), Point(1, 2)),
                   Segment(Point(1, 2), Point(1, 0)),
                   Segment(Point(1, 0), Point(0, 0))]

vertices, edges =  get_edges(points, segments)

@test length(vertices) == 6
@test length(filter(e -> e.is_primary == true, edges)) == 5
