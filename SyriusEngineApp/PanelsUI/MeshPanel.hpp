#pragma once

#include "../Include.hpp"
#include "../SceneObjects/Mesh.hpp"
#include "../MaterialLoader.hpp"

namespace Syrius{

    class MeshPanel{
    public:
        MeshPanel(const ResourceView<RenderCommand>& renderCommand, Worker& dispatcher);

        ~MeshPanel();

        void draw();

    private:

        void createRandomMesh();

        void drawMeshOptions();

    private:
        const ResourceView<RenderCommand>& m_RenderCommand;
        Worker& m_Dispatcher;

        int32 m_SelectedMesh;

        std::vector<Resource<Mesh>> m_Meshes;
        MaterialLoader m_MaterialLoader;
        std::vector<std::string> m_MaterialNames;
    };

}

