// McCAD                                                                             
#include "splitsurfaces_impl.hpp"

// C++
#include <algorithm>

bool
McCAD::Decomposition::SplitSurfaces::Impl::throughNoBoundarySurfaces(
        const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList){
    return std::any_of(
                facesList.cbegin(),
                facesList.cend(),
                [](const std::shared_ptr<Geometry::BoundSurface>& face){
                    return face->accessSImpl()->numberCollidingSurfaces == 0;
                });
}

Standard_Boolean
McCAD::Decomposition::SplitSurfaces::Impl::planeSplitOnlyPlane(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList){
    return std::any_of(
                facesList.cbegin(),
                facesList.cend(),
                [](const std::shared_ptr<Geometry::BoundSurface>& face){
                    return face->getSurfaceType() == "Plane"
                            && face->accessSImpl()->numberCollidingCurvedSurfaces == 0;
                });
}

void
McCAD::Decomposition::SplitSurfaces::Impl::generateSplitFacesList(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList,
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& selectedSplitFacesList){

    // 1st step: Select surfaces that go through 0 boundary surfaces.
    for(const auto& face : splitFacesList){
        if(face->accessSImpl()->numberCollidingSurfaces == 0)
            selectedSplitFacesList.push_back(face);
    }

    if(!selectedSplitFacesList.empty()){
        sortSplitFaces(selectedSplitFacesList);
        return;
    }

    // 2nd step: select surfaces that go through 0 curved surfaces.
    for(const auto& face : splitFacesList){
        if(face->accessSImpl()->numberCollidingCurvedSurfaces == 0
            && face->accessSImpl()->surfaceType == "Plane")
            selectedSplitFacesList.push_back(face);
    }

    if(!selectedSplitFacesList.empty()){
        sortSplitFaces(selectedSplitFacesList);
        return;
    }

    // 3th step: for cylinders

    // 4th step: select the other surfaces
    for(const auto& face : splitFacesList){
        if(face->accessSImpl()->surfaceType == "Plane")
            selectedSplitFacesList.push_back(face);
    }

    if (!selectedSplitFacesList.empty()){
        sortSplitFaces(selectedSplitFacesList);
    }
}

void
<<<<<<< HEAD
McCAD::Decomposition::SplitSurfaces::Impl::sortSplitFaces(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList){

    // Use a lambda expression as a comparator:
    // Faces which go through more concave edges have a higher priority.
    // If two faces go through the same amount of concave edges,
    // the face with less colliding surfaces has a higher priority.
    auto comparator
            = [](
            const std::shared_ptr<Geometry::BoundSurface>& first,
            const std::shared_ptr<Geometry::BoundSurface>& second){
        const auto& fst = *first->accessSImpl();
        const auto& snd = *second->accessSImpl();
        return fst.throughConcaveEdges > snd.throughConcaveEdges
                ||
                (fst.throughConcaveEdges == snd.throughConcaveEdges
                 &&
                 fst.numberCollidingSurfaces < snd.numberCollidingSurfaces);
    };

    std::sort(splitFacesList.begin(), splitFacesList.end(), comparator);

//    for(const auto& face : splitFacesList)
//        std::cout << "throughConcaveEdges: "
//                  << face->accessSImpl()->throughConcaveEdges
//                  << ", numberCollidingSurfaces: "
//                  << face->accessSImpl()->numberCollidingSurfaces
//                  << std::endl;

=======
McCAD::Decomposition::SplitSurfaces::Impl::sortSplitFaces(std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList){
  if (splitFacesList.size() < 2)
    {
      return;
    }
  // Implement more modern sorting. maybe utilizing sorting of zipped vectors.
  // A Splitting surface that goes through more concave edges has priority.
  for (Standard_Integer i = 0; i <= splitFacesList.size() - 2; ++i)
    {
      for (Standard_Integer j = i + 1; j <= splitFacesList.size() - 1; ++j)
	{
	  if (splitFacesList[i]->accessSImpl()->throughConcaveEdges < splitFacesList[j]->accessSImpl()->throughConcaveEdges)
	    {
	      std::swap(splitFacesList[i], splitFacesList[j]);
	    }
	}
    }
  // A Splitting surface that intesects with less boundary surfaces has priority.
  for (Standard_Integer i = 0; i <= splitFacesList.size() - 2; ++i)
    {
      for (Standard_Integer j = i + 1; j <= splitFacesList.size() - 1; ++j)
        {
	  if (splitFacesList[i]->accessSImpl()->throughConcaveEdges <= splitFacesList[j]->accessSImpl()->throughConcaveEdges)
	    {
	      if (splitFacesList[i]->accessSImpl()->numberCollidingSurfaces > splitFacesList[j]->accessSImpl()->numberCollidingSurfaces)
		{
		  std::swap(splitFacesList[i], splitFacesList[j]);
		}
	    }
	}
    }
  /*
  for (Standard_Integer i = 0; i <= splitFacesList.size() - 1; ++i)
    {
      //std::cout << "throughConcaveEdges: " << splitFacesList[i]->accessSImpl()->throughConcaveEdges << " , numberCollidingSurfaces: " << splitFacesList[i]->accessSImpl()->numberCollidingSurfaces << std::endl;
    }
  */
>>>>>>> 8352a6283d76be033b3ce470ada525db92b50bbe
}
