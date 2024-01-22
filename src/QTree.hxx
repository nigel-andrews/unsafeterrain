#pragma once

#include <cmath>

#include "QTree.h"

template <std::size_t CHUNK_SIZE>
glm::vec2 QTree<CHUNK_SIZE>::compute_parent_pos(enum direction dir)
{
    const auto& pos = this->pos_;

    switch (dir)
    {
    case direction::top_left:
        return glm::vec2(pos.x - CHUNK_SIZE, pos.y + CHUNK_SIZE);
    case direction::top_right:
        return glm::vec2(pos.x + CHUNK_SIZE, pos.y + CHUNK_SIZE);
    case direction::bottom_left:
        return glm::vec2(pos.x - CHUNK_SIZE, pos.y - CHUNK_SIZE);
    case direction::bottom_right:
        return glm::vec2(pos.x + CHUNK_SIZE, pos.y - CHUNK_SIZE);
    default:
        break;
    }

    return glm::vec2();
}

template <std::size_t CHUNK_SIZE>
QTree<CHUNK_SIZE>::QTree(std::unique_ptr<QTree> tree, enum direction dir)
    : height_(0)
    , pos_(tree->pos_)
    , chunk_id_(0)
    , top_left_(dir == direction::bottom_right ? std::move(tree) : nullptr)
    , top_right_(dir == direction::bottom_left ? std::move(tree) : nullptr)
    , bottom_left_(dir == direction::top_right ? std::move(tree) : nullptr)
    , bottom_right_(dir == direction::top_left ? std::move(tree) : nullptr)
{}

template <std::size_t CHUNK_SIZE>
auto add_node(std::unique_ptr<QTree<CHUNK_SIZE>>& root, const glm::vec2& pos,
              std::size_t) -> std::unique_ptr<QTree<CHUNK_SIZE>>&
{
    if (!root)
        return;

    auto& node = root;
    std::optional<enum direction> dir = std::nullopt;
    while ((dir = node->is_in_bounds(pos)))
    {
        node->height_++;
        node =
            std::make_unique<QTree<CHUNK_SIZE>>(std::move(node), dir.value());
    }

    // TODO: Check if move required
    return node;
}

template <std::size_t CHUNK_SIZE>
auto QTree<CHUNK_SIZE>::is_in_bounds(const glm::vec2& pos)
    -> std::optional<enum direction>
{
    std::size_t bound = (std::pow(2, this->height_) * CHUNK_SIZE) / 2;

    if (pos.x < this->pos_.x - bound)
    {
        if (pos.y < this->pos_.y - bound)
            return direction::bottom_left;
        else
            return direction::top_left;
    }
    else if (pos.x > this->pos_.x + bound)
    {
        if (pos.y < this->pos_.y - bound)
            return direction::bottom_right;
        else
            return direction::top_right;
    }
    else
    {
        if (pos.y < this->pos_.y - bound)
            return direction::bottom_left;
        else if (pos.y > this->pos_.y + bound)
            return direction::top_right;
        else
            return std::nullopt;
    }
}
