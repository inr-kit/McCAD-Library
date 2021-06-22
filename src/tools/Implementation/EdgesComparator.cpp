// McCAD
#include "EdgesComparator.hpp"
// OCC
#include <GeomAbs_CurveType.hxx>
#include <gp_Lin.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>

McCAD::Tools::EdgesComparator::EdgesComparator(){
}

McCAD::Tools::EdgesComparator::EdgesComparator(const Standard_Real& angularTolerance,
                                               const Standard_Real& distanceTolerance,
                                               const Standard_Real& precision)
    : angularTolerance{angularTolerance}, distanceTolerance{distanceTolerance},
      precision{precision}{
}

McCAD::Tools::EdgesComparator::~EdgesComparator(){
}

Standard_Boolean
McCAD::Tools::EdgesComparator::operator()(const TopoDS_Edge& firstEdge,
                                          const TopoDS_Edge& secondEdge){
    // Compare edge types first.
    BRepAdaptor_Curve firstAdaptor(firstEdge);
    BRepAdaptor_Curve secondAdaptor(secondEdge);
    if(firstAdaptor.GetType() != secondAdaptor.GetType()) return Standard_False;
    else {
        if(firstAdaptor.GetType() == GeomAbs_Line)
            return compareLines(firstAdaptor, secondAdaptor);
        else if(firstAdaptor.GetType() == GeomAbs_Circle)
            return compareCircles(firstAdaptor, secondAdaptor);
        else if(firstAdaptor.GetType() == GeomAbs_Ellipse)
            return compareEllipses(firstAdaptor, secondAdaptor);
    }
    return Standard_False;
}

Standard_Boolean
McCAD::Tools::EdgesComparator::compareLines(const BRepAdaptor_Curve& firstEdge,
                                            const BRepAdaptor_Curve& secondEdge){
    gp_Lin firstLine = firstEdge.Line();
    gp_Lin secondLine = secondEdge.Line();
    // Compare the locations.
    if(!firstLine.Location().IsEqual(secondLine.Location(), distanceTolerance))
        return Standard_False;
    // Compare the the normals.
    if(!firstLine.Position().IsParallel(secondLine.Position(), angularTolerance) &&
            !firstLine.Position().IsOpposite(secondLine.Position(), angularTolerance))
        return Standard_False;
    // Compare the distance.
    if(firstLine.Distance(secondLine) > distanceTolerance)
        return Standard_False;
    // Compare the angle.
    if(firstLine.Angle(secondLine) > angularTolerance)
        return Standard_False;
    return Standard_True;
}

Standard_Boolean
McCAD::Tools::EdgesComparator::compareCircles(const BRepAdaptor_Curve& firstEdge,
                                              const BRepAdaptor_Curve& secondEdge){
    gp_Circ firstCircle = firstEdge.Circle();
    gp_Circ secondCircle = secondEdge.Circle();
    // Compare the locations.
    if(!firstCircle.Location().IsEqual(secondCircle.Location(), distanceTolerance))
        return Standard_False;
    // Compare the plane of the circles.
    if(!firstCircle.Axis().IsParallel(secondCircle.Axis(), angularTolerance) &&
            !firstCircle.Axis().IsOpposite(secondCircle.Axis(), angularTolerance))
        return Standard_False;
    // Compare the radii.
    if(std::abs(firstCircle.Radius() - secondCircle.Radius()) > precision)
        return Standard_False;
    // Compare the circuferences.
    if(std::abs(firstCircle.Length() - secondCircle.Length()) > precision)
        return Standard_False;
    // Compare the areas.
    if(std::abs(firstCircle.Area() - secondCircle.Area()) > precision)
        return Standard_False;
    return Standard_True;
}

Standard_Boolean
McCAD::Tools::EdgesComparator::compareEllipses(const BRepAdaptor_Curve& firstEdge,
                                               const BRepAdaptor_Curve& secondEdge){
    gp_Elips firstEllipse = firstEdge.Ellipse();
    gp_Elips secondEllipse = secondEdge.Ellipse();
    // Compare the locations.
    if(!firstEllipse.Location().IsEqual(secondEllipse.Location(), distanceTolerance))
        return Standard_False;
    // Compare the plane of the circles.
    if(!firstEllipse.Axis().IsParallel(secondEllipse.Axis(), angularTolerance) &&
            !firstEllipse.Axis().IsOpposite(secondEllipse.Axis(), angularTolerance))
        return Standard_False;
    // Compare the radii.
    if(std::abs(firstEllipse.MajorRadius() - secondEllipse.MajorRadius()) > precision
            || std::abs(firstEllipse.MinorRadius() - secondEllipse.MinorRadius()) > precision)
        return Standard_False;
    // Compare the focal distance.
    if(std::abs(firstEllipse.Focal() - secondEllipse.Focal()) > precision)
        return Standard_False;
    // Compare the areas.
    if(std::abs(firstEllipse.Area() - secondEllipse.Area()) > precision)
        return Standard_False;
    return Standard_True;
}
