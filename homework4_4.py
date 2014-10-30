"""
@author: Yuchen Hou
"""
import pydot
g1 = pydot.Dot(graph_type='digraph')
g1.add_node(pydot.Node('buy', label='buy'))
g1.add_node(pydot.Node('game', label='game'))
g1.add_node(pydot.Node('likes', label='likes'))
g1.add_node(pydot.Node('personality', label='personality'))
g1.add_node(pydot.Node('rating', label='rating'))
g1.add_edge(pydot.Edge('rating', 'buy'))
g1.add_edge(pydot.Edge('likes', 'buy'))
g1.add_edge(pydot.Edge('game', 'likes'))
g1.add_edge(pydot.Edge('personality', 'likes'))
g1.write_png('homework4.png')
