#include "StaticMesh.h"

#include <glad/glad.h>

#include "graphics.h"

namespace OM3D
{

    extern bool audit_bindings_before_draw;

    StaticMesh::StaticMesh(const MeshData& data)
        : _vertex_buffer(data.vertices)
        , _index_buffer(data.indices)
    {}

    void StaticMesh::draw() const
    {
        _vertex_buffer.bind(BufferUsage::Attribute);
        _index_buffer.bind(BufferUsage::Index);

        // Vertex position
        DOGL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex),
                                   nullptr));
        // Vertex normal
        DOGL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex),
                                   reinterpret_cast<void*>(3 * sizeof(float))));
        // Vertex uv
        DOGL(glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex),
                                   reinterpret_cast<void*>(6 * sizeof(float))));
        // Tangent / bitangent sign
        DOGL(glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex),
                                   reinterpret_cast<void*>(8 * sizeof(float))));
        // Vertex color
        DOGL(
            glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex),
                                  reinterpret_cast<void*>(12 * sizeof(float))));

        DOGL(glEnableVertexAttribArray(0));
        DOGL(glEnableVertexAttribArray(1));
        DOGL(glEnableVertexAttribArray(2));
        DOGL(glEnableVertexAttribArray(3));
        DOGL(glEnableVertexAttribArray(4));

        if (audit_bindings_before_draw)
        {
            audit_bindings();
        }

        DOGL(glDrawElements(GL_TRIANGLES, int(_index_buffer.element_count()),
                            GL_UNSIGNED_INT, nullptr));
    }

} // namespace OM3D
