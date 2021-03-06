// ======================================================================== //
// Copyright 2009-2018 Intel Corporation                                    //
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

#include "primitive.h"
#include "curveNv.h"
#include "curveNi.h"
#include "curveNi_mb.h"
#include "linei.h"
#include "triangle.h"
#include "trianglev.h"
#include "trianglev_mb.h"
#include "trianglei.h"
#include "quadv.h"
#include "quadi.h"
#include "subdivpatch1.h"
#include "object.h"
#include "instance.h"
#include "subgrid.h"

namespace embree
{
  /********************** Curve4v **************************/

  template<>
  Curve4v::Type::Type ()
    : PrimitiveType("curve4v",sizeof(Curve4v),4) {}

  template<>
  size_t Curve4v::Type::size(const char* This) const {
    return *This;
  }

  template<>
  size_t Curve4v::Type::getBytes(const char* This) const {
    return Curve4v::bytes(size(This));
  }

  /********************** Curve8v **************************/

  template<>
  Curve8v::Type::Type ()
    : PrimitiveType("curve8v",sizeof(Curve8v),8) {}

  template<>
  size_t Curve8v::Type::size(const char* This) const {
    return *This;
  }

  template<>
  size_t Curve8v::Type::getBytes(const char* This) const {
    return Curve8v::bytes(size(This));
  }

  /********************** Curve4i **************************/

  template<>
  Curve4i::Type::Type ()
    : PrimitiveType("curve4i",sizeof(Curve4v),4) {}

  template<>
  size_t Curve4i::Type::size(const char* This) const {
    return *This;
  }

  template<>
  size_t Curve4i::Type::getBytes(const char* This) const {
    return Curve4i::bytes(size(This));
  }

  /********************** Curve8i **************************/

  template<>
  Curve8i::Type::Type ()
    : PrimitiveType("curve8i",sizeof(Curve8v),8) {}

  template<>
  size_t Curve8i::Type::size(const char* This) const {
    return *This;
  }

  template<>
  size_t Curve8i::Type::getBytes(const char* This) const {
    return Curve8i::bytes(size(This));
  }

  /********************** Curve4iMB **************************/

  template<>
  Curve4iMB::Type::Type ()
    : PrimitiveType("curve4imb",sizeof(Curve4iMB),4) {}

  template<>
  size_t Curve4iMB::Type::size(const char* This) const {
    return *This;
  }

  template<>
  size_t Curve4iMB::Type::getBytes(const char* This) const {
    return Curve4iMB::bytes(size(This));
  }

  /********************** Curve8iMB **************************/

  template<>
  Curve8iMB::Type::Type ()
    : PrimitiveType("curve8imb",sizeof(Curve8iMB),8) {}

  template<>
  size_t Curve8iMB::Type::size(const char* This) const {
    return *This;
  }

  template<>
  size_t Curve8iMB::Type::getBytes(const char* This) const {
    return Curve8iMB::bytes(size(This));
  }

  /********************** Line4i **************************/

  template<>
  Line4i::Type::Type ()
    : PrimitiveType("line4i",sizeof(Line4i),4) {}

  template<>
  size_t Line4i::Type::size(const char* This) const {
    return ((Line4i*)This)->size();
  }

  /********************** Triangle4 **************************/

  template<>
  Triangle4::Type::Type ()
    : PrimitiveType("triangle4",sizeof(Triangle4),4) {}

  template<>
  size_t Triangle4::Type::size(const char* This) const {
    return ((Triangle4*)This)->size();
  }

  /********************** Triangle4v **************************/

  template<>
  Triangle4v::Type::Type ()
  : PrimitiveType("triangle4v",sizeof(Triangle4v),4) {}

  template<>
  size_t Triangle4v::Type::size(const char* This) const {
    return ((Triangle4v*)This)->size();
  }

  /********************** Triangle4i **************************/

  template<>
  Triangle4i::Type::Type ()
    : PrimitiveType("triangle4i",sizeof(Triangle4i),4) {}

  template<>
  size_t Triangle4i::Type::size(const char* This) const {
    return ((Triangle4i*)This)->size();
  }

  /********************** Triangle4vMB **************************/

  template<>
  Triangle4vMB::Type::Type ()
  : PrimitiveType("triangle4vmb",sizeof(Triangle4vMB),4) {}

  template<>
  size_t Triangle4vMB::Type::size(const char* This) const {
    return ((Triangle4vMB*)This)->size();
  }

  /********************** Quad4v **************************/

  template<>
  Quad4v::Type::Type ()
    : PrimitiveType("quad4v",sizeof(Quad4v),4) {}

  template<>
  size_t Quad4v::Type::size(const char* This) const {
    return ((Quad4v*)This)->size();
  }

  /********************** Quad4i **************************/

  template<>
  Quad4i::Type::Type ()
    : PrimitiveType("quad4i",sizeof(Quad4i),4) {}

  template<>
  size_t Quad4i::Type::size(const char* This) const {
    return ((Quad4i*)This)->size();
  }

  /********************** SubdivPatch1 **************************/

  SubdivPatch1::Type::Type ()
    : PrimitiveType("subdivpatch1",sizeof(SubdivPatch1),1) {}

  size_t SubdivPatch1::Type::size(const char* This) const {
    return 1;
  }

  SubdivPatch1::Type SubdivPatch1::type;

  /********************** Virtual Object **************************/

  Object::Type::Type ()
    : PrimitiveType("object",sizeof(Object),1) {}

  size_t Object::Type::size(const char* This) const {
    return 1;
  }

  Object::Type Object::type;

  /********************** Instance **************************/

  InstancePrimitive::Type::Type ()
    : PrimitiveType("instance",sizeof(InstancePrimitive),1) {}

  size_t InstancePrimitive::Type::size(const char* This) const {
    return 1;
  }

  InstancePrimitive::Type InstancePrimitive::type;

  /********************** SubGrid **************************/

  SubGrid::Type::Type ()
    : PrimitiveType("subgrid",sizeof(SubGrid),1) {}

  size_t SubGrid::Type::size(const char* This) const {
    return 1;
  }

  SubGrid::Type SubGrid::type;
  
  /********************** SubGridQBVH4 **************************/

  template<>
  SubGridQBVH4::Type::Type ()
    : PrimitiveType("SubGridQBVH4",sizeof(SubGridQBVH4),1) {}

  template<>
  size_t SubGridQBVH4::Type::size(const char* This) const {
    return 1;
  }

  /********************** SubGridQBVH8 **************************/

  template<>
  SubGridQBVH8::Type::Type ()
    : PrimitiveType("SubGridQBVH8",sizeof(SubGridQBVH8),1) {}

  template<>
  size_t SubGridQBVH8::Type::size(const char* This) const {
    return 1;
  }

}
