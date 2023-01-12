#ifndef EDGETYPE_HPP
#define EDGETYPE_HPP


namespace McCAD::Tools{
    class EdgeType{
        public:
            EdgeType() = default;

            enum customEDGEConvexity{
                concave = 0,
                convex,
                flat,
                ignore
            };
    };
}

#endif // EDGETYPE_HPP
