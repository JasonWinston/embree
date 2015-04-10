// ======================================================================== //
// Copyright 2009-2015 Intel Corporation                                    //
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

#include "default.h"
#include "geometry.h"
#include "primref.h"
#include "buffer.h"

namespace embree
{
    struct BezierCurves : public Geometry
    {
      struct Vertex {
        float x,y,z,r;
      };

      static const Geometry::Type geom_type = Geometry::BEZIER_CURVES;

    public:
      /*! bezier curve construction */
      BezierCurves (Scene* parent, RTCGeometryFlags flags, size_t numPrimitives, size_t numVertices, size_t numTimeSteps); 
    
      /*! wirte the bezier curve geometry to disk */
      void write(std::ofstream& file);

    public:
      void enabling();
      void disabling();
      void setMask (unsigned mask);
      void setBuffer(RTCBufferType type, void* ptr, size_t offset, size_t stride);
      void* map(RTCBufferType type);
      void unmap(RTCBufferType type);
      void immutable ();
      bool verify ();

    public:

      /*! returns number of bezier curves */
      __forceinline size_t size() const {
	return numPrimitives;
      }

      /*! returns the i'th curve */
      __forceinline const int& curve(size_t i) const {
        assert(i < numPrimitives);
        return curves[i];
      }

      /*! returns i'th vertex of j'th timestep */
      __forceinline const Vec3fa& vertex(size_t i, size_t j = 0) const {
        assert(i < numVertices);
        assert(j < numTimeSteps);
        return (Vec3fa&)vertices[j][i];
      }

      /*! returns i'th radius of j'th timestep */
      __forceinline float radius(size_t i, size_t j = 0) const {
        assert(i < numVertices);
        assert(j < numTimeSteps);
        return vertices[j][i].r;
      }

      /*! check if the i'th primitive is valid */
      __forceinline bool valid(size_t i, BBox3fa* bbox = nullptr) const 
      {
	const int index = curve(i);
	if (index+3 >= numVertices) return false;

	for (size_t j=0; j<numTimeSteps; j++) 
	{
	  const float r0 = radius(index+0,j);
	  const float r1 = radius(index+1,j);
	  const float r2 = radius(index+2,j);
	  const float r3 = radius(index+3,j);
	  if (!isvalid(r0) || !isvalid(r1) || !isvalid(r2) || !isvalid(r3))
	    return false;
          if (min(r0,r1,r2,r3) < 0.0f)
            return false;

	  const Vec3fa& v0 = vertex(index+0,j);
	  const Vec3fa& v1 = vertex(index+1,j);
	  const Vec3fa& v2 = vertex(index+2,j);
	  const Vec3fa& v3 = vertex(index+3,j);
	  if (!isvalid(v0) || !isvalid(v1) || !isvalid(v2) || !isvalid(v3))
	    return false;
	}

	if (bbox) *bbox = bounds(i);
	return true;
      }

      /*! calculates bounding box of i'th bezier curve */
      __forceinline BBox3fa bounds(size_t i, size_t j = 0) const 
      {
        const int index = curve(i);
        const float r0 = radius(index+0,j);
        const float r1 = radius(index+1,j);
        const float r2 = radius(index+2,j);
        const float r3 = radius(index+3,j);
        const Vec3fa& v0 = vertex(index+0,j);
        const Vec3fa& v1 = vertex(index+1,j);
        const Vec3fa& v2 = vertex(index+2,j);
        const Vec3fa& v3 = vertex(index+3,j);
        const BBox3fa b = merge(BBox3fa(v0),BBox3fa(v1),BBox3fa(v2),BBox3fa(v3));
        return enlarge(b,Vec3fa(max(r0,r1,r2,r3)));
      }

      /*! calculates bounding box of i'th bezier curve */
      __forceinline BBox3fa bounds(const AffineSpace3fa& space, size_t i, size_t j = 0) const 
      {
        const int index = curve(i);
        const float r0 = radius(index+0,j);
        const float r1 = radius(index+1,j);
        const float r2 = radius(index+2,j);
        const float r3 = radius(index+3,j);
        const Vec3fa& v0 = xfmPoint(space,vertex(index+0,j));
        const Vec3fa& v1 = xfmPoint(space,vertex(index+1,j));
        const Vec3fa& v2 = xfmPoint(space,vertex(index+2,j));
        const Vec3fa& v3 = xfmPoint(space,vertex(index+3,j));
        const BBox3fa b = merge(BBox3fa(v0),BBox3fa(v1),BBox3fa(v2),BBox3fa(v3));
        return enlarge(b,Vec3fa(max(r0,r1,r2,r3)));
      }

      __forceinline const Vec3fa *fristVertexPtr(size_t i) const { // FIXME: remove, use buffer to access vertices instead!
        return &vertex(curve(i));
      }

#if defined(__MIC__)

      /*! calculates bounding box of i'th bezier curve */
      __forceinline mic2f bounds_mic2f(size_t i) const 
      {
        const int index = curve(i);
        const Vec3fa& cp0 = vertex(index+0);
        const Vec3fa& cp1 = vertex(index+1);
        const Vec3fa& cp2 = vertex(index+2);
        const Vec3fa& cp3 = vertex(index+3);
	
	const mic_m m_4f = 0xf;
	const mic_f v0 = permute<0,0,0,0>(uload16f(m_4f,(float*)&cp0));
	const mic_f v1 = permute<0,0,0,0>(uload16f(m_4f,(float*)&cp1));
	const mic_f v2 = permute<0,0,0,0>(uload16f(m_4f,(float*)&cp2));
	const mic_f v3 = permute<0,0,0,0>(uload16f(m_4f,(float*)&cp3));

	const mic_f b_min = min(min(v0,v1),min(v2,v3));
	const mic_f b_max = max(max(v0,v1),max(v2,v3));

	const mic_f b_min_r = b_min - swDDDD(b_max);
	const mic_f b_max_r = b_max + swDDDD(b_max);

        return mic2f(b_min_r,b_max_r);
      }
      
#endif

    public:
      unsigned int mask;                //!< for masking out geometry
      BufferT<int> curves;              //!< array of curve indices
      BufferT<Vertex> vertices[2];      //!< vertex array
      size_t numVertices;               //!< number of vertices
    };
}
