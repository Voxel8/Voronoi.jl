module Voronoi
    export Point, Segment, Vertex, Edge, get_edges

    @static if is_windows()
        const library_path =  joinpath(dirname(@__FILE__), "voronoi.dll")
    end

    @static if is_unix()
        const library_path = joinpath(dirname(@__FILE__), "voronoi.so")
    end

    type Point
        X::Int32
        Y::Int32
    end

    type Segment
      p0::Point
      p1::Point
    end

    type Vertex
        X::Float64
        Y::Float64
    end

    type Edge
        start_index::UInt64
        end_index::UInt64

        is_primary::Bool

        site1::UInt64
        site2::UInt64
    end

    function Base.show(io::IO, point::Point)
      print(io, "[$(point.X),$(point.Y)]")
    end

    function Base.show(io::IO, vertex::Vertex)
      print(io, "[$(vertex.X),$(vertex.Y)]")
    end

    function Base.show(io::IO, segment::Segment)
      print(io, "($(segment.p0),$(segment.p1))")
    end

    function appendVertex!(outputArray::Ptr{Void}, vertex::Vertex)
        ourArray = unsafe_pointer_to_objref(outputArray)::Vector{Vertex}

        push!(ourArray, vertex)
    end

    function appendEdge!(outputArray::Ptr{Void}, edge::Edge)
        ourArray = unsafe_pointer_to_objref(outputArray)::Vector{Edge}

        push!(ourArray, Edge(edge.start_index, edge.end_index, edge.is_primary,
                              edge.site1, edge.site2))
    end

    function get_edges(points::Vector{Point}, segments::Vector{Segment})
      vertices = Vector{Vertex}()
      edges = Vector{Edge}()

      ccall((:get_edges, library_path), Void, (Ptr{Ptr{Cint}}, Csize_t,
                                               Ptr{Ptr{Cint}}, Csize_t,
                                               Any, Ptr{Void},
                                               Any, Ptr{Void}),
            [[p.X, p.Y] for p in points],
            length(points),
            [[s.p0.X, s.p0.Y, s.p1.X, s.p1.Y] for s in segments],
            length(segments),
            vertices,
            cfunction(appendVertex!, Any, (Ptr{Void}, Vertex)),
            edges,
            cfunction(appendEdge!, Any, (Ptr{Void}, Edge)))

      vertices, edges
    end
end
