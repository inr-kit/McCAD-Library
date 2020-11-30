// McCAD
#include "boundingBox_impl.hpp"

McCAD::Geometry::BoundingBox::BoundingBox(
        const Coord3D& minima,
        const Coord3D& maxima)
    : minima_{minima}, maxima_{maxima}, gap_{CT_0()}{
    if(maxima.x < minima.x || maxima.y < minima.y || maxima.z < minima.z){

	throw std::runtime_error{
        "Minimum coordinates must be smaller than maximum coordinates"};
    };
}

const McCAD::Geometry::Coord3D&
McCAD::Geometry::BoundingBox::minima() const{
    return minima_;
}

const McCAD::Geometry::Coord3D&
McCAD::Geometry::BoundingBox::maxima() const{
    return maxima_;
}

const McCAD::Geometry::coord_type&
McCAD::Geometry::BoundingBox::getGap() const{
    return gap_;
}

bool
McCAD::Geometry::BoundingBox::setGap(
        coord_type gap){
    if(gap >= CT_0()){
        auto diff = gap - gap_;
        auto diffVec = Coord3D{diff, diff, diff};
        maxima_ += diffVec;
        minima_ -= diffVec;
        gap_ = gap;
        return true;
    }
    return false;
}

void
McCAD::Geometry::BoundingBox::resetGap(){
    setGap(CT_0());
}

McCAD::Geometry::coord_type
McCAD::Geometry::BoundingBox::minSize() const{
    return std::min({
                        maxima_.x.value - minima_.x.value,
                        maxima_.y.value - minima_.y.value,
                        maxima_.z.value - minima_.z.value
                    });

}

McCAD::Geometry::coord_type
McCAD::Geometry::BoundingBox::maxSize() const{
    return std::max({
                        maxima_.x.value - minima_.x.value,
                        maxima_.y.value - minima_.y.value,
                        maxima_.z.value - minima_.z.value
                    });
}

McCAD::Geometry::coord_type
McCAD::Geometry::BoundingBox::diagonal() const{
    return length(maxima_ - minima_);
}

McCAD::Geometry::BoundingBox
McCAD::Geometry::detail::makeBoundingBox(
        const TopoDS_Solid& solid){
    Bnd_Box boundingBox;
    BRepBndLib::Add(solid, boundingBox);
    Coord3D min, max;
    boundingBox.Get(min.x.value, min.y.value, min.z.value,
                    max.x.value, max.y.value, max.z.value);
    return {min, max};
}
