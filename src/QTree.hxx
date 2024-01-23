#pragma once

#include <cmath>
#include <format>
#include <memory>
#include <sstream>
#include <utility>

#include "QTree.h"

namespace OM3D
{
    template <u32 CHINK_SIZE>
    std::size_t QTree<CHINK_SIZE>::chunk_count = 0;

    template <u32 CHUNK_SIZE>
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

        std::unreachable();
    }

    namespace
    {
        static enum direction get_dir(const glm::vec2& from,
                                      const glm::vec2& to)
        {
            if (to.x < from.x)
            {
                if (to.y < from.y)
                    return direction::bottom_left;
                else
                    return direction::top_left;
            }
            else if (to.x > from.x)
            {
                if (to.y < from.y)
                    return direction::bottom_right;
                else
                    return direction::top_right;
            }
            else
                return direction::top_right;
        }
    } // namespace

#define RECURSE_QTREE(Target, Offset)                                          \
    if (!Target)                                                               \
        Target = std::unique_ptr<QTree<CHUNK_SIZE>>(new QTree<CHUNK_SIZE>(     \
            qt->height_ - 1, qt->pos_ + quarter_width * (Offset)));            \
    return add_node_aux(Target, pos);

    template <u32 CHUNK_SIZE>
    void QTree<CHUNK_SIZE>::add_node_aux(std::unique_ptr<QTree<CHUNK_SIZE>>& qt,
                                         const glm::vec2& pos)
    {
        // TODO: Figure out chunk IDs

        direction dir = get_dir(qt->pos_, pos);

        if (qt->height_ == 1)
        {
            auto new_chunk = std::make_unique<QTree<CHUNK_SIZE>>(
                pos, QTree<CHUNK_SIZE>::chunk_count);

            switch (dir)
            {
            case direction::top_left:
                qt->top_left_ = std::move(new_chunk);
                break;
            case direction::top_right:
                qt->top_right_ = std::move(new_chunk);
                break;
            case direction::bottom_left:
                qt->bottom_left_ = std::move(new_chunk);
                break;
            case direction::bottom_right:
                qt->bottom_right_ = std::move(new_chunk);
                break;
            }

            return;
        }

        float quarter_width = (std::pow(2, qt->height_) * CHUNK_SIZE) / 4;

        switch (dir)
        {
        case direction::bottom_left:
            RECURSE_QTREE(qt->bottom_left_, glm::vec2(-1, -1));
        case direction::top_left:
            RECURSE_QTREE(qt->top_left_, glm::vec2(-1, 1));
        case direction::bottom_right:
            RECURSE_QTREE(qt->bottom_right_, glm::vec2(1, -1));
        case direction::top_right:
            RECURSE_QTREE(qt->top_right_, glm::vec2(1, 1));
        }
    }

    template <u32 CHUNK_SIZE>
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
            node = std::make_unique<QTree<CHUNK_SIZE>>(std::move(node),
                                                       dir.value());
        }

        if (node->height_ != 0)
            add_node_aux(node, pos);

        return std::move(node);
    }

    template <u32 CHUNK_SIZE>
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

    namespace
    {
        std::ostream& operator<<(std::ostream& os, const glm::vec2& v)
        {
            return os << "vec2(" << v.x << ", " << v.y << ")";
        }

        std::ostream& operator<<(std::ostream& os, const direction dir)
        {
            switch (dir)
            {
            case direction::top_left:
                return os << "top_left";
            case direction::top_right:
                return os << "top_right";
            case direction::bottom_left:
                return os << "bottom_left";
            case direction::bottom_right:
                return os << "bottom_right";
            }

            std::unreachable();
        }
    } // namespace

#define PRINT_QTREE(Child)                                                     \
    if (Child)                                                                 \
    {                                                                          \
        os << '"' << this << "\" -> \"" << Child << "\"\n";                    \
        Child->dump_dot(os);                                                   \
    }

    template <u32 CHINK_SIZE>
    std::ostream& QTree<CHINK_SIZE>::dump_dot(std::ostream& os) const
    {
        PRINT_QTREE(this->top_left_);
        PRINT_QTREE(this->top_right_);
        PRINT_QTREE(this->bottom_left_);
        PRINT_QTREE(this->bottom_right_);

        return os;
    }

    template <u32 CHUNK_SIZE>
    std::ostream& operator<<(std::ostream& os, QTree<CHUNK_SIZE> const& qt)
    {
        os << "digraph CHUNK {\n";
        qt.dump_dot(os);
        os << "}";

        return os;
    }
} // namespace OM3D
