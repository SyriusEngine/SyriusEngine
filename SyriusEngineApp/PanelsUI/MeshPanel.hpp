#pragma once

#include "../Serializer/Serializer.hpp"

namespace Syrius{

    class MeshPanel{
    public:
        explicit MeshPanel(Serializer& serializer);

        ~MeshPanel();

        void draw();

    private:

        void drawMeshOptions();

    private:
        Serializer& m_Serializer;
        std::vector<Resource<Mesh>>& m_Meshes;

        int32 m_SelectedMesh;
    };

}

