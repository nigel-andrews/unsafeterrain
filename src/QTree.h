#pragma once

#include <cstddef>
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <ostream>

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
        : chunk_id_(chunk_id)
        , pos_(pos)
        , height_(0)
    {
        QTree<CHUNK_SIZE>::chunk_count++;
    }

    QTree(std::unique_ptr<QTree> tree, enum direction dir)
        : chunk_id_(0)
        , pos_(tree->compute_parent_pos(dir))
        , height_(tree->height_ + 1)
        , top_left_(dir == direction::bottom_right ? std::move(tree) : nullptr)
        , top_right_(dir == direction::bottom_left ? std::move(tree) : nullptr)
        , bottom_left_(dir == direction::top_right ? std::move(tree) : nullptr)
        , bottom_right_(dir == direction::top_left ? std::move(tree) : nullptr)
    {}

    static auto add_node(std::unique_ptr<QTree<CHUNK_SIZE>>& root,
                         const glm::vec2& pos)
        -> std::unique_ptr<QTree<CHUNK_SIZE>>&&;

    template <size_t N>
    friend std::ostream& operator<<(std::ostream& os, const QTree<N>& qt);

    // Do not touch very dangerous onyo >w<
    static size_t chunk_count;

private:
    QTree() = delete;
    QTree(const QTree& rhs) = delete;

    QTree(size_t height, glm::vec2 pos)
        : chunk_id_(0)
        , pos_(pos)
        , height_(height)
    {}

    std::optional<enum direction> is_in_bounds(const glm::vec2& pos);
    glm::vec2 compute_parent_pos(enum direction dir);

    static void add_node_aux(std::unique_ptr<QTree<CHUNK_SIZE>>& qt,
                             const glm::vec2& pos);
    std::ostream& dump_dot(std::ostream& os) const;

    // Only valid for leaf (height_ == 0) nodes.
    std::size_t chunk_id_;

    glm::vec2 pos_;
    std::size_t height_;

    std::unique_ptr<QTree> top_left_;
    std::unique_ptr<QTree> top_right_;
    std::unique_ptr<QTree> bottom_left_;
    std::unique_ptr<QTree> bottom_right_;
};

template <size_t CHUNK_SIZE>
std::ostream& operator<<(std::ostream& os, QTree<CHUNK_SIZE> const& qt);

#include "QTree.hxx"
