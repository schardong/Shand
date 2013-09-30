// Copyright Akira Takahashi 2012
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/detail/lightweight_test.hpp>
#include <deque>
#include <string>
#include <shand/graph/euler_path.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/assign/list_of.hpp>

template <class Container>
class add_path {
    Container& path_;
public:
    add_path(Container& path) : path_(path) {}

    template <class VertexDescriptor>
    void operator()(VertexDescriptor v)
    {
        path_.push_front(v);
    }
};

struct null_add_path {
    template <class VertexDescriptor>
    void operator()(VertexDescriptor) {}
};

template <class Graph, class Vertex, class VertexDescriptor>
void success_test(const Graph& g,
                  const Vertex& start,
                  const std::deque<VertexDescriptor>& expected)
{
    std::deque<VertexDescriptor> path;

    add_path<std::deque<VertexDescriptor> > adder(path);
    if (!shand::graph::euler_path(g, start, adder)) {
        BOOST_TEST(false);
        return;
    }

    BOOST_TEST(path == expected);
}

template <class Graph, class Vertex>
void fail_test(const Graph& g, const Vertex& start)
{
    BOOST_TEST(!shand::graph::euler_path(g, start, null_add_path()));
}

//        A
//        +
//     -     -
// B +---------+ C
//   | -     - |
//   |    +    |
//   | -     - |
// D +---------+ E

int main()
{
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor vertex_desc;

    enum {A, B, C, D, E, N};

    // House of Santa
    // http://www.mathematische-basteleien.de/house.html
    const std::vector<std::pair<int, int> > edges = {
        {A, B},
        {B, C},
        {C, A},
        {B, D},
        {B, E},
        {C, D},
        {D, E},
        {E, C}
    };

    const Graph g(edges.begin(), edges.end(), N);

    {
        const std::deque<vertex_desc> expected = boost::assign::list_of(E)(B)(A)(C)(B)(D)(C)(E)(D);
        success_test(g, E, expected);
    }
    {
        const std::deque<vertex_desc> expected = boost::assign::list_of(D)(B)(A)(C)(B)(E)(D)(C)(E);
        success_test(g, D, expected);
    }

    fail_test(g, A);
    fail_test(g, B);
    fail_test(g, C);

    return boost::report_errors();
}

