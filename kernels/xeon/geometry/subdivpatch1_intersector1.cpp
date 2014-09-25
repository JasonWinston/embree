// ======================================================================== //
// Copyright 2009-2014 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "subdivpatch1_intersector1.h"

namespace embree
{

  void subdivide_intersect1(Ray& ray,
			    const IrregularCatmullClarkPatch &patch,
			    const unsigned int subdiv_level)
  {
    if (subdiv_level == 0)
      {
	__aligned(64) FinalQuad finalQuad;
	patch.init( finalQuad );
        
	intersectTri(finalQuad.vtx[0],
                     finalQuad.vtx[1],
                     finalQuad.vtx[2],
                     ray,
                     finalQuad.geomID,
                     finalQuad.primID,NULL); 

	intersectTri(finalQuad.vtx[2],
                     finalQuad.vtx[3],
                     finalQuad.vtx[0],
                     ray,
                     finalQuad.geomID,
                     finalQuad.primID,NULL); 
      }
    else
      {
	IrregularCatmullClarkPatch subpatches[4];
	patch.subdivide(subpatches);
	for (size_t i=0;i<4;i++)
	  subdivide_intersect1(ray,
			       subpatches[i],
			       subdiv_level - 1);	    
      }   
  }
};
