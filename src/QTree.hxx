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
    }
}

template <std::size_t CHUNK_SIZE>
auto QTree<CHUNK_SIZE>::add_node(std::unique_ptr<QTree<CHUNK_SIZE>>& root,
                                 const glm::vec2& pos)
    -> std::unique_ptr<QTree<CHUNK_SIZE>>&&
{
    if (!root)
        return std::move(root);

    auto& node = root;
    std::optional<enum direction> dir = std::nullopt;
    while ((dir = node->is_in_bounds(pos)))
    {
        node->height_++;
        node =
            std::make_unique<QTree<CHUNK_SIZE>>(std::move(node), dir.value());
    }

    return std::move(node);
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

template <size_t CHUNK_SIZE>
std::ostream& operator<<(std::ostream& os, QTree<CHUNK_SIZE> const& qt)
{
    os << "QTree {\n" //
       << "\tpos_: " << qt.pos_ << "\n"
       << "\theight_: " << qt.height_ << "\n"
       << "\tchunk_id_: " << qt.chunk_id_ << "\n";

    if (qt.top_left_)
        os << "\ttop_left_: " << qt.top_left_ << "\n";
    if (qt.top_right_)
        os << "\ttop_right_: " << qt.top_right_ << "\n";
    if (qt.bottom_left_)
        os << "\tbottom_left_: " << qt.bottom_left_ << "\n";
    if (qt.bottom_right_)
        os << "\tbottom_right_: " << qt.bottom_right_ << "\n";

    os << "}";

    return os;
}
