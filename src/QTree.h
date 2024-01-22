#pragma once

#include <cstddef>
#include <glm/glm.hpp>
#include <memory>
#include <optional>

enum class direction
{
    top_left,
    top_right,
    bottom_left,
    bottom_right,
};

template <std::size_t CHUNK_SIZE>
class QTree
{
public:
    QTree(const glm::vec2& pos, std::size_t chunk_id)
        : pos_(pos)
        , chunk_id_(chunk_id)
    {}

    template <std::size_t N>
    static void add_node(const std::unique_ptr<QTree<N>>& root,
                         std::size_t chunk_id);

private:
    QTree() = delete;
    QTree(const QTree& rhs) = delete;
    QTree(std::unique_ptr<QTree> tree, enum direction dir);

    std::optional<enum direction> is_in_bounds(const glm::vec2& pos);
    glm::vec2 compute_parent_pos(enum direction dir);

    std::size_t height_;

    glm::vec2 pos_;
    std::size_t chunk_id_;

    std::unique_ptr<QTree> top_left_;
    std::unique_ptr<QTree> top_right_;
    std::unique_ptr<QTree> bottom_left_;
    std::unique_ptr<QTree> bottom_right_;
};

#include "QTree.hxx"
