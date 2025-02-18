#pragma once
#include <cstddef>
#include <array>

#ifdef _DEBUG
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

namespace alg
{
    struct Node {
        size_t left, right;
    };

    template <size_t N>
    class Padder {
        public:
            Padder(const size_t L);
            ~Padder();
        public:
            const std::pair<char, bool> alloc(const size_t idx);
        public:
            char* padspace;
            size_t m_pads;
            std::array<Node, N> m_nodes;
    };

    template <size_t N>
    Padder<N>::Padder(const size_t L): m_pads(L) {
        const auto step = L / N;
        size_t idx = 0u;

        for (auto& node : m_nodes) {
            LOG("[%ld] at [%ld]\n", idx, idx * step);

            node.left = idx * step;
            node.right = idx++ * step;
        }

        padspace = new char[L];
    }

    template <size_t N>
    Padder<N>::~Padder() {
        delete [] padspace;
    }

    template <size_t N>
    const std::pair<char, bool> Padder<N>::alloc(const size_t idx) {
        auto& curr = m_nodes[idx];
        auto& prev = m_nodes[(idx - 1) % N];
        auto& next = m_nodes[(idx + 1) % N];
        auto left_width = curr.left - prev.right;
        auto right_width = next.left - curr.right;

        char pad;
        
        if (left_width <= 0 || right_width <= 0) { // non-trivial case
            bool isLegal = false;

            if (prev.left > curr.right) { // prev is ahead, legal
                left_width += m_pads;
                isLegal = true;
            }

            if (next.right < curr.left) { // next is behind, legal
                right_width += m_pads;
                isLegal = true;
            }

            if (! isLegal)
                return { '\0', false };
        }

        if (left_width > right_width) { // move left
            pad = padspace[++curr.left];
            LOG("pad %c at [%ld] allocated to %ld", pad, curr.left, idx);
        } else { // move right
            pad = padspace[++curr.right];
            LOG("pad %c at [%ld] allocated to %ld", pad, curr.right, idx);
        }

        return { pad, true };
    }
} // namespace alg