[![Build Status](https://travis-ci.org/Voxel8/Voronoi.jl.svg?branch=master)](https://travis-ci.org/Voxel8/Voronoi.jl)

[![Build status](https://ci.appveyor.com/api/projects/status/0tacpn1wpke3am0m?svg=true)](https://ci.appveyor.com/project/Voxel8/voronoi-jl)

# Voronoi

A wrapper for the [Boost Voronoi](http://www.boost.org/doc/libs/1_53_0_beta1/libs/polygon/doc/voronoi_main.htm) library,
allowing creation of Voronoi diagrams for points and straight line segments.

The only function in the library is `get_edges` which takes a vector of `Point`s and
a vector of `Segment`s with integer coordinates.

It returns `Vector{Vertex}, Vector{Edge}`. The vertices have `Float64` coordinates.

`Edge.start_index` and `Edge.end_index` are indices into the vertex vector. They can be 0
if the edge is unbounded at that end.

`Edge.site1` and `Edge.site2` are indices into the array of points, followed by the array of segments.
I.e. if there are 3 points and 5 segments, `site1 = 6` means that the first Voronoi region of this
edge belongs to the third segment.

`Edge.is_primary` specifies whether the edge is a boundary between two primary regions (points or segments).
An edge that is not primary is a boundary between a segment and one of its own endpoints
(See [Voronoi basic tutorial](http://www.boost.org/doc/libs/1_53_0_beta1/libs/polygon/doc/voronoi_basic_tutorial.htm)).

## License
Available under the [Boost Software License - Version 1.0](http://www.boost.org/LICENSE_1_0.txt).
See: [LICENSE.md](./LICENSE.md).
