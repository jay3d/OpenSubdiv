//
//   Copyright 2013 Pixar
//
//   Licensed under the Apache License, Version 2.0 (the "Apache License")
//   with the following modification; you may not use this file except in
//   compliance with the Apache License and the following modification to it:
//   Section 6. Trademarks. is deleted and replaced with:
//
//   6. Trademarks. This License does not grant permission to use the trade
//      names, trademarks, service marks, or product names of the Licensor
//      and its affiliates, except as required to comply with Section 4(c) of
//      the License and to reproduce the content of the NOTICE file.
//
//   You may obtain a copy of the Apache License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the Apache License with the above modification is
//   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
//   KIND, either express or implied. See the Apache License for the specific
//   language governing permissions and limitations under the Apache License.
//

#ifndef FAR_END_CAP_GREGORY_BASIS_PATCH_FACTORY_H
#define FAR_END_CAP_GREGORY_BASIS_PATCH_FACTORY_H

#include "../far/patchTablesFactory.h"
#include "../far/protoStencil.h"
#include "../vtr/level.h"

namespace OpenSubdiv {
namespace OPENSUBDIV_VERSION {

namespace Far {

class GregoryBasis;
class TopologyRefiner;

/// \brief A specialized factory to gather Gregory basis control vertices
///
class EndCapGregoryBasisPatchFactory {

public:

    //
    // Single patch GregoryBasis basis factory
    //

    /// \brief Instantiates a GregoryBasis from a TopologyRefiner that has been
    ///        refined adaptively for a given face.
    ///
    /// @param refiner     The TopologyRefiner containing the topology
    ///
    /// @param faceIndex   The index of the face (level is assumed to be MaxLevel)
    ///
    /// @param fvarChannel Index of face-varying channel topology (default -1)
    ///
    static GregoryBasis const * Create(TopologyRefiner const & refiner,
        Index faceIndex, int fvarChannel=-1);

    /// \brief Returns the maximum valence of a vertex in the mesh that the
    ///        Gregory patches can handle
    static int GetMaxValence();

public:

    ///
    /// Multi-patch Gregory stencils factory
    ///

    // XXXX need to add support for face-varying channel stencils

    /// \brief This factory accumulates vertex for Gregory basis patch
    ///
    /// @param refiner                TopologyRefiner from which to generate patches
    ///
    /// @param shareBoundaryVertices  Use same boundary vertices for neighboring
    ///                               patches. It reduces the number of stencils
    ///                               to be used.
    ///
    EndCapGregoryBasisPatchFactory(TopologyRefiner const & refiner,
                                   bool shareBoundaryVertices=true);

    ~EndCapGregoryBasisPatchFactory();

    /// \brief Returns a patch type for \a patchTag generated by this factory.
    ///
    /// @param patchTag    identified tag information for the patch
    ///
    PatchDescriptor::Type GetPatchType(PatchTablesFactoryBase::PatchFaceTag const &patchTag) const;

    /// \brief Returns an end patch vertex indices for \a faceIndex.
    ///
    /// @param level            vtr refinement level
    ///
    /// @param faceIndex        vtr faceIndex at the level
    ///
    /// @param levelPatchTags   Array of patchTags for all faces in the level
    ///
    /// @param levelVertOffset  relative offset of patch vertex indices
    ///
    ConstIndexArray GetTopology(Vtr::Level const& level, Index faceIndex,
                                PatchTablesFactoryBase::PatchFaceTag const * levelPatchTags,
                                int levelVertOffset);

    /// \brief Returns the face index in vtr level for the given \a patchIndex.
    Index GetFaceIndex(Index patchIndex) const;

    /// \brief Returns the number of gregory basis patches generated.
    ///
    int GetNumGregoryBasisPatches() const;

    /// \brief Returns the number of gregory basis vertices generated.
    ///
    int GetNumGregoryBasisVertices() const;

    /// After all the gregory basis vertices have been collected, factorize
    /// their stencils with given stencil table and create new stencil table.
    ///
    /// @param baseStencils  source stencil table
    ///
    /// @param append        if true, returns concatinated stencil table which
    ///                      also includes source stencil table at the front.
    ///
    /// @param permute       index permutation table (optional)
    ///
    StencilTables const * CreateVertexStencilTables(
        StencilTables const *baseStencils, bool append, int const permute[20]=0);
    StencilTables const * CreateVaryingStencilTables(
        StencilTables const *baseStencils, bool append, int const permute[20]=0);

private:

    /// Creates a basis for the vertices specified in mask on the face and
    /// accumates it
    bool addPatchBasis(Index faceIndex, bool newVerticesMask[4][5]);
    StencilTables const *createStencilTables(StencilAllocator &alloc,
                                             StencilTables const *baseStencils,
                                             bool append,
                                             int const permute[20]);

    struct Context;

    Context *_context;
};

} // end namespace Far

} // end namespace OPENSUBDIV_VERSION
} // end namespace OpenSubdiv

#endif  // FAR_END_CAP_GREGORY_BASIS_PATCH_FACTORY_H
