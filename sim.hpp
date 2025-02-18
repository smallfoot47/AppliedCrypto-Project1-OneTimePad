#pragma once

#ifdef _DEBUG
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

#include <sstream>
#include <random>
#include <string>
#include <array>

namespace sim
{
    class Node {
        public:
            Node(): m_buf(), m_char_width(0u) {}
        public:
            const char send() const; // sends a random message
            const std::string read() const; // sends a random message
            const size_t traffic() const; // sends 
            void recieve(const char& message); // recieves a message
            void flush(); // empties buffer
        public:
            std::string m_buf;
            size_t m_char_width;
    };

    template <size_t N>
    class Simulator {
        public:
            Simulator(){};
        public:
            std::pair<size_t, size_t> run();
            const std::string peek();
        private:
        public:
            std::array<Node, N> nodes;
    };

    const char Node::send() const {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<char> printable(32, 126);

        return printable(rng); // printable ASCII range
    }

    const std::string Node::read() const {
        return m_buf;
    }

    const size_t Node::traffic() const {
        return m_char_width;
    }

    void Node::recieve(const char& message) {
        m_buf.push_back(message);
        ++m_char_width;
    }

    void Node::flush() {
        m_buf.clear();
    }

    template <size_t N>
    std::pair<size_t, size_t> Simulator<N>::run() {        
        size_t perms = N * (N - 1u);

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<size_t> index(0, perms - 1);

        size_t idx = index(rng);
        
        size_t sender_idx = idx / (N - 1u);
        size_t reciever_idx = (idx % (N - 1u) + sender_idx + 1u) % N;
        
        char data = nodes[sender_idx].send();
        nodes[reciever_idx].recieve(data);
        
        LOG("Sent from %ld -> %ld: %c\n", sender_idx, reciever_idx, data);

        return { sender_idx, reciever_idx };
    }

    template <size_t N>
    const std::string Simulator<N>::peek() {
        std::stringstream stream;

        size_t idx = 0u;
        for (auto& node : nodes) {
            stream << "<" << std::to_string(idx++) << ">:'" << node.m_buf << "'\n";
        }

        return stream.str();
    }        
} // namespace sim