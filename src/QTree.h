#pragma once

#include <cstddef>
#include <memory>

template <typename DEPTH>
class QTree
{
public:
    QTree() = default;
    QTree(std::size_t chunk_id)
        : chunk_id_(chunk_id)
    {}

    QTree(const QTree& rhs) = delete;

    static std::unique_ptr<QTree<DEPTH>>
    add_node(const std::unique_ptr<QTree<DEPTH>>& root, std::size_t chunk_id);

private:
    std::size_t chunk_id_;
    int depth_;

    std::unique_ptr<QTree> top_left_;
    std::unique_ptr<QTree> top_right_;
    std::unique_ptr<QTree> bottom_left_;
    std::unique_ptr<QTree> bottom_right_;
};
